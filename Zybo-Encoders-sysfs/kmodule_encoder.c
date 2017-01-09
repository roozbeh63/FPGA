/****************************************************/
/*													*/
/*	Version 1.0		 								*/
/*	Author: Bas Janssen								*/
/*	Lectoraat Robotics and High Tech Mechatronics 	*/
/*	2016				 							*/
/*													*/
/****************************************************/

#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/kernel.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/types.h>
#include <linux/kfifo.h>
#include <linux/ioport.h>
#include <linux/slab.h>
#include <linux/of.h>
#include <linux/of_device.h>
#include <linux/of_address.h>
#include <linux/platform_device.h>
#include <linux/list.h>
#include <asm/io.h>
#include <asm/uaccess.h>

#define CLASS_NAME "ENC"
#define DEVICE_NAME "ENC"
#define DIR_MASK 0x80000000
#define POS_MASK 0x7FFFFFFF
#define N_ENCODER_MINORS 32

static struct class* Encoder_class = NULL;
static int Encoder_major = 0;

static DECLARE_BITMAP(minors, N_ENCODER_MINORS);

static LIST_HEAD(device_list);
static DEFINE_MUTEX(device_list_lock);

//Make sure only one proccess can accessour the device
static DEFINE_MUTEX(Encoder_device_mutex);

static int memory_request = 0;

//Custom struct to store the data we want in the driver
struct Encoder_data {
    uint32_t* base_address;
    char      direction;
    uint32_t  position;
    int 	  message_read;
    uint32_t   base_register;
    struct list_head device_entry;
    dev_t		   devt;
};


static ssize_t Encoder_read(struct file* filp, char __user *buffer, size_t lenght, loff_t* offset)
{
    struct Encoder_data *Encoder;
    ssize_t retval = -1;
    ssize_t copied = 0;
    unsigned int fpga_value = 0;
    char int_array[20];

    //Grab the Encoder_data struct out of the file struct.
    Encoder = filp->private_data;

    //cat keeps requesting new data until it receives a "return 0", so we do a one shot.
    if(Encoder->message_read)
        return 0;
    //Read from the I/O register
    fpga_value = ioread32(Encoder->base_address);
    printk("value:%d\n", fpga_value);
    copied = snprintf(int_array, 20, "%i\n", fpga_value);

    retval = copy_to_user(buffer, &int_array, copied);
    Encoder->message_read = 1;
    if(retval)
        return retval;
    return retval ? retval : copied;
}

static ssize_t Encoder_write(struct file* filp, const char __user *buffer, size_t lenght, loff_t* offset)
{
    struct Encoder_data *Encoder;
    ssize_t retval = -1;
    unsigned int converted_value = 0;
    ssize_t count = lenght;

    //Grab the Encoder_data struct out of the file struct.
    Encoder = filp->private_data;

    //Since the data we need is in userspace we need to copy it to kernel space so we can use it.
    retval = kstrtouint_from_user(buffer, count, 0, &converted_value);
    if(retval > 8192)
    {
        retval = 8192;
    }
    //Write to the I/O register
    iowrite32(converted_value, Encoder->base_address);
    return retval ? retval : count;
}

static int Encoder_open(struct inode* inode, struct file* filp)
{
    int status = -1;
    struct Encoder_data *Encoder;
    mutex_lock(&device_list_lock);

    //Find the address of the struct using the device_list and the device_entry member of the Encoder_data struct.
    list_for_each_entry(Encoder, &device_list, device_entry) {
        //Check if the struct is the correct one.
        if(Encoder->devt == inode->i_rdev) {
            //Store the struct in the private_data member of the file struct so that it is usable in the read and write functions of the device node.
            Encoder->message_read = 0;
            filp->private_data = Encoder;

            status = 0;
        }
    }

    //Try to lock the device, if it fails the device is already in use.
    if(!mutex_trylock(&Encoder_device_mutex))
    {
        printk(KERN_WARNING "Device is in use by another process\n");
        return -EBUSY;
    }

    mutex_unlock(&device_list_lock);

    return 0;
}

static int Encoder_release(struct inode* inode, struct file* filp)
{
    //Remove the mutex lock, so other processes can use the device.
    mutex_unlock(&Encoder_device_mutex);
    //	printk(KERN_INFO "Unlocking mutex\n");
    return 0;
}

struct file_operations Encoder_fops = {
    .owner =	THIS_MODULE,
    .read =		Encoder_read,
    .write = 	Encoder_write,
    .open =		Encoder_open,
    .release =	Encoder_release,
};

/*------------------------------------------------------------------------------------------------------*/
/*Sysfs endpoint definitions and handler functions.														*/
/*The sysfs nodes are used to transfer settings to the PID controller and read status information.		*/
/*E.g. the values for the P, I and D factors for the settings, and Emergency stop for the status.		*/
/*																										*/
/*																										*/
/*------------------------------------------------------------------------------------------------------*/

static ssize_t sys_set_node(struct device* dev, struct device_attribute* attr, const char* buffer, size_t lenght)
{
    struct Encoder_data *Encoder;
    int retval = -1;
    unsigned int converted_value = 0;
    unsigned int * address = 0;
    int count = lenght;

    //Find the address of the struct using the device_list and the device_entry member of the PID_data struct.
    list_for_each_entry(Encoder, &device_list, device_entry) {
        //Check if the struct is the correct one.
        if(Encoder->devt == dev->devt) {
            //Grab the address for the node that is being called.
            // HKMS: ik zou een array maken van filenamen en adressen, loop door de namen om te kijken welke file
            // HKMS: benaderd wordt en neem dan het bijpassende adres over. Makkelijker configureren en voorkomt
            // HKMS: code explosie bij nog meer files
            if(strcmp(attr->attr.name, "DIRECTION") == 0)
            {
                address = Encoder->base_address;
                printk(KERN_INFO "direction address%d\n", address);
                retval = kstrtoint(buffer, 0, &converted_value);
            }
            else if(strcmp(attr->attr.name, "POSITION") == 0)
            {
                address = Encoder->base_address;
                printk(KERN_INFO "Position address%d\n", address);
                retval = kstrtoint(buffer, 0, &converted_value);
                if(converted_value)
                {
                    converted_value = ioread32(address)|converted_value;
                }
                else
                {
                    converted_value = ioread32(address)&0xFFFE;
                }
            }
            else
            {
                printk(KERN_WARNING "Node does not exist\n");
                return -ENXIO;
            }
        }
    }

    iowrite32(converted_value, address);

    return retval ? retval : count;
}

static ssize_t sys_read_node(struct device* dev, struct device_attribute* attr, char *buffer)
{
    struct Encoder_data *Encoder;
    int retval = -1;
    int copied = 0;
    char int_array[20];
    unsigned int * address = 0;
    unsigned int fpga_value = 0;
    //Find the address of the struct using the device_list and the device_entry member of the PID_data struct.
    list_for_each_entry(Encoder, &device_list, device_entry) {
        //Check if the struct is the correct one.
        if(Encoder->devt == dev->devt) {
            //Grab the address for the node that is being called.
            if(strcmp(attr->attr.name, "POSITION") == 0)
            {
                address = Encoder->base_address;
                fpga_value = ioread32(address);
                fpga_value &= POS_MASK;
                printk(KERN_INFO "position called%d\n", fpga_value);
            }
            else if(strcmp(attr->attr.name, "DIRECTION") == 0)
            {
                address = Encoder->base_address;
                fpga_value = ioread32(address);
                fpga_value = (fpga_value  & DIR_MASK)>>31;
                printk(KERN_INFO "direction called%d\n", fpga_value);
            }
            else
            {
                printk(KERN_WARNING "Node does not exist\n");
                return -ENXIO;
            }
        }
    }



    copied = snprintf(int_array, 20, "%i", fpga_value);
    printk(KERN_INFO "copied variable%d\n", copied);
    printk(KERN_INFO "int array variable%s\n", &int_array);
    retval = copy_to_user(buffer, &int_array, copied);
    printk(KERN_INFO "retval variable%d\n", retval);
    printk(KERN_INFO "buffer variable%s\n", buffer);
    return retval ? retval : copied;
}

//Define the device attributes for the sysfs, and their handler functions.
static DEVICE_ATTR(POSITION, S_IRUSR | S_IWUSR, sys_read_node, sys_set_node);
static DEVICE_ATTR(DIRECTION, S_IRUSR | S_IWUSR, sys_read_node, sys_set_node);

static struct attribute *Encoder_attrs[] = {
    &dev_attr_POSITION.attr,
    &dev_attr_DIRECTION.attr,
    NULL,
};

static struct attribute_group Encoder_attr_group = {
    .attrs = Encoder_attrs,
};

static const struct attribute_group* Encoder_attr_groups[] = {
    &Encoder_attr_group,
    NULL,
};

/*------------------------------------------------------------------------------------------------------*/
/*Platform driver functions and struct																	*/
/*@Encoder_dt_ids[] struct to store the compatible device tree names										*/
/*@Encoder_probe called when a compatible device is found in the device tree. Creates device and maps iomem	*/
/*@Encoder_remove called when the driver is removed from the kernel, removes the device and unmaps iomem	*/
/*@Encoder_driver struct to define the platform driver, contains the compatible ID's and the function names */
/*------------------------------------------------------------------------------------------------------*/

static const struct of_device_id Encoder_dt_ids[] = {
    { .compatible = "xlnx,encoder-2.0"},
    {},
};

MODULE_DEVICE_TABLE(of, Encoder_dt_ids);

static int Encoder_probe(struct platform_device *pltform_Encoder)
{
    int minor = 0;
    int status = -1;
    struct resource res;
    int rc = 0;

    struct Encoder_data *Encoder;

    Encoder = kzalloc(sizeof(*Encoder), GFP_KERNEL);
    if(!Encoder)
        return -ENOMEM;

    INIT_LIST_HEAD(&Encoder->device_entry);

    mutex_lock(&device_list_lock);
    minor = find_first_zero_bit(minors, N_ENCODER_MINORS);
    if (minor < N_ENCODER_MINORS)
    {
        struct device *dev;

        Encoder->devt = MKDEV(Encoder_major, minor);
        dev = device_create_with_groups(Encoder_class, NULL, Encoder->devt, NULL, Encoder_attr_groups, CLASS_NAME "%d", minor);
        status = PTR_ERR_OR_ZERO(dev);
    }
    else
    {
        printk(KERN_DEBUG "No minor number available!\n");
        status = -ENODEV;
    }
    if( status == 0)
    {
        printk(KERN_INFO "New Encoder controller Encoder%d\n", minor);
        set_bit(minor, minors);
        list_add(&Encoder->device_entry, &device_list);
        //Retreive the base address and request the memory region.
        rc = of_address_to_resource(pltform_Encoder->dev.of_node, 0, &res);
        printk("resource rc: %d\n", &res);
        if( request_mem_region(res.start, resource_size(&res), CLASS_NAME) == NULL)
        {
            printk(KERN_WARNING "Unable to obtain physical I/O addresses\n");
            goto failed_memregion;
        }
        Encoder->base_register = res.start;
        //Remap the memory region in to usable memory

        Encoder->base_address = of_iomap(pltform_Encoder->dev.of_node, 0);

    }
    mutex_unlock(&device_list_lock);

    if(status)
        kfree(Encoder);
    else
        platform_set_drvdata(pltform_Encoder, Encoder);

    return status;

failed_memregion:
    device_destroy(Encoder_class, Encoder->devt);
    clear_bit(MINOR(Encoder->devt), minors);
    return -ENODEV;
}

static int Encoder_remove(struct platform_device *pltform_Encoder)
{
    struct Encoder_data *Encoder = platform_get_drvdata(pltform_Encoder);
    struct resource res;
    int rc = 0;

    rc = of_address_to_resource(pltform_Encoder->dev.of_node, 0, &res);

    mutex_lock(&device_list_lock);
    //Unmap the iomem
    iounmap(Encoder->base_address);
    //Delete the device from the list
    list_del(&Encoder->device_entry);
    //Destroy the device node
    device_destroy(Encoder_class, Encoder->devt);
    //Clear the minor bit
    clear_bit(MINOR(Encoder->devt), minors);
    if(Encoder->base_register)
    {
        release_mem_region(res.start, resource_size(&res));
        memory_request = 0;
    }
    //Free the kernel memory
    kfree(Encoder);
    mutex_unlock(&device_list_lock);

    return 0;
}

static struct platform_driver Encoder_driver = {
    .driver = {
        .name = "Encoder",
        .owner = THIS_MODULE,
        .of_match_table = of_match_ptr(Encoder_dt_ids),
    },
    .probe = Encoder_probe,
    .remove = Encoder_remove,
};

/*------------------------------------------------------------------------------------------------------*/
/*Module level functions																				*/
/*@Encoder_init initialisation function for the driver														*/
/*@Encoder_exit exit function for the driver																*/
/*------------------------------------------------------------------------------------------------------*/

static int Encoder_init(void)
{
    int retval;
    //Request a major device number from the kernel
    Encoder_major = register_chrdev(0, DEVICE_NAME, &Encoder_fops);
    //If the number is smaller than 0, it's an error. So jump to the error state.
    if(Encoder_major < 0)
    {
        printk(KERN_WARNING "Encoder: can't get major %d\n", Encoder_major);
        retval = Encoder_major;
        goto failed_chrdevreg;
    }
    //Now that we have a major number, create a device class.
    Encoder_class = class_create(THIS_MODULE, CLASS_NAME);
    //If there is an error, jump to the error state.
    if( IS_ERR(Encoder_class))
    {
        printk(KERN_NOTICE "Error while creating device class\n");
        retval =  PTR_ERR(Encoder_class);
        goto failed_classreg;
    }
    //Now that we have a class, we can create our device node.
    retval = platform_driver_register(&Encoder_driver);
    if(retval)
    {
        printk(KERN_NOTICE "Error while registering driver\n");
        goto failed_driverreg;
    }

    //Initialize the mutex lock.
    mutex_init(&Encoder_device_mutex);

    return 0;

failed_driverreg:
    class_destroy(Encoder_class);
failed_classreg:
    unregister_chrdev(Encoder_major, DEVICE_NAME);
failed_chrdevreg:
    return -1;
}

static void Encoder_exit(void)
{
    //When we remove the driver, destroy it's device(s), class and major number.
    //device_destroy(Encoder_class, MKDEV(Encoder_major, 0));
    platform_driver_unregister(&Encoder_driver);
    class_destroy(Encoder_class);
    unregister_chrdev(Encoder_major, DEVICE_NAME);
}

module_init(Encoder_init);
module_exit(Encoder_exit);

MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("Bas Janssen");
