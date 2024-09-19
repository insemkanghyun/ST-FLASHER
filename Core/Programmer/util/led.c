
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
    static uint8_t step = 0;  // LED 순차적 제어를 위한 단계 변수


    switch (currentState) {
        case TARGET_LED_STAT_BOOT:
#if 0
            if (counter % LED_TOGGLE_INTERVAL == 0) {
                LED_Toggle(LED_GREEN_GPIO_Port, LED_GREEN_Pin);
                LED_Toggle(LED_ORANGE_GPIO_Port, LED_ORANGE_Pin);
                LED_Toggle(LED_RED_GPIO_Port, LED_RED_Pin);
            }
            break;
#endif
            if (counter >= LED_TOGGLE_INTERVAL && step == 0) {
            	LED_On(LED_GREEN_GPIO_Port, LED_GREEN_Pin);   // 초록색 켜기
            	LED_Off(LED_ORANGE_GPIO_Port, LED_ORANGE_Pin);
            	LED_Off(LED_RED_GPIO_Port, LED_RED_Pin);
            	step++;
            	counter = 0;  // 카운터 초기화
            }
            if (counter >= LED_TOGGLE_INTERVAL && step == 1) {
            	LED_Off(LED_GREEN_GPIO_Port, LED_GREEN_Pin);
            	LED_On(LED_ORANGE_GPIO_Port, LED_ORANGE_Pin);  // 주황색 켜기
            	LED_Off(LED_RED_GPIO_Port, LED_RED_Pin);
            	step++;
            	counter = 0;
            }
            if (counter >= LED_TOGGLE_INTERVAL && step == 2) {
            	LED_Off(LED_GREEN_GPIO_Port, LED_GREEN_Pin);
            	LED_Off(LED_ORANGE_GPIO_Port, LED_ORANGE_Pin);
            	LED_On(LED_RED_GPIO_Port, LED_RED_Pin);        // 빨간색 켜기
            	step++;
            	counter = 0;
            }
            if (counter >= LED_TOGGLE_INTERVAL && step == 3) {
            	LED_On(LED_GREEN_GPIO_Port, LED_GREEN_Pin);    // 초록색 켜고 나머지 끄기
            	LED_Off(LED_ORANGE_GPIO_Port, LED_ORANGE_Pin);
            	LED_Off(LED_RED_GPIO_Port, LED_RED_Pin);
            	step++;  // 마지막 단계로 진행
            }
            break;
        case TARGET_LED_STAT_PROGRAMMING:
            if (counter % 100 == 0) { // Fast toggle rate
                LED_Toggle(LED_ORANGE_GPIO_Port, LED_ORANGE_Pin);
            }
          	LED_Off(LED_GREEN_GPIO_Port, LED_GREEN_Pin);
          	LED_Off(LED_RED_GPIO_Port, LED_RED_Pin);
            break;
        case TARGET_LED_STAT_COMPLETE:
        	LED_On(LED_GREEN_GPIO_Port, LED_GREEN_Pin);
        	LED_Off(LED_ORANGE_GPIO_Port, LED_ORANGE_Pin);
        	LED_Off(LED_RED_GPIO_Port, LED_RED_Pin);
            break;
        case TARGET_LED_STAT_FAILED:
        	LED_Off(LED_GREEN_GPIO_Port, LED_GREEN_Pin);
          LED_Off(LED_ORANGE_GPIO_Port, LED_ORANGE_Pin);
          LED_On(LED_RED_GPIO_Port, LED_RED_Pin);
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
