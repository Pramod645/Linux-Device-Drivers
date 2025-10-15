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

/*
 * Module parameters
 *
 * Load it and unload it.  While the module is loaded, look at its
 * directory in /sys/module, and see if you can change the value of
 * the parameter you established.
 */

#include <linux/module.h>
#include <linux/init.h>

static int mod_param = 12;
module_param (mod_param, int, S_IRUGO | S_IWUSR);

static int __init my_init (void)
{
    printk (KERN_INFO "Hello world from modfun \n");
    printk (KERN_INFO " mod_param = %d\n", mod_param);
    return 0;
}

static void __exit my_exit (void)
{
    printk (KERN_INFO "Goodbye world from modfun \n");
}

module_init (my_init);
module_exit (my_exit);

MODULE_AUTHOR ("Pramod Kumar);
MODULE_DESCRIPTION ("Embedded Linux Device Drivers Excercise");
MODULE_LICENSE ("GPL v2");
