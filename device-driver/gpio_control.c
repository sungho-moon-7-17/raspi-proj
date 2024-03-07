#include "gpio_control.h"

#include <linux/delay.h>

int __iomem * gpio_baseAddress;

int gc_init(void){
    gpio_baseAddress = (int *)ioremap(GPIO_PM_ADDR, 0xff);
    if (!gpio_baseAddress) {
        pr_err("ioremap failed\n");
        return -1;
    }   

    gc_setDirection(4, 1);
    gc_setOutput(4, 1);
    udelay(1);
    pr_info("%d", gc_stateGPIO(4));

    return 0;
}

void gc_exit(void){
    gc_setDirection(4, 0);
    gc_setOutput(4, 0);
    udelay(1);

    pr_info("%d", gc_stateGPIO(4));

    iounmap(gpio_baseAddress);
}


void gc_setDirection(unsigned int pin, unsigned int direction){
    int * targetAddr;
    int bitSet, mask;

    if ( pin > 57 && direction > 1){
        pr_err("gc_setDirctions function fail : invalid argument");
        return ;
    }

    targetAddr = gpio_baseAddress + GPSET + pin/10;

    mask = 0b111 << pin%10 * 3;
    bitSet = direction << pin%10 * 3;
    
    *targetAddr = (*targetAddr & ~mask) | bitSet;
}

void gc_setOutput(unsigned int pin, unsigned int output){
    int * targetAddr;
    int bitSet, offset;

    if ( pin > 57 && output > 1){
        pr_err("gc_setOutput function fail : invalid argument");
        return ;
    }

    offset = (output == 0)? GPCLR : GPSET;
    targetAddr = gpio_baseAddress + offset + pin/32;

    bitSet = 1 << pin%32;

    *targetAddr |= bitSet;
}

int gc_stateGPIO(unsigned int pin){
    int * targetAddr;
    int bitSet, result;

    if ( pin > 57){
        pr_err("gc_stateGPIO function fail : invalid argument");
        return -1;
    }

    targetAddr = gpio_baseAddress + GPLEV + pin/32;

    bitSet = 1 << pin%32;
    result = *targetAddr & bitSet;

    return result;
}