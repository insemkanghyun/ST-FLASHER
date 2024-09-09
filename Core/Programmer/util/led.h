/*
 * led.h
 *
 *  Created on: Jul 5, 2024
 *      Author: kangh
 */

#ifndef LED_H_
#define LED_H_

typedef enum
{
  TARGET_LED_STAT_BOOT				= 0x00U,
	TARGET_LED_STAT_PROGRAMMING	= 0x01U,
	TARGET_LED_STAT_COMPLETE		= 0x02U,
	TARGET_LED_STAT_FAILED			= 0x03U,
} LedStatus;

void LED_Handler(void);
void LED_SetState(LedStatus status);
#endif /* LED_H_ */
