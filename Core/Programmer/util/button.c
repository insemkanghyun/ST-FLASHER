#include "button.h"

#define BUTTON_GPIO_PORT PROGRAM_BTN_GPIO_Port
#define BUTTON_GPIO_PIN  PROGRAM_BTN_Pin

#define DEBOUNCE_DELAY_MS 20

static uint8_t button_state = GPIO_PIN_SET;
static uint8_t last_button_state = GPIO_PIN_SET;
static uint32_t last_debounce_time = 0;
static uint8_t current_state = 0;

static uint8_t button_pressed_event = 0;

void Button_Init(void)
{
#if 0
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    // GPIO 클럭 활성화
    __HAL_RCC_GPIOA_CLK_ENABLE(); // 사용하시는 포트에 맞게 수정하세요.

    // GPIO 핀 설정: 입력 모드, 풀다운 저항 사용 (회로에 따라 풀업 저항 사용 가능)
    GPIO_InitStruct.Pin = BUTTON_GPIO_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLDOWN; // 회로에 따라 GPIO_PULLUP으로 변경 가능

    HAL_GPIO_Init(BUTTON_GPIO_PORT, &GPIO_InitStruct);
#endif
}

void Button_Update(void)
{
    current_state = HAL_GPIO_ReadPin(BUTTON_GPIO_PORT, BUTTON_GPIO_PIN);

    if (current_state != last_button_state)
    {
        last_debounce_time = HAL_GetTick();
    }

    if ((HAL_GetTick() - last_debounce_time) > DEBOUNCE_DELAY_MS)
    {
        if (current_state != button_state)
        {
            button_state = current_state;

            if (button_state == GPIO_PIN_RESET)
            {
                button_pressed_event = 1;
            }
        }
    }

    last_button_state = current_state;
}

uint8_t Button_WasPressed(void)
{
    if (button_pressed_event)
    {
        button_pressed_event = 0;
        return 1;
    }
    return 0;
}
