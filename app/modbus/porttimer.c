#include <port.h>
#include <mbport.h>
#include <periph.h>

#include "modbus.h"

extern TIM_HandleTypeDef htim14;
static uint16_t timeout     = 0;
static uint16_t downcounter = 0;
static uint16_t poll_counter = 0;

BOOL xMBPortTimersInit(USHORT usTimerout50us) {
	htim14.Instance               = TIM14;
	htim14.Init.Prescaler         = (HAL_RCC_GetPCLK1Freq() / 1000000) - 1;
	htim14.Init.CounterMode       = TIM_COUNTERMODE_UP;
	htim14.Init.Period            = 50 - 1;
	htim14.Init.ClockDivision     = TIM_CLOCKDIVISION_DIV1;
	htim14.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	timeout = usTimerout50us;

	if (!HAL_TIM_Base_Init(&htim14) == HAL_OK) {
		return FALSE;
	}

	HAL_NVIC_SetPriority(TIM14_IRQn, 5, 0);
	HAL_NVIC_EnableIRQ(TIM14_IRQn);

	return TRUE;
}

void modbus_poll_enable() {
	poll_counter = 1000 / 50;
}

void vMBPortTimersEnable()
{
	downcounter = timeout;
	HAL_TIM_Base_Start_IT(&htim14);
}

void vMBPortTimersDisable() {
	// HAL_TIM_Base_Stop_IT(&htim14);
}

extern uint32_t test_var;

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
	if (htim->Instance != TIM14) {
		return;
	}

	if (downcounter == 0) {
		test_var++;
		pxMBPortCBTimerExpired();
	} else {
		downcounter--;
	}

	if (!--poll_counter) {
		modbus_poll();
		modbus_poll_enable();
	}

}

void TIM14_IRQHandler(void) {
	HAL_TIM_IRQHandler(&htim14);
}
