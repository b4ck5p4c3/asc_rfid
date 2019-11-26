#include "main.h"
#include "stm32f0xx_hal.h"
// #include "cmsis_os.h"

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "modbus.h"
#include "registers.h"

#include "pn532_stm32f1.h"

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

void modbus_poll_enable();

void app() {
    uint8_t buff[255];
    uint8_t uid[MIFARE_UID_MAX_LENGTH];

    PN532 pn532;
    PN532_I2C_Init(&pn532);

    printf("=== RFID ASC B4CKSP4CE ===\n");

    HAL_GPIO_WritePin(LED_0_GPIO_Port, LED_0_Pin, GPIO_PIN_SET);

    bool nfc_ready = false;

    if (PN532_GetFirmwareVersion(&pn532, buff) == PN532_STATUS_OK) {
        nfc_ready = true;
        HAL_GPIO_WritePin(LED_0_GPIO_Port, LED_0_Pin, GPIO_PIN_RESET);
    } else {
        HAL_GPIO_WritePin(LED_0_GPIO_Port, LED_0_Pin, GPIO_PIN_SET);
    }

    if(nfc_ready) {
        PN532_SamConfiguration(&pn532);
    }

    modbus_init();
    modbus_poll_enable();

    while(1) {
        if(nfc_ready) {
            int32_t uid_len = PN532_ReadPassiveTarget(&pn532, uid, PN532_MIFARE_ISO14443A, 1000);
            if (uid_len == PN532_STATUS_ERROR) {
            } else {
                for(size_t i = 0; i < uid_len; i++) {
                    registers_set(i, uid[i]);
                }
                HAL_GPIO_TogglePin(LED_0_GPIO_Port, LED_0_Pin);
            }
        }

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