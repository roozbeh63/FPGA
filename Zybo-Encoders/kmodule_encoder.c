#include <linux/module.h>
#include <linux/of.h> /* "Open Firmware" */
#include <linux/of_address.h>
#include <linux/of_device.h>
#include <asm/io.h>
#include <linux/slab.h>
#include <linux/fs.h>
#include <linux/cdev.h>

#define DRIVER_AUTHOR "Janou Christophe <j.christophe@student.fontys.nl>, Marco Veltman <m.veltman@student.fontys.nl>"
#define DRIVER_DESC   "Device driver"
#define DEV_NAME 	  "zedboard_enc"

//#define DEBUG

#define NDEVICES 6
#define REG_DIRECTION 0x004
#define DIR_MASK 0x80000000
#define POS_MASK 0x7FFFFFFF

/* parameters */
static int enc_ndevices = NDEVICES;
/* ================================================================ */

static unsigned int enc_major = 0;
static struct cdev *enc_devices = NULL;
static struct class *enc_class = NULL;
static int devices_to_destroy = 0;
/* ================================================================ */

struct zybo_enc_dev {
    const void* _register;
    const void* _position;
    const void* _direction;
};

struct zybo_enc_dev *encoders;

/* Table of "compatible" values to search for */
static const struct of_device_id zybo_encoder_of_match[] = {
{ .compatible = "xlnx,rotary-encoder-1.0", },
{},
};
MODULE_DEVICE_TABLE(of, zybo_encoder_of_match);

static int enc_open(struct inode *inode, struct file *file)
{
    try_module_get(THIS_MODULE);

#ifdef DEBUG
    printk(KERN_INFO "Device opened\n");
#endif
    return 0;
}
static int enc_release(struct inode *inode, struct file *f)
{
    module_put(THIS_MODULE);

#ifdef DEBUG
    printk(KERN_ALERT "Device closed\n");
#endif
    return 0;
}

static ssize_t enc_read(struct file *f, char *buf,
                        size_t count, loff_t *f_pos)
{
    int pos;
    int dir;

    int minor = iminor(f->f_path.dentry->d_inode);

#ifdef DEBUG
    printk("minor: %d\n", minor);
    unsigned long cast;
    int valuer;
    int value;
    int valuee;
    int valuecn;
    int valueecn;
    value = ioread32(encoders[minor]._direction);
    valuee = ioread32(encoders[minor]._position);
    valuecn = be32_to_cpup(encoders[minor]._direction);
    valueecn = be32_to_cpup(encoders[minor]._position);
    printk("%d - %d, %d - %d\n", value, valuecn, valuee, valueecn);

    cast = (int)encoders[minor]._register;
    //The correct way to access hardware registers is with iowrite32(), ioread32() and other io-something functions and macros. All device drivers demonstrate this.
    valuer = ioread32(encoders[minor]._register);
    printk("%lu - %p , %d\n", cast, encoders[minor]._register, valuer);
#endif

    //real data
    pos = ioread32(encoders[minor]._register);
    //dir = ioread32(encoders[minor]._register + REG_DIRECTION);

    dir = (pos & DIR_MASK)>>31;
    pos &= POS_MASK;

    printk("position: %d, direction %d\n", pos, dir);
    return 0;
}

struct file_operations enc_fops = {
    .owner =    THIS_MODULE,
    .open = enc_open,
    .release = enc_release,
    .read = enc_read
};

/* ================================================================ */
/* Setup and register the device with specific index (the index is also
 * the minor number of the device).
 * Device class should be created beforehand.
 */
static int
enc_construct_device(struct cdev *dev, int minor,
                     struct class *class)
{
    int err = 0;
    dev_t devno = MKDEV(enc_major, minor);
    struct device *device = NULL;

    /* Memory is to be allocated when the device is opened the first time */

    cdev_init(dev, &enc_fops);
    dev->owner = THIS_MODULE;

    err = cdev_add(dev, devno, 1);
    if (err)
    {
        printk(KERN_WARNING "[target] Error %d while trying to add %s%d",
               err, DEV_NAME, minor);
        return err;
    }

    device = device_create(class, NULL, /* no parent device */
                           devno, NULL, /* no additional data */
                           DEV_NAME "%d", minor);

    if (device == NULL) {
        err = PTR_ERR(device);
        printk(KERN_WARNING "[target] Error %d while trying to create %s%d",
               err, DEV_NAME, minor);
        cdev_del(dev);
        return err;
    }
    return 0;
}

/* Destroy the device and free its buffer */
static void
enc_destroy_device(struct cdev *dev, int minor,
                   struct class *class)
{
    device_destroy(class, MKDEV(enc_major, minor));
    cdev_del(dev);
    return;
}

static void
enc_cleanup_module(int devices_to_destroy)
{
    int i;

    /* Get rid of character devices (if any exist) */
    if (enc_devices) {
        for (i = 0; i < devices_to_destroy; ++i) {
            enc_destroy_device(&enc_devices[i], i, enc_class);
        }
        kfree(enc_devices);
    }
    if (encoders)
        kfree(encoders);

    if (enc_class)
        class_destroy(enc_class);

    /* [NB] enc_cleanup_module is never called if alloc_chrdev_region()
     * has failed. */
    unregister_chrdev_region(MKDEV(enc_major, 0), enc_ndevices);
    return;
}

/* ================================================================ */

static int zybo_encoder_probe(struct platform_device *pdev)
{
    static int count = 0;
    const struct of_device_id *match;
    int rc = 0;
    struct resource res;
    void *registers;
    const void* ptr_direction;
    const void* ptr_position;
    int err;

#ifdef DEBUG
    printk("Probe Call %d\n", count);
#endif

    match = of_match_device(zybo_encoder_of_match, &pdev->dev);

    if (!match)
        return -EINVAL;


    rc = of_address_to_resource(pdev->dev.of_node, 0, &res);
    if (rc) {
        /* Fail */
    }

    if  (!request_mem_region(res.start, resource_size(&res), "IP_Enc_Struct")) {
        /* Fail */
    }

    registers = of_iomap(pdev->dev.of_node, 0);

    if (!registers) {
        /* Fail */
    }
    encoders[count]._register = registers;

    ptr_position = of_get_property(pdev->dev.of_node, "xlnx,s00-axi-data-width", NULL);

    if (!ptr_position) {
        /* Couldn't find the entry */
    }

    encoders[count]._position = ptr_position;

    ptr_direction = of_get_property(pdev->dev.of_node, "xlnx,s00-axi-addr-width", NULL);

    if (!ptr_direction) {
        /* Couldn't find the entry */
    }

    encoders[count]._direction = ptr_direction;

    err = enc_construct_device(&enc_devices[count], count, enc_class);
    if (err) {
        devices_to_destroy = count;
        goto fail;
    }

    count++;
    return 0;

fail:
    enc_cleanup_module(devices_to_destroy);
    return err;
}

static int zybo_encoder_remove(struct platform_device *pdev)
{
    pr_info("Driver removed\n");

    return 0;
}

static struct platform_driver zybo_encoder_driver = {
    .probe = zybo_encoder_probe,
    .remove = zybo_encoder_remove,
    .driver = {
        .name = DEV_NAME,
        .owner = THIS_MODULE,
        .of_match_table = zybo_encoder_of_match,
    },
};

static int __init
enc_init_module(void)
{
    int err = 0;
    dev_t dev = 0;

    if (enc_ndevices <= 0)
    {
        printk(KERN_WARNING "[target] Invalid value of enc_ndevices: %d\n",
               enc_ndevices);
        err = -EINVAL;
        return err;
    }

    /* Get a range of minor numbers (starting with 0) to work with */
    err = alloc_chrdev_region(&dev, 0, enc_ndevices, DEV_NAME);
    if (err < 0) {
        printk(KERN_WARNING "[target] alloc_chrdev_region() failed\n");
        return err;
    }
    enc_major = MAJOR(dev);

    /* Create device class (before allocation of the array of devices) */
    enc_class = class_create(THIS_MODULE, DEV_NAME);
    if (IS_ERR(enc_class)) {
        err = PTR_ERR(enc_class);
        goto fail;
    }

    /* Allocate the array of devices */
    enc_devices = (struct cdev *)kzalloc(
                enc_ndevices * sizeof(struct cdev),
                GFP_KERNEL);
    encoders = (struct zybo_enc_dev *)kzalloc(
                enc_ndevices * sizeof(struct zybo_enc_dev),
                GFP_KERNEL);
    if (enc_devices == NULL) {
        err = -ENOMEM;
        goto fail;
    }

    platform_driver_register(&zybo_encoder_driver);

    return 0; /* success */

fail:
    enc_cleanup_module(devices_to_destroy);
    return err;
}

static void __exit
enc_exit_module(void)
{
    enc_cleanup_module(enc_ndevices);
    platform_driver_unregister(&zybo_encoder_driver);
    return;
}

module_init(enc_init_module);
module_exit(enc_exit_module);
MODULE_LICENSE("GPL");
MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESC);
/* ================================================================ */
