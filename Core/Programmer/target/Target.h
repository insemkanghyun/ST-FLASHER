
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
#define STM32C0_DEV_ID_0x443					0x443 //STM32C011xx
#define STM32C0_DEV_ID_0x453					0x453 //STM32C031xx

#define STM32C0_REV_ID1					0X1000 //STM32C011xx(0x443) Revision Code A(1.0)
#define STM32C0_REV_ID2					0X1001 //STM32C011xx(0x443) Revision Code Z(1.1)
#define STM32C0_REV_ID3					0X1000 //STM32C031xx(0x453) Revision Code A(1.0)
#define STM32C0_REV_ID4					0X1001 //STM32C031xx(0x453) Revision Code Z(1.1)


/* STM32H7 DEV_ID & REV_ID */
#if 0
//#define STM32H7_DEV_ID1					0x485 //STM32H7Rx/7Sx
#define STM32H7_DEV_ID2					0x480 //STM32H7A3/7B3/7B0
#define STM32H7_DEV_ID3					0x483 //STM32H72x, STM32H73x
#define STM32H7_DEV_ID4					0x450 //STM32H742, STM32H743/753 and STM32H750 + STM32H745/755 and STM32H747/757
#endif
//#define STM32H7_DEV_ID1					0x485 //STM32H7Rx/7Sx
#define STM32H7_DEV_ID_0x480					0x480 //STM32H7A3/7B3/7B0
#define STM32H7_DEV_ID_0x483					0x483 //STM32H72x, STM32H73x
#define STM32H7_DEV_ID_0x450					0x450 //STM32H742, STM32H743/753 and STM32H750 + STM32H745/755 and STM32H747/757

#define STM32H7_DEV_ID2_REV_ID_A	0x1000
#define STM32H7_DEV_ID2_REV_ID_Z	0x1001
#define STM32H7_DEV_ID2_REV_ID_X	0x1007

#define STM32H7_DEV_ID3_REV_ID_A	0x1000
#define STM32H7_DEV_ID3_REV_ID_Z	0x1001

#define STM32H7_DEV_ID4_REV_ID_Z 0x1001
#define STM32H7_DEV_ID4_REV_ID_Y 0x1003
#define STM32H7_DEV_ID4_REV_ID_X 0x2001
#define STM32H7_DEV_ID4_REV_ID_V 0x2003

#define STM32H7_FLASH_SIZE_REG1	0x1FF1E880 //for Device ID 0x450
#define STM32H7_FLASH_SIZE_REG2 0x08FFF80c //for Device ID 0x480

#define STM32H7_FLASH_SUPPORT_DUALBANK 1
#define STM32H7_FLASH_SUPPORT_SINGLEBANK 0

#define STM32H7_FLASH_SIZE_128KB		0x80//128
#define STM32H7_FLASH_SIZE_512KB		0x200//512
#define STM32H7_FLASH_SIZE_1024KB		0x400//1024
#define STM32H7_FLASH_SIZE_2048KB		0x800//2048



#define TARGET_OK			0
#define TARGET_ERROR	1

typedef enum
{
  TARGET_STM32L0	= 0U,
  TARGET_STM32U0	= 1U,
  TARGET_STM32L4	= 2U,
  TARGET_STM32L5	= 3U,
  TARGET_STM32U5	= 4U,
  TARGET_STM32C0	= 5U,
  TARGET_STM32F0	= 6U,
  TARGET_STM32G0	= 7U,
  TARGET_STM32F1	= 8U,
  TARGET_STM32F3	= 9U,
  TARGET_STM32G4	= 10U,
  TARGET_STM32F2	= 11U,
  TARGET_STM32F4	= 12U,
  TARGET_STM32H5	= 13U,
  TARGET_STM32F7	= 14U,
  TARGET_STM32H7	= 15U,
} Target_FamilyTypeDef;


typedef struct target{
	uint32_t 							TargetDpId;
	uint32_t							TargetApId;
	Target_FamilyTypeDef	TargetFamily;
	uint32_t							TargetDevId;
	uint32_t							TargetRevId;
	uint32_t							TargetIsDualBank;
}Target_InfoTypeDef;

void Target_MainLoop(void);
void log_message(const char *format, ...);

#endif /* UTIL_INC_TARGET_H_ */
