/***************************************************************************
 *                                                                         *
 *      Author          : Ganesh Naik                                      *
 *      Organisation    : Levana Technologies, Mumbai, India.              *
 *      Website		: www.levanatech.com                               *
 *                                                                         *
 *  Conducting Workshops on - Embedded Linux & Device Drivers Training.    *
 *  -------------------------------------------------------------------    *
 *  Tel : 91-9869260324, Email : info@levanatech.com                       *
 *                                                                         *
 ***************************************************************************
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation. No warranty is attached; we cannot take *
 *   responsibility for errors or fitness for use.                         *
 ***************************************************************************/

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include "ioctl.h"

#define NAME "simple_char"
static int major;

ssize_t my_read(struct file *, char __user *, size_t, loff_t *);
ssize_t my_write(struct file *, const char __user *, size_t, loff_t *);
int my_open(struct inode *, struct file *);
int my_close(struct inode *, struct file *);
int my_ioctl(struct inode *, struct file *, unsigned int, unsigned long);


struct file_operations fops = {
	.owner = THIS_MODULE,
	.read = my_read,
	.write = my_write,
	.open = my_open,
	.release = my_close,
	.ioctl = my_ioctl
};

int char_init(void)
{
	major = register_chrdev(0, NAME, &fops);
	if(major < 0)
	{
		printk("Device could not be registered\n");
		return -EPERM;
	}
	printk("Driver registered with major %d\n", major);
	return 0;
}


void char_exit(void)
{
	unregister_chrdev(major, NAME);
	printk("chrdev unregistered\n");
}


ssize_t my_read(struct file *filp, char __user *usr, size_t size, loff_t *off)
{
	printk("Reading from device\n");
	return -EPERM;
}

ssize_t my_write(struct file *filp, const char __user *usr, size_t size, loff_t *off)
{
	printk("Trying to write into the device\n");
	return -EPERM;
}

int my_open(struct inode *ino, struct file *filp)
{
	try_module_get(THIS_MODULE);
	printk("device opened\n");
	return 0;
}

int my_close(struct inode *ino, struct file *filp)
{
	module_put(THIS_MODULE);
	printk("device closed\n");
	return 0;
}

int my_ioctl(struct inode *ino, struct file *filp, unsigned int cmd, unsigned long arg)
{
	switch(cmd) {
	
		case ONE:
				printk("One selected\n");
				break;
		case TWO:
				printk("Two selcted\n");
				break;
	}

	return 0;
}

module_init(char_init);
module_exit(char_exit);

MODULE_LICENSE("GPL");
