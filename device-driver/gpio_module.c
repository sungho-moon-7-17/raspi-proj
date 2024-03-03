#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>

static int __init gpio_module_init(void){
    pr_info("gpio kernel module start\n");
    return 0;
}

static void __exit gpio_module_exit(void){
    pr_info("gpio kernel module end\n");
}

module_init(gpio_module_init);
module_exit(gpio_module_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("sungho <sungho8203@gmail.com>");
MODULE_DESCRIPTION("gpio control");
MODULE_VERSION("1.0.0");