#include <linux/build-salt.h>
#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

BUILD_SALT;

MODULE_INFO(vermagic, VERMAGIC_STRING);
MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__section(.gnu.linkonce.this_module) = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

#ifdef CONFIG_RETPOLINE
MODULE_INFO(retpoline, "Y");
#endif

static const struct modversion_info ____versions[]
__used __section(__versions) = {
	{ 0xdd8f8694, "module_layout" },
	{ 0x37669270, "single_release" },
	{ 0x40970142, "seq_read" },
	{ 0x1be63b22, "seq_lseek" },
	{ 0x6091b333, "unregister_chrdev_region" },
	{ 0x22b90774, "cdev_del" },
	{ 0x9cfebac4, "remove_proc_entry" },
	{ 0x22bb8281, "proc_create_data" },
	{ 0x75c6269a, "proc_mkdir" },
	{ 0xc4952f09, "cdev_add" },
	{ 0x2064fa56, "cdev_init" },
	{ 0x3fd78f3b, "register_chrdev_region" },
	{ 0x362ef408, "_copy_from_user" },
	{ 0xb44ad4b3, "_copy_to_user" },
	{ 0x4578f528, "__kfifo_to_user" },
	{ 0xdecd0b29, "__stack_chk_fail" },
	{ 0x30a80826, "__kfifo_from_user" },
	{ 0x375cb97e, "seq_printf" },
	{ 0x1d8f45ba, "single_open" },
	{ 0xba2a97aa, "PDE_DATA" },
	{ 0xbdfb6dbb, "__fentry__" },
};

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "A9D0829F8C98B29DD5DB98B");
