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
#include "char.h"

int major;
struct file_operations fops = {
	.owner = THIS_MODULE,
	.open = my_open,
	.release = my_close,
	.read = my_read,
	.write = my_write
};

int char_init(void)
{
	major = register_chrdev(0, "simple_char", &fops);
	if(major < 0)
		return -EPERM;
	printk("registered with major: %d\n", major);
	return 0;
}

void char_exit(void)
{
	unregister_chrdev(major, "simple_char");
	printk("unregistered major: %d\n", major);
}

module_init(char_init);
module_exit(char_exit);

MODULE_LICENSE("GPL");
