#include "main.h"
#include "stm32f0xx_hal.h"
// #include "cmsis_os.h"

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "modbus.h"
#include "registers.h"

// pin init helper
typedef enum {
    GpioModeInput,
    GpioModeOutput,
} GpioMode;

typedef struct {
    GPIO_TypeDef* port;
    uint32_t pin;
} GpioPin;

typedef struct {
    GpioPin gpio;
    GpioPin pull;
} IoLine;

void app_gpio_init(GpioPin gpio, GpioMode mode) {
    if(gpio.pin != 0) {
        GPIO_InitTypeDef GPIO_InitStruct;

        GPIO_InitStruct.Pin = gpio.pin;
        GPIO_InitStruct.Pull = GPIO_NOPULL;

        switch(mode) {
            case GpioModeInput:
                GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
            break;

            case GpioModeOutput: 
                GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
                GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
            break;
        }

        HAL_GPIO_Init(gpio.port, &GPIO_InitStruct);
    }
}

void app_gpio_write(GpioPin gpio, bool state) {
    if(gpio.pin != 0) {
        HAL_GPIO_WritePin(gpio.port, gpio.pin, state ? GPIO_PIN_SET : GPIO_PIN_RESET);
    }
}

bool app_gpio_read(GpioPin gpio) {
    return (HAL_GPIO_ReadPin(gpio.port, gpio.pin) == GPIO_PIN_SET);
}

void set_coil(uint8_t index, uint8_t state) {

}

uint8_t get_discrete(uint8_t index) {
    uint8_t res = 0;

    return res;
}

void app() {
    printf("=== RFID ASC B4CKSP4CE ===\n");

    modbus_init();

    while(1) {
        modbus_poll();
        HAL_GPIO_TogglePin(LED_0_GPIO_Port, LED_0_Pin);
        HAL_Delay(1);
    }
}

extern uint32_t test_var;

/*
void led_task(void const* argument) {
    while(1) {
        osDelay(400);
        HAL_GPIO_TogglePin(LED_0_GPIO_Port, LED_0_Pin);
    }
}
*/