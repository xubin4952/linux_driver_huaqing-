#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>

#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/kfifo.h>

#include <linux/ioctl.h>
#include <linux/uaccess.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>

#include "vser.h"

#define VSER_MAJOR	256
#define VSER_MINOR	0
#define VSER_DEV_CNT	1
#define VSER_DEV_NAME	"vser"

struct vser_dev {
	unsigned int baud;
	struct option opt;
	struct cdev cdev;
	struct proc_dir_entry *pdir;
	struct proc_dir_entry *pdat;
};

DEFINE_KFIFO(vsfifo, char, 32);
static struct vser_dev vsdev;

static int vser_open(struct inode *inode, struct file *filp)
{
	return 0;
}

static int vser_release(struct inode *inode, struct file *filp)
{
	return 0;
}

static ssize_t vser_read(struct file *filp, char __user *buf, size_t count, loff_t *pos)
{
	int ret;
	unsigned int copied = 0;

	ret = kfifo_to_user(&vsfifo, buf, count, &copied);

	return ret == 0 ? copied : ret;
}

static ssize_t vser_write(struct file *filp, const char __user *buf, size_t count, loff_t *pos)
{
	int ret;
	unsigned int copied = 0;

	ret = kfifo_from_user(&vsfifo, buf, count, &copied);

	return ret == 0 ? copied : ret;
}

static long vser_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
	if (_IOC_TYPE(cmd) != VS_MAGIC)
		return -ENOTTY;

	switch (cmd) {
	case VS_SET_BAUD:
		vsdev.baud = arg;
		break;
	case VS_GET_BAUD:
		arg = vsdev.baud;
		break;
	case VS_SET_FFMT:
		if (copy_from_user(&vsdev.opt, (struct option __user *)arg, sizeof(struct option)))
			return -EFAULT;
		break;
	case VS_GET_FFMT:
		if (copy_to_user((struct option __user *)arg, &vsdev.opt, sizeof(struct option)))
			return -EFAULT;
		break;
	default:
		return -ENOTTY;
	}

	return 0;
}

static struct file_operations vser_ops = {
	.owner = THIS_MODULE,
	.open = vser_open,
	.release = vser_release,
	.read = vser_read,
	.write = vser_write,
	.unlocked_ioctl = vser_ioctl,
};

static int dat_show(struct seq_file *m, void *v)
{
	struct vser_dev *dev = m->private;

	seq_printf(m, "baudrate: %d\n", dev->baud);
	seq_printf(m, "frame format: %d%c%d\n", dev->opt.datab, \
			dev->opt.parity == 0 ? 'N' : dev->opt.parity == 1 ? 'O': 'E', \
			dev->opt.stopb);
	return 0;
}

static int proc_open(struct inode *inode, struct file *file)
{
	return single_open(file, dat_show, PDE_DATA(inode));
}

static struct file_operations proc_ops = {
	.owner = THIS_MODULE,
	.open = proc_open,
	.release = single_release,
	.read = seq_read,
	.llseek = seq_lseek,
};

static int __init vser_init(void)
{
	int ret;
	dev_t dev;

	dev = MKDEV(VSER_MAJOR, VSER_MINOR);
	ret = register_chrdev_region(dev, VSER_DEV_CNT, VSER_DEV_NAME);
	if (ret)
		goto reg_err;

	cdev_init(&vsdev.cdev, &vser_ops);
	vsdev.cdev.owner = THIS_MODULE;
	vsdev.baud = 115200;
	vsdev.opt.datab = 8;
	vsdev.opt.parity = 0;
	vsdev.opt.stopb = 1;

	ret = cdev_add(&vsdev.cdev, dev, VSER_DEV_CNT);
	if (ret)
		goto add_err;

	vsdev.pdir = proc_mkdir("vser", NULL);
	if (!vsdev.pdir)
		goto dir_err;
	vsdev.pdat = proc_create_data("info", 0, vsdev.pdir, &proc_ops, &vsdev);
	if (!vsdev.pdat)
		goto dat_err;

	return 0;

dat_err:
	remove_proc_entry("vser", NULL);
dir_err:
	cdev_del(&vsdev.cdev);
add_err:
	unregister_chrdev_region(dev, VSER_DEV_CNT);
reg_err:
	return ret;
}

static void __exit vser_exit(void)
{
	
	dev_t dev;

	dev = MKDEV(VSER_MAJOR, VSER_MINOR);

	remove_proc_entry("info", vsdev.pdir);
	remove_proc_entry("vser", NULL);

	cdev_del(&vsdev.cdev);
	unregister_chrdev_region(dev, VSER_DEV_CNT);
}

module_init(vser_init);
module_exit(vser_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Kevin Jiang <jiangxg@farsight.com.cn>");
MODULE_DESCRIPTION("A simple character device driver");
MODULE_ALIAS("virtual-serial");
