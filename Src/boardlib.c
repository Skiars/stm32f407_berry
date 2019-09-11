#include "berry.h"
#include "stm32f4xx_hal.h"
#include "gpio.h"
#include <string.h>

int m_pin_mode(bvm *vm)
{
    int argc = be_top(vm);
    if (argc >= 2) {
        int pin = be_toint(vm, 1);
        int mode = be_toint(vm, 2);
        gpio_pin_mode(pin, mode, 0);
    }
    be_return_nil(vm);
}

int m_pin_write(bvm *vm)
{
    int argc = be_top(vm);
    if (argc >= 2) {
        int pin = be_toint(vm, 1);
        int value = be_toint(vm, 2);
        gpio_pin_write(pin, value);
    }
    be_return_nil(vm);
}

int m_pin_read(bvm *vm)
{
    int argc = be_top(vm);
    if (argc >= 1) {
        int pin = be_toint(vm, 1);
        be_pushint(vm, gpio_pin_read(pin));
        be_return(vm);
    }
    be_return_nil(vm);
}

static int m_delay(bvm *vm)
{
    if (be_isnumber(vm, 1)) {
        int xms = be_toint(vm, 1);
        HAL_Delay(xms);
    }
    be_return_nil(vm);
}

static int m_reboot(bvm *vm)
{
    be_writestring("software reboot...\n\n");
    HAL_Delay(10);
    NVIC_SystemReset();
    be_return_nil(vm);
}

/* @const_object_info_begin
module board (scope: global) {
    pin_mode, func(m_pin_mode)
    pin_write, func(m_pin_write)
    pin_read, func(m_pin_read)
    delay, func(m_delay)
    reboot, func(m_reboot)
}
@const_object_info_end */
#include "../generate/be_fixed_board.h"
