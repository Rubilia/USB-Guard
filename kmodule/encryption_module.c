#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("ILIA RUBASHKIN");
MODULE_DESCRIPTION("USB Encryption Layer Module");
MODULE_VERSION("0.1");

static int __init encryption_init(void) {
    printk(KERN_INFO "USB Encryption Layer: Module loaded.\n");
    return 0;
}

static void __exit encryption_exit(void) {
    printk(KERN_INFO "USB Encryption Layer: Module unloaded.\n");
}

module_init(encryption_init);
module_exit(encryption_exit);
