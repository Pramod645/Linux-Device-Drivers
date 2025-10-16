 ***************************************************************************
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation. No warranty is attached; we cannot take *
 *   responsibility for errors or fitness for use.                         *
 ***************************************************************************/

#include <linux/module.h>
#include <linux/sched.h>
#include <linux/jiffies.h>
#include <linux/kthread.h>


//static struct task_struct *sender_thread = NULL;

static int my_sender_thread (void *data)
{

	while(1)
	{
	
		printk("Hi I am kernel thread!\n");

		msleep(100);
		if (kthread_should_stop())
		break;

	}
    	return 0;

}             

static int __init my_init (void)
{
    sender_thread = kthread_run (my_sender_thread,
                                 NULL, "kernel_thread");
    if (IS_ERR (sender_thread)) {
        printk ("Error %ld creating thread\n",
                PTR_ERR (sender_thread));
        return -1;
    }

    printk (KERN_INFO "Starting Kernel thread in init module\n");

    return 0;
}

static void __exit my_exit (void)
{
	printk (KERN_INFO "Exiting kernel_thread module\n");
	kthread_stop(sender_thread);
}

module_exit (my_exit);
module_init (my_init);

MODULE_AUTHOR ("Pramod Kumar);
MODULE_DESCRIPTION ("Embedded Linux Device Drivers Excercise");
MODULE_LICENSE ("GPL v2");
