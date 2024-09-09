#include <stdint.h>
#include "buzzer.h"
#include "stm32f4xx_hal.h"



extern TIM_HandleTypeDef htim2;



volatile BuzzerState state = BUZZER_BOOT;


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
        uint32_t period = (1000000 / frequency) - 1;
        htim2.Init.Period = period;
        HAL_TIM_PWM_Init(&htim2);
        __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, period / 2);
    } else {
        __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, 0); // Turn off buzzer
    }
}

void PlayBootSound(void)
{
		/* Set Frequency */
    SetBuzzerFrequency(5000);

    /* Set Duration */
    HAL_Delay(500);

    /* Buzzer Off */
    SetBuzzerFrequency(0);
}

void PlayDuringProgrammingSound(void)
{
		/* Set Frequency */
    SetBuzzerFrequency(5000);

    /* Set Duration */
    HAL_Delay(150);

    /* Buzzer Off */
    SetBuzzerFrequency(0);
}

void PlayProgrammingCompleteSound(void)
{
		/* Set Frequency */
    SetBuzzerFrequency(5000);

    /* Set Duration */
    HAL_Delay(300);

    /* Set Frequency */
    SetBuzzerFrequency(0);

    /* Set Duration */
    HAL_Delay(300);

    /* Set Frequency */
    SetBuzzerFrequency(5000);

    /* Set Duration */
    HAL_Delay(300);

    /* Buzzer Off */
    SetBuzzerFrequency(0);
}

void PlayProgrammingFailedSound(void){

    for (uint8_t i = 0; i < 6; i++)
    {
    		/* Set Frequency */
        SetBuzzerFrequency(5000);

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
