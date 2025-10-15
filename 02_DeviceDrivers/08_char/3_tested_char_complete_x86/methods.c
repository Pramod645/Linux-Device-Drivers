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
#include <linux/sched.h>
#include <asm/uaccess.h>
#include "char.h"

static long unsigned int jiff;
#define MAX_LENGTH 1024
char char_device_buf[MAX_LENGTH];

int my_open(struct inode *ino, struct file *filp)
{
	try_module_get(THIS_MODULE);
	//jiff = jiffies;
	printk("file/device opened\n");
	return 0;
}

int my_close(struct inode *ino, struct file *filp)
{
//	printk("file open for %lu seconds\n", (jiffies - jiff)/HZ);
	printk("file/device closed\n");
	module_put(THIS_MODULE);
	return 0;
}

ssize_t my_read(struct file *file, char __user *buf, size_t lbuf, loff_t *ppos)
{
	int maxbytes; 
        int bytes_to_do; 
        int nbytes; 

        maxbytes = MAX_LENGTH - *ppos;

        if( maxbytes > lbuf ) bytes_to_do = lbuf;
        else bytes_to_do = maxbytes;
/*
        if( bytes_to_do == 0 ) {
                printk("Reached end of device\n");
                return -ENOSPC;   
        }
*/
        nbytes = bytes_to_do -
                 copy_to_user( buf, 
                               char_device_buf + *ppos, 
                               bytes_to_do ); 
        *ppos += nbytes;
        return nbytes;

}

ssize_t my_write(struct file *file, const char __user *buf, size_t lbuf, loff_t *ppos)
{
	int nbytes; 
        int bytes_to_do;
        int maxbytes; 

        maxbytes = MAX_LENGTH - *ppos;

        if( maxbytes > lbuf ) bytes_to_do = lbuf;
        else bytes_to_do = maxbytes;

        if( bytes_to_do == 0 ) {

                return -ENOSPC;
        }

        nbytes = bytes_to_do -
                 copy_from_user( char_device_buf + *ppos,
                                 buf, 
                                 bytes_to_do ); 
        *ppos += nbytes;
        return nbytes;

}
