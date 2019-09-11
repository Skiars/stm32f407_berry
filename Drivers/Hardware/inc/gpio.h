#ifndef __GPIO_H
#define __GPIO_H

void gpio_pin_mode(int pin, int mode, int pull);
void gpio_pin_write(int pin, int value);
int gpio_pin_read(int pin);

#endif
