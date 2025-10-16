 ***************************************************************************
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation. No warranty is attached; we cannot take *
 *   responsibility for errors or fitness for use.                         *
 ***************************************************************************/

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/interrupt.h>


struct tasklet_struct task;
int id;

void my_func(unsigned long);
void my_func(unsigned long data)
{
	printk("tasklet is scheduled\n");
}

irqreturn_t handler(int irq, void *id)
{

	//printk("inside the handler\n");
	tasklet_schedule(&task);
	return 0;
}

int init_module(void)
{
	task.func=&my_func;
	
	printk("simple modules loaded\n");
	request_irq(10, handler, IRQF_SHARED, "levana", &id); 
	return 0;
}

void cleanup_module(void)
{
	printk("simple module unloaded\n");
	free_irq(10, &id);   
}

MODULE_LICENSE("GPL");

