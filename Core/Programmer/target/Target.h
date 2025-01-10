#ifndef UTIL_INC_TARGET_H_
#define UTIL_INC_TARGET_H_

/**
  ******************************************************************************
  * Include
  ******************************************************************************
**/
#include <stdint.h>


/**
  ******************************************************************************
  * Firmware & Option Byte file name configuration
  ******************************************************************************
**/
#define FIRMWARE_FILENAME_HEX					"FIRMWARE.HEX"
#define FIRMWARE_FILENAME_BIN					"FIRMWARE.BIN"
#define FIRMWARE_PROTECT_OPTION_FILE			"OPTION.TXT"

/**
  ******************************************************************************
  * SW-DP & AHB-AP ID Code
  ******************************************************************************
**/
#define STM32C0_SWDP_ID							0x0BC11477	//RM0490 / page1004
#define STM32H7_SWDP_ID							0x6BA02477	//RM0399 / page3238
#define STM32G0_SWDP_ID							0x0BC11477	//RM0444 / page1357
#define STM32C0_AHBAP_ID  						0x04770031	//?????
#define STM32H7_AHBAP_ID  						0x84770001	//RM0399 / page3251
#define STM32G0_AHBAP_ID  						0x04770031	//?????

/**
  ******************************************************************************
  * MCU Device ID Register Address
  ******************************************************************************
**/
#define STM32C0_REG_DEVICE_ID					0x40015800	//RM0490 / page1003
#define STM32H7_REG_DEVICE_ID					0x5C001000	//RM0399 / page3364
#define STM32G0_REG_DEVICE_ID					0x40015800	//RM0444 / page1355

/**
  ******************************************************************************
  * STM32C0 Device Specific define
  ******************************************************************************
**/
#define STM32C0_DEV_ID_0x443					0x443		//STM32C011xx
#define STM32C0_DEV_ID_0x453					0x453		//STM32C031xx
#define STM32C0_DEV_ID_0x44C					0x44C		//STM32C051xx
#define STM32C0_DEV_ID_0x493					0x493		//STM32C071xx
#define STM32C0_DEV_ID_0x44D					0x44D		//STM32C091xx/92xx

/**
  ******************************************************************************
  * STM32H7 Device Specific define
  ******************************************************************************
**/
#define STM32H7_DEV_ID_0x485					0x485		//STM32H7Rx/7Sx
#define STM32H7_DEV_ID_0x480					0x480		//STM32H7A3/7B3/7B0
#define STM32H7_DEV_ID_0x483					0x483		//STM32H72x, STM32H73x
#define STM32H7_DEV_ID_0x450					0x450		//STM32H742, STM32H743/753 and STM32H750 + STM32H745/755 and STM32H747/757

#define STM32H7_REG_FLASH_SIZE_1				0x1FF1E880	//for Device ID 0x450
#define STM32H7_REG_FLASH_SIZE_2				0x08FFF80c	//for Device ID 0x480

#define STM32H7_FLASH_SIZE_128KB				0x80		//128 KB
#define STM32H7_FLASH_SIZE_512KB				0x200		//512 KB
#define STM32H7_FLASH_SIZE_1024KB				0x400		//1024 KB
#define STM32H7_FLASH_SIZE_2048KB				0x800		//2048 KB

#define STM32H7_FLASH_SUPPORT_DUALBANK			1
#define STM32H7_FLASH_SUPPORT_SINGLEBANK		0

/**
  ******************************************************************************
  * STM32G0 Device Specific define
  ******************************************************************************
**/
#define STM32G0_DEV_ID_0x467					0x467		//STM32G0B0xx, STM32G0B1xx and STM32G0C1xx
#define STM32G0_DEV_ID_0x460					0x460		//STM32G070xx, STM32G071xx and STM32G081xx
#define STM32G0_DEV_ID_0x456					0x456		//STM32G050xx, STM32G051xx and STM32G061xx
#define STM32G0_DEV_ID_0x466					0x466		//STM32G030xx, STM32G031xx and STM32G041xx

/**
  ******************************************************************************
  * Others
  ******************************************************************************
**/
#define TARGET_OK		0
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
	Target_FamilyTypeDef				TargetFamily;
	uint32_t							TargetDevId;
	uint32_t							TargetRevId;
	uint32_t							TargetIsDualBank;
	uint32_t							TargetChipErased;
}Target_InfoTypeDef;

void Target_MainLoop(void);
void log_message(const char *format, ...);

#endif /* UTIL_INC_TARGET_H_ */
