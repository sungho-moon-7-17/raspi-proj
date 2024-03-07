#ifndef GPIO_CONTROL_H
#define GPIO_CONTROL_H

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/cdev.h>

#include <linux/io.h>

#define GPIO_PM_ADDR    0xfe200000

#define GPFSEL  0x00                // GPIO functions select
#define GPSET   0x1c/sizeof(int)    // GPIO pin output set
#define GPCLR   0x28/sizeof(int)    // GPIO pin output clear
#define GPLEV   0x34/sizeof(int)    // GPIO pin level

#define DIR_IN 0
#define DIR_OUT 1

extern int gc_init(void);
extern void gc_exit(void);

extern void gc_setDirection(unsigned int pin, unsigned int direction);
extern void gc_setOutput(unsigned int pin, unsigned int output);
extern int gc_stateGPIO(unsigned int pin);

#endif