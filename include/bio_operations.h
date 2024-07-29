#ifndef __BIO_OPERATIONS_H__
#define __BIO_OPERATIONS_H__

#include <linux/bio.h>
#include <linux/blkdev.h>

int usb_bio_read(struct block_device *bdev, sector_t sector, void *buffer);
int usb_bio_write(struct block_device *bdev, sector_t sector, void *buffer);

#endif
