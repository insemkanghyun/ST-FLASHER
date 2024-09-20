#ifndef BUTTON_H
#define BUTTON_H

#include "stm32f4xx_hal.h"
#include "main.h"

void Button_Init(void);
void Button_Update(void);
uint8_t Button_WasPressed(void);


#endif // BUTTON_H
