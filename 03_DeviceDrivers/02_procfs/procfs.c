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

#ifndef __KERNEL__
	#define __KERNEL__
#endif

#ifndef MODULE
	#define MODULE
#endif

#define EXPORT_SYMTAB
#define SUCCESS 0

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/proc_fs.h>
#include <linux/slab.h>
#include <asm/uaccess.h>


static int my_read(char *page, char **start, off_t off,
		   int count, int *eof, void *data) 
{
	return sprintf(page+off, "Hi. my_read. Jiffies %u\n", jiffies);
}


static int my_write(struct file *file, const char *buffer, 
		   unsigned long count, void *data)
{
	ulong retval;
	char *str = (char *)kmalloc( count * sizeof(char), GFP_KERNEL );
	retval = copy_from_user(str, buffer, count);
	printk("Got String: %s\n", str);
	kfree(str);
	return count-retval;
}

static struct proc_dir_entry *my_proc_entry = 0;
		   

static __init int init_proc(void)
{
	my_proc_entry = create_proc_entry("levana_proc", 0, 0);
	if( !my_proc_entry ) return -EINVAL;
	my_proc_entry->read_proc = my_read;
	my_proc_entry->write_proc = my_write;
	return SUCCESS;
}


static __exit void cleanup_proc(void)
{
	if( my_proc_entry )
		remove_proc_entry("levana_proc", 0);
}

module_init(init_proc);
module_exit(cleanup_proc);

MODULE_AUTHOR("Ganesh Naik");
MODULE_DESCRIPTION("Example");
MODULE_LICENSE("GPL");

