/*
 * CharDev.c
 *
 *  Created on: 15 дек. 2018 г.
 *      Author: bekir
 */

#include <linux/kernel.h>
#include <linux/cdev.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/types.h>
#include <linux/errno.h>
#include <linux/uaccess.h>
#include <linux/mm.h>
#include <linux/slab.h>
#include <asm/uaccess.h>
#include <linux/device.h>
#include <linux/kdev_t.h>
#include <asm/io.h>



MODULE_AUTHOR("Bekir");
MODULE_DESCRIPTION("Test module for linux kernel");
MODULE_LICENSE("GPL");

#define DEVICE_NAME "test_module"
#define KBUF_SIZE (size_t)(10*PAGE_SIZE)

static int is_device_open = 0;
static int  counter = 0;

static int count = 1;
static int major = 300;
static int minor = 0;

static dev_t first;
static struct cdev* char_dev;
static struct class* char_class;

static ssize_t read(struct file *, char __user *, size_t, loff_t *);
static ssize_t write(struct file *, const char __user *, size_t, loff_t *);
static int open(struct inode *, struct file *);
static int release(struct inode *, struct file *);

static struct file_operations fops =
 {
  .owner = THIS_MODULE,
  .read = read,
  .write =write,
  .open = open,
  .release = release
 };

typedef struct _CharData
{
	char* buf_ptr;
	int data;
}CharData;

CharData data_drv={NULL, 0};

static int probe_module (void)
{
	int status;

	first = MKDEV(major, minor);
	status = register_chrdev_region(first, count, DEVICE_NAME);

	char_class = class_create(THIS_MODULE, "mychardrv");
	device_create(char_class, NULL, first, "%s", "mychardev");

	 	 if (status < 0 )
		 {
		  printk(KERN_INFO "Registering the character device failed with %d\n", major);
		  return -EINVAL;

		 }

	char_dev=cdev_alloc();
	cdev_init(char_dev, &fops);
	cdev_add(char_dev, first, count);

	printk(KERN_INFO "Registering the character device with major number %d\n", major);
	return 0;
}

static int __init module_start(void)
{
	if (!probe_module()) return 0;
	//probe_module();
	return 0;
}


static void __exit module_stop(void)
{
	device_destroy(char_class, first);
	class_destroy(char_class);

	cdev_del(char_dev);
	unregister_chrdev( major, DEVICE_NAME );
	unregister_chrdev_region(first, count);

	printk( KERN_INFO "%s - Test module is unloaded!\n", __FUNCTION__ );
	return;
}

static ssize_t read(struct file *f, char __user *ptr, size_t size, loff_t *pos)
{
	char* kbuf = ((CharData*)(f->private_data))->buf_ptr;
	int nbytes;

	sprintf(kbuf + *pos, "%d", data_drv.data);

	nbytes=size;

	if (*pos>=KBUF_SIZE)
	{
		return -ENOMEM;
	}
	if ((*pos + size) >= KBUF_SIZE)
	{
		nbytes = KBUF_SIZE - *pos;
	}

	copy_to_user(ptr, kbuf + *pos, nbytes);

	printk(KERN_INFO "%s - Read %d \n\r", __FUNCTION__, nbytes);
	*pos += nbytes;

	return nbytes;


}

static ssize_t write(struct file *f, const char __user *ptr, size_t size, loff_t *pos)
{
	char* kbuf = ((CharData*)(f->private_data))->buf_ptr;
	int nbytes;

	long data;

	nbytes=size;

	if (*pos>=KBUF_SIZE)
	{
		return -ENOMEM;
	}
	if ((*pos + size) >= KBUF_SIZE)
	{
		nbytes = KBUF_SIZE - *pos;
	}

	printk(KERN_INFO "Write %d \n\r", nbytes);

	copy_from_user(kbuf + *pos, ptr, nbytes);
	kstrtol(kbuf + *pos, 10, &data);
	data_drv.data = data;
	*pos += nbytes;

	return nbytes;
}

static int open(struct inode *node, struct file *f)
{
	is_device_open = 1;
	char* kbuf;

	kbuf = kmalloc(KBUF_SIZE, GFP_KERNEL);
	memset(kbuf, '\0', KBUF_SIZE);
	f->private_data = &data_drv;
	data_drv.buf_ptr = kbuf;
	counter++;

	printk(KERN_INFO "%s - Devices count %d \n\r", __FUNCTION__, counter);

	return 0;
}

static int release(struct inode *node, struct file *f)
{
	char* kbuf = ((CharData*)(f->private_data))->buf_ptr;

		if (kbuf) kfree(kbuf);

	kbuf=NULL;
	((CharData*)(f->private_data))->buf_ptr = NULL;

		if (counter>0) counter--;

	printk(KERN_INFO "%s - Devices count %d \n\r", __FUNCTION__, counter);
	return 0;
}

module_init(module_start);
module_exit(module_stop);

