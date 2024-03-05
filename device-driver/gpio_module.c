#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/cdev.h>

#define DRIVER_NAME         "gpio_driver"
#define DRIVER_CLASS_NAME   "gpio_module_class"

static struct cdev gpio_cdev;
static dev_t gpio_dev_t;
static struct class * gpio_class;
static struct device * gpio_device;

static ssize_t gpio_module_read(struct file * file, char __user * buf, size_t length, loff_t * offset);
static ssize_t gpio_module_write(struct file * file, const char __user * buf, size_t length, loff_t * offset);

static struct file_operations gpio_fops = {
    .owner = THIS_MODULE,
    .read = gpio_module_read,
    .write = gpio_module_write,
};

static ssize_t gpio_module_read(struct file * filp, char __user * buf, size_t length, loff_t * offset){
    return 0;
}

static ssize_t gpio_module_write(struct file * filp, const char __user * buf, size_t length, loff_t * offset){
    return 0;
}


static int __init gpio_module_init(void){
    int errCode = 0;

    // 디바이스 번호 할당
    errCode = alloc_chrdev_region(&gpio_dev_t, 0, 1, DRIVER_NAME);
    if (errCode < 0){
        pr_info("Device Nr. could not be allocated!\n");
        goto ERROR1;
    }

    // device class 생성 (device_create에 사용됨)
    gpio_class = class_create(THIS_MODULE, DRIVER_CLASS_NAME);
    if (gpio_class == NULL){
        pr_info("fail create device class\n");
        errCode = -1;
        goto ERROR2;
    }

    // 장치 생성 (sysfs에 등록)
    gpio_device = device_create(gpio_class, NULL, gpio_dev_t, NULL, DRIVER_CLASS_NAME);
    if (gpio_device == NULL){
        pr_info("fail create device file");
        errCode = -1;
        goto ERROR3;
    }

    cdev_init(&gpio_cdev, &gpio_fops);

    errCode = cdev_add(&gpio_cdev, gpio_dev_t, 1);
    if (errCode < 0){
        pr_info("Registering of device to kernel failed!\n");
        goto ERROR4;
    }

    pr_info("gpio kernel module start");
    return 0;

ERROR4:
    device_destroy(gpio_class, gpio_dev_t);
ERROR3:
    class_destroy(gpio_class);
ERROR2:
    unregister_chrdev_region(gpio_dev_t, 1);
ERROR1:
    return errCode;
}

static void __exit gpio_module_exit(void){
    cdev_del(&gpio_cdev);
    device_destroy(gpio_class, gpio_dev_t);
    class_destroy(gpio_class);
    unregister_chrdev_region(gpio_dev_t, 1);
    pr_info("gpio kernel module end\n");
}

module_init(gpio_module_init);
module_exit(gpio_module_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("sungho <sungho8203@gmail.com>");
MODULE_DESCRIPTION("gpio control");
MODULE_VERSION("1.0.0");