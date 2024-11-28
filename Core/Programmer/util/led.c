
#ifndef UTIL_INC_LED_H_
#define UTIL_INC_LED_H_

#include "led.h"
#include "stm32f4xx_hal.h"
#include "main.h"
#include "stdbool.h"

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

// Boot 상태 처리 함수
void LED_HandleBootState(uint32_t *counter, uint8_t *step)
{
    if (*counter >= LED_TOGGLE_INTERVAL * 2)
    {
        *counter = 0; // 카운터 초기화

        switch (*step)
        {
            case 0:
                LED_On(LED_GREEN_GPIO_Port, LED_GREEN_Pin);
                LED_Off(LED_ORANGE_GPIO_Port, LED_ORANGE_Pin);
                LED_Off(LED_RED_GPIO_Port, LED_RED_Pin);
                break;

            case 1:
                LED_Off(LED_GREEN_GPIO_Port, LED_GREEN_Pin);
                LED_On(LED_ORANGE_GPIO_Port, LED_ORANGE_Pin);
                LED_Off(LED_RED_GPIO_Port, LED_RED_Pin);
                break;

            case 2:
                LED_Off(LED_GREEN_GPIO_Port, LED_GREEN_Pin);
                LED_Off(LED_ORANGE_GPIO_Port, LED_ORANGE_Pin);
                LED_On(LED_RED_GPIO_Port, LED_RED_Pin);
                break;

            case 3:
                LED_On(LED_GREEN_GPIO_Port, LED_GREEN_Pin);
                LED_Off(LED_ORANGE_GPIO_Port, LED_ORANGE_Pin);
                LED_Off(LED_RED_GPIO_Port, LED_RED_Pin);
                break;

            default:
                *step = 0; // 마지막 단계 이후 초기화
                return;
        }

        (*step)++;
    }
}

// Programming 상태 처리 함수
void LED_HandleProgrammingState(uint32_t counter)
{
    if (counter % 100 == 0) // 빠른 토글 주기
    {
        LED_Toggle(LED_ORANGE_GPIO_Port, LED_ORANGE_Pin);
    }

    LED_Off(LED_GREEN_GPIO_Port, LED_GREEN_Pin);
    LED_Off(LED_RED_GPIO_Port, LED_RED_Pin);
}

// Complete 상태 처리 함수
void LED_HandleCompleteState(void)
{
    LED_On(LED_GREEN_GPIO_Port, LED_GREEN_Pin);
    LED_Off(LED_ORANGE_GPIO_Port, LED_ORANGE_Pin);
    LED_Off(LED_RED_GPIO_Port, LED_RED_Pin);
}

// Failed 상태 처리 함수
void LED_HandleFailedState(void)
{
    LED_Off(LED_GREEN_GPIO_Port, LED_GREEN_Pin);
    LED_Off(LED_ORANGE_GPIO_Port, LED_ORANGE_Pin);
    LED_On(LED_RED_GPIO_Port, LED_RED_Pin);
}

// SysTick 인터럽트 핸들러
void LED_Handler(void)
{
    static uint32_t counter = 0;
    static uint8_t step = 0; // LED 순차적 제어를 위한 단계 변수
    static bool bootCompleted = false; // BOOT 상태 완료 여부

    counter += SYS_TICK_PERIOD_MS;

    switch (currentState)
    {
        case TARGET_LED_STAT_BOOT:
            if (!bootCompleted)
            {
                // BOOT 상태 처리
            	LED_HandleBootState(&counter, &step);

                // BOOT 상태 완료 시 COMPLETE로 전환
                if (step > 3) // 모든 단계 완료
                {
                    currentState = TARGET_LED_STAT_COMPLETE; // 상태 변경
                    bootCompleted = true; // BOOT 완료 플래그 설정
                }
            }
            break;

        case TARGET_LED_STAT_PROGRAMMING:
        		LED_HandleProgrammingState(counter);
            break;

        case TARGET_LED_STAT_COMPLETE:
        		LED_HandleCompleteState();
            break;

        case TARGET_LED_STAT_FAILED:
        		LED_HandleFailedState();
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
