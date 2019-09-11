#include "gpio.h"
#include "stm32f4xx_hal.h"
#include <string.h>

static int _gpio_mode(int mode)
{
    switch (mode) {
    case 1: return GPIO_MODE_OUTPUT_PP;
    case 2: return GPIO_MODE_OUTPUT_OD;
    case 0: default: return GPIO_MODE_INPUT;
    }
}

static int _gpio_pull(int pull)
{
    switch (pull) {
    case 1: return 0x00000001U;
    case 2: return 0x00000002U;
    case 0: default: return 0x00000000U;
    }
}

void gpio_pin_mode(int pin, int mode, int pull)
{
    GPIO_InitTypeDef GPIO_InitStruct = { 0 };

    /*Configure GPIO pin : LED_USER_Pin */
    GPIO_InitStruct.Pin = 1 << (pin < 16 ? pin > 0 ? pin : 0 : 15);
    GPIO_InitStruct.Mode = _gpio_mode(mode);
    GPIO_InitStruct.Pull = _gpio_pull(pull);
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

void gpio_pin_write(int pin, int value)
{
    /* Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(GPIOA, pin, value ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

int gpio_pin_read(int pin)
{
    return HAL_GPIO_ReadPin(GPIOA, pin) != GPIO_PIN_RESET;
}
