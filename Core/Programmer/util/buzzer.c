#include <stdint.h>
#include "buzzer.h"
#include "stm32f4xx_hal.h"

extern TIM_HandleTypeDef htim1;
volatile BuzzerState state = BUZZER_BOOT;

#define DEFAULT_FREQ 3000



static void SetBuzzerFrequency(uint32_t frequency);
static void PlayBootSound(void);
static void PlayDuringProgrammingSound(void);
static void PlayProgrammingCompleteSound(void);
static void PlayProgrammingFailedSound(void);
static void PlayIdleSound(void);

void Buzzer_SetState(BuzzerState state)
{

	switch(state)
	{
		case BUZZER_BOOT:
			PlayBootSound();
			break;
		case BUZZER_PROG_START:
			PlayDuringProgrammingSound();
			break;
		case BUZZER_PROG_COMPLETE:
			PlayProgrammingCompleteSound();
			break;
		case BUZZER_PROG_FAILED:
			PlayProgrammingFailedSound();
			break;
		case BUZZER_IDLE:
			PlayIdleSound();
			break;
		default:
			break;
	}
	state = BUZZER_IDLE;
}

void SetBuzzerFrequency(uint32_t frequency) {
    if (frequency > 0) {
        // 타이머 주기 계산: 타이머 클럭(1MHz)에서 원하는 주파수를 설정
        uint32_t period = (1000000 / frequency) - 1;

        // 주기(ARR)를 설정하여 타이머의 PWM 주파수를 조정
        __HAL_TIM_SET_AUTORELOAD(&htim1, period);

        // 듀티 사이클을 50%로 설정 (부저가 50% 듀티에서 동작하도록)
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, period / 2);

        // 타이머 PWM 시작
        HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3);
    } else {
        // 주파수가 0이면 부저를 끕니다
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, 0); // Turn off buzzer
        HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_3);
    }
}

void PlayBootSound(void)
{
		/* Set Frequency */
    SetBuzzerFrequency(DEFAULT_FREQ);

    /* Set Duration */
    HAL_Delay(500);

    /* Buzzer Off */
    SetBuzzerFrequency(0);
}

void PlayDuringProgrammingSound(void)
{
		/* Set Frequency */
    SetBuzzerFrequency(DEFAULT_FREQ);

    /* Set Duration */
    HAL_Delay(150);

    /* Buzzer Off */
    SetBuzzerFrequency(0);
}

void PlayProgrammingCompleteSound(void)
{
		/* Set Frequency */
    SetBuzzerFrequency(DEFAULT_FREQ);

    /* Set Duration */
    HAL_Delay(300);

    /* Set Frequency */
    SetBuzzerFrequency(0);

    /* Set Duration */
    //HAL_Delay(300);
}

void PlayProgrammingFailedSound(void){

    for (uint8_t i = 0; i < 5; i++)
    {
    		/* Set Frequency */
        SetBuzzerFrequency(DEFAULT_FREQ);

        /* Set Duration */
        HAL_Delay(150);

        /* Buzzer Off */
        SetBuzzerFrequency(0);

        /* Set Duration */
        HAL_Delay(150);
    }
}

void PlayIdleSound(void)
{
    // No sound during idle
    SetBuzzerFrequency(0);
}
