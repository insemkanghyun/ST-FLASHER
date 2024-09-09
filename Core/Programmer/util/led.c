
#ifndef UTIL_INC_LED_H_
#define UTIL_INC_LED_H_

#include "led.h"
#include "stm32f4xx_hal.h"
#include "main.h"

LedStatus currentState = TARGET_LED_STAT_BOOT;

// SysTick 타이머 주기 설정
#define SYS_TICK_PERIOD_MS  1
#define LED_TOGGLE_INTERVAL 200  // 200ms

// LED 제어 함수
void LED_Toggle(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin) {
    HAL_GPIO_TogglePin(GPIOx, GPIO_Pin);
}

void LED_On(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin) {
    HAL_GPIO_WritePin(GPIOx, GPIO_Pin, GPIO_PIN_RESET);
}

void LED_Off(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin) {
    HAL_GPIO_WritePin(GPIOx, GPIO_Pin, GPIO_PIN_SET);
}

// SysTick 인터럽트 핸들러
void LED_Handler(void)
{
    static uint32_t counter = 0;

    counter += SYS_TICK_PERIOD_MS;

    switch (currentState) {
        case TARGET_LED_STAT_BOOT:
            if (counter % LED_TOGGLE_INTERVAL == 0) {
                LED_Toggle(LED_GREEN_GPIO_Port, LED_GREEN_Pin);
                LED_Toggle(LED_ORANGE_GPIO_Port, LED_ORANGE_Pin);
                LED_Toggle(LED_RED_GPIO_Port, LED_RED_Pin);
            }
            break;
        case TARGET_LED_STAT_PROGRAMMING:
            if (counter % 100 == 0) { // Fast toggle rate
                LED_Toggle(LED_ORANGE_GPIO_Port, LED_ORANGE_Pin);
            }
            if (counter % 500 == 0) { // Red LED ON
                LED_On(LED_RED_GPIO_Port, LED_RED_Pin);
            } else {
                LED_Off(LED_RED_GPIO_Port, LED_RED_Pin);
            }
            break;
        case TARGET_LED_STAT_COMPLETE:
            LED_On(LED_GREEN_GPIO_Port, LED_GREEN_Pin);
            LED_Off(LED_ORANGE_GPIO_Port, LED_ORANGE_Pin);
            LED_On(LED_RED_GPIO_Port, LED_RED_Pin);
            break;
        case TARGET_LED_STAT_FAILED:
            if (counter % 500 == 0) {
                LED_Toggle(LED_RED_GPIO_Port, LED_RED_Pin);
            }
            LED_On(LED_GREEN_GPIO_Port, LED_GREEN_Pin);
            LED_Off(LED_ORANGE_GPIO_Port, LED_ORANGE_Pin);
            break;
        default:
            break;
    }
}

void LED_SetState(LedStatus status)
{
	currentState = status;
}

#endif //UTIL_INC_LED_H_
