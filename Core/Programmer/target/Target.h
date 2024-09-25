
#ifndef UTIL_INC_TARGET_H_
#define UTIL_INC_TARGET_H_

#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include "led.h"
#include "buzzer.h"
#include "button.h"
#include "fatfs.h"
#include "swd\dap.h"
#include "swd\errors.h"

#include "swd\delay.h"
#include "util\ihex_parser.h"

#define FIRMWARE_FILENAME_HEX					"FIRMWARE.HEX"
#define FIRMWARE_FILENAME_BIN					"FIRMWARE.BIN"
#define FIRMWARE_PROTECT_OPTION_FILE	"OPTION.TXT"

/* Target SW-DP ID */
#define STM32C0_SWDP_ID		0x0BC11477
#define STM32H7_SWDP_ID		0x6BA02477

/* Target SW-AP ID */
#define STM32_AHBAP_ID_1  0x04770031 //STM32_C0x1
#define STM32_AHBAP_ID_2  0x84770001 //STM32_H743

/* Target Device ID Register Address */
#define STM32C0_REG_DEVICE_ID		0x40015800
#define STM32H7_REG_DEVICE_ID		0x5C001000

/* STM32C0 DEV_ID & REV_ID */
#define STM32C0_DEV_ID1					0x443 //STM32C011xx
#define STM32C0_DEV_ID2					0x453 //STM32C031xx

#define STM32C0_REV_ID1					0X1000 //STM32C011xx(0x443) Revision Code A(1.0)
#define STM32C0_REV_ID2					0X1001 //STM32C011xx(0x443) Revision Code Z(1.1)
#define STM32C0_REV_ID3					0X1000 //STM32C031xx(0x453) Revision Code A(1.0)
#define STM32C0_REV_ID4					0X1001 //STM32C031xx(0x453) Revision Code Z(1.1)


/* STM32H7 DEV_ID & REV_ID */
#define STM32H7_DEV_ID1					0x485 //STM32H7Rx/7Sx
#define STM32H7_DEV_ID2					0x480 //STM32H7A3/7B3/7B0
#define STM32H7_DEV_ID3					0x483 //STM32H72x, STM32H73x
#define STM32H7_DEV_ID4					0x450 //STM32H742, STM32H743/753 and STM32H750 + STM32H745/755 and STM32H747/757

#define TARGET_OK			0
#define TARGET_ERROR	1

typedef enum
{
  TARGET_STM32L0	= 0x00U,
  TARGET_STM32U0	= 0x01U,
  TARGET_STM32L4	= 0x02U,
  TARGET_STM32L5	= 0x03U,
  TARGET_STM32U5	= 0x04U,
  TARGET_STM32C0	= 0x05U,
  TARGET_STM32F0	= 0x06U,
  TARGET_STM32G0	= 0x07U,
  TARGET_STM32F1	= 0x08U,
  TARGET_STM32F3	= 0x09U,
  TARGET_STM32G4	= 0x10U,
  TARGET_STM32F2	= 0x11U,
  TARGET_STM32F4	= 0x12U,
  TARGET_STM32H5	= 0x13U,
  TARGET_STM32F7	= 0x14U,
  TARGET_STM32H7	= 0x15U,
} Target_FamilyTypeDef;


typedef struct target{
	uint32_t 							TargetDpId;
	uint32_t							TargetApId;
	Target_FamilyTypeDef	TargetFamily;
	uint32_t							TargetDevId;
	uint32_t							TargetRevId;
}Target_InfoTypeDef;

void Target_MainLoop(void);
void log_message(const char *format, ...);

#endif /* UTIL_INC_TARGET_H_ */
