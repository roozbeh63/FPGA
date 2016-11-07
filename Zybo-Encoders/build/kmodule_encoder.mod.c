#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);

__visible struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0x951ca67e, __VMLINUX_SYMBOL_STR(module_layout) },
	{ 0x96114fff, __VMLINUX_SYMBOL_STR(platform_driver_unregister) },
	{ 0xee57ea5d, __VMLINUX_SYMBOL_STR(kmalloc_caches) },
	{ 0x3d47ec15, __VMLINUX_SYMBOL_STR(__platform_driver_register) },
	{ 0x17506ac5, __VMLINUX_SYMBOL_STR(kmem_cache_alloc) },
	{ 0x8435e048, __VMLINUX_SYMBOL_STR(__class_create) },
	{ 0x29537c9e, __VMLINUX_SYMBOL_STR(alloc_chrdev_region) },
	{ 0x788fe103, __VMLINUX_SYMBOL_STR(iomem_resource) },
	{ 0x3a2ac1c1, __VMLINUX_SYMBOL_STR(device_create) },
	{ 0xad67ce6d, __VMLINUX_SYMBOL_STR(cdev_add) },
	{ 0x2f5c1a6, __VMLINUX_SYMBOL_STR(cdev_init) },
	{ 0xe7c98531, __VMLINUX_SYMBOL_STR(of_get_property) },
	{ 0xa5d04b6, __VMLINUX_SYMBOL_STR(of_iomap) },
	{ 0xadf42bd5, __VMLINUX_SYMBOL_STR(__request_region) },
	{ 0x87a7c782, __VMLINUX_SYMBOL_STR(of_address_to_resource) },
	{ 0xdda2763d, __VMLINUX_SYMBOL_STR(of_match_device) },
	{ 0xefd6cf06, __VMLINUX_SYMBOL_STR(__aeabi_unwind_cpp_pr0) },
	{ 0x7485e15e, __VMLINUX_SYMBOL_STR(unregister_chrdev_region) },
	{ 0xdd6a611a, __VMLINUX_SYMBOL_STR(class_destroy) },
	{ 0x37a0cba, __VMLINUX_SYMBOL_STR(kfree) },
	{ 0xaaa1ccbf, __VMLINUX_SYMBOL_STR(cdev_del) },
	{ 0x151a40d1, __VMLINUX_SYMBOL_STR(device_destroy) },
	{ 0x27e1a049, __VMLINUX_SYMBOL_STR(printk) },
	{ 0x6b916f02, __VMLINUX_SYMBOL_STR(try_module_get) },
	{ 0x2e5810c6, __VMLINUX_SYMBOL_STR(__aeabi_unwind_cpp_pr1) },
	{ 0xcfea7ca5, __VMLINUX_SYMBOL_STR(module_put) },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";

MODULE_ALIAS("of:N*T*Cxlnx,rotary-encoder-1.0*");
