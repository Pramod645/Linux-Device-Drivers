 ***************************************************************************
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation. No warranty is attached; we cannot take *
 *   responsibility for errors or fitness for use.                         *
 ***************************************************************************/

#include <linux/kernel.h>
#include <linux/module.h>
#include "add.h"

int add(int a, int b)
{
	return a + b;
}
EXPORT_SYMBOL_GPL(add);

int init_module(void)
{
	printk("simple modules loaded\n");
	return 0;
}

void cleanup_module(void)
{
	printk("simple module unloaded\n");
}

MODULE_LICENSE("GPL");

