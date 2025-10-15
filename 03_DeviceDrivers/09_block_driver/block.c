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

#include <linux/fs.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/init.h>
#include <linux/vmalloc.h>
#include <linux/blkdev.h>
#include <linux/genhd.h>
#include <linux/errno.h>
#include <linux/hdreg.h>
#include <linux/version.h>

#define MY_DEVICE_NAME "simple_block"

static int mybdrv_ma_no = 0, diskmb = 128;
static int disk_size = 0;
static char *my_dev;
static struct gendisk *my_gd;
static spinlock_t lock;
static unsigned short sector_size = 512;

module_param_named (size, diskmb, int, 0);

static struct request_queue *my_request_queue;

#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,31)
static void my_request (struct request_queue *q)
{
    struct request *rq;
    int size;
    char *ptr;
    unsigned nr_sectors, sector;

    printk (KERN_INFO "entering request routine\n");

    while ((rq = elv_next_request (q))) {
        if (!blk_fs_request (rq)) {
            printk (KERN_WARNING "This was not a normal fs request, skipping\n");
            end_request (rq, 0);
            continue;
        }

        nr_sectors = rq->current_nr_sectors;
        sector = rq->sector;
        ptr = my_dev + sector * sector_size;
        size = nr_sectors * sector_size;

        if ((ptr + size) > (my_dev + disk_size)) {
            printk (KERN_WARNING " tried to go past end of device\n");
            end_request (rq, 0);
            continue;
        }

        if (rq_data_dir (rq)) {
            printk (KERN_INFO "writing at sector %d, %ud sectors \n",
                    sector, nr_sectors);
            memcpy (ptr, rq->buffer, size);
        } else {
            printk (KERN_INFO "reading at sector %d, %ud sectors \n",
                    sector, nr_sectors);
            memcpy (rq->buffer, ptr, size);
        }

        end_request (rq, 1);
    }
    printk (KERN_INFO "leaving request\n");
}
#else
static void my_request (struct request_queue *q)
{
    struct request *rq;
    int size, res=0;
    char *ptr;
    unsigned nr_sectors, sector;
    printk (KERN_INFO "entering request routine\n");

    rq = blk_fetch_request(q);
    while (rq){
        if (!blk_fs_request (rq)) {
            printk (KERN_WARNING "This was not a normal fs request, skipping\n");
            goto done;
        }
        nr_sectors = blk_rq_cur_sectors(rq);
        sector = blk_rq_pos(rq);
        
        ptr = my_dev + sector * sector_size;
        size = nr_sectors * sector_size;
        
        if ((ptr + size) > (my_dev + disk_size)) {
            printk (KERN_WARNING " tried to go past end of device\n");
            goto done;
        }

        if (rq_data_dir (rq)) {
            printk (KERN_INFO "writing at sector %d, %ud sectors \n",
                    sector, nr_sectors);
            memcpy (ptr, rq->buffer, size);
        } else {
            printk (KERN_INFO "reading at sector %d, %ud sectors \n",
                    sector, nr_sectors);
            memcpy (rq->buffer, ptr, size);
        }
    done:
        if (!__blk_end_request_cur(rq, res))
            rq = blk_fetch_request(q);
    }
    printk (KERN_INFO "leaving request\n");
}
#endif

#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,28)
static int my_ioctl (struct inode *i, struct file *f,
                     unsigned int cmd, unsigned long arg)
#else
static int my_ioctl (struct block_device *bdev, fmode_t mode,
                     unsigned int cmd, unsigned long arg)
#endif
{
    long size;
    struct hd_geometry geo;

    printk (KERN_INFO "cmd=%d\n", cmd);

    switch (cmd) {
    case HDIO_GETGEO:
        printk (KERN_INFO "HIT HDIO_GETGEO\n");
        /*
         * get geometry: we have to fake one...  trim the size to a
         * multiple of 64 (32k): tell we have 16 sectors, 4 heads,
         * whatever cylinders. Tell also that data starts at sector. 4.
         */

        size = disk_size;
        size &= ~0x3f;          /* multiple of 64 */
        geo.cylinders = size >> 6;
        geo.heads = 4;
        geo.sectors = 16;
        geo.start = 4;

        if (copy_to_user ((void __user *)arg, &geo, sizeof (geo)))
            return -EFAULT;

        return 0;
    }
    printk (KERN_WARNING "return -ENOTTY\n");

    return -ENOTTY;             /* unknown command */
}

static struct block_device_operations mybdrv_fops = {
    .owner = THIS_MODULE,
    .ioctl = my_ioctl,
};

static int __init my_init (void)
{
    //disk_size = diskmb * 1024 * 1024;
    disk_size = diskmb * 512 * 512;
    spin_lock_init (&lock);

    if (!(my_dev = vmalloc (disk_size)))
        return -ENOMEM;

    if (!(my_request_queue = blk_init_queue (my_request, &lock))) {
        vfree (my_dev);
        return -ENOMEM;
    }

#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,31)
    blk_queue_hardsect_size (my_request_queue, sector_size);
#else
    blk_queue_logical_block_size (my_request_queue, sector_size);
#endif

    mybdrv_ma_no = register_blkdev (mybdrv_ma_no, MY_DEVICE_NAME);
    if (mybdrv_ma_no < 0) {
        printk (KERN_ERR "Failed registering mybdrv, returned %d\n",
                mybdrv_ma_no);
        vfree (my_dev);
        return mybdrv_ma_no;
    }

    if (!(my_gd = alloc_disk (16))) {
        unregister_blkdev (mybdrv_ma_no, MY_DEVICE_NAME);
        vfree (my_dev);
        return -ENOMEM;
    }

    my_gd->major = mybdrv_ma_no;
    my_gd->first_minor = 0;
    my_gd->fops = &mybdrv_fops;
    strcpy (my_gd->disk_name, MY_DEVICE_NAME);
    my_gd->queue = my_request_queue;
    set_capacity (my_gd, disk_size / sector_size);
    add_disk (my_gd);

    printk (KERN_INFO "device successfully   registered, Major No. = %d\n",
            mybdrv_ma_no);
    printk (KERN_INFO "Capacity of ram disk is: %d MB\n", diskmb);

    return 0;
}

static void __exit my_exit (void)
{
    del_gendisk (my_gd);
    put_disk (my_gd);
    unregister_blkdev (mybdrv_ma_no, MY_DEVICE_NAME);
    printk (KERN_INFO "module successfully unloaded, Major No. = %d\n",
            mybdrv_ma_no);
    blk_cleanup_queue (my_request_queue);
    vfree (my_dev);
}

module_init (my_init);
module_exit (my_exit);

MODULE_AUTHOR ("Ganesh Naik");
MODULE_DESCRIPTION ("Embedded Linux Workshop, Levana Technologies, Mumbai");
MODULE_LICENSE ("GPL v2");
