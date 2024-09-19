#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include "Target.h"
#include "fatfs.h"
#include "swd\dap.h"
#include "swd\errors.h"
#include "swd\utils.h"
#include "swd\delay.h"
#include "util\ihex_parser.h"
#include "stm32c0_flash.h"
#include "led.h"
#include "buzzer.h"

#define PRINTF_REDIRECTION	int __io_putchar(int ch)
#define TO_BE_IMPLEMENT_CALLBACK 0

//#define printf(...) //for printf remove

volatile Target_InfoTypeDef target;
volatile uint8_t u8_ButtonPushed = 0;
volatile uint8_t u8_ButtonLock = 0;
int u32_StartTime = 0;
int u32_ElasedTime = 0;
extern UART_HandleTypeDef huart1;

static void Target_Classify(Target_InfoTypeDef *target);
static void Target_ErrorHandle(Target_StatusTypeDef status, const char *errorMessage);

/* iHex_Parser Callback */
static bool Target_VerifyCallback(uint32_t addr, const uint8_t *buf, uint8_t bufsize);
static bool Target_ProgramCallback_STM32C0(uint32_t addr, const uint8_t *buf, uint8_t bufsize);
static bool (*Target_ProgramCallback[])(uint32_t addr, const uint8_t *buf, uint8_t bufsize)={\
					TO_BE_IMPLEMENT_CALLBACK,\
					TO_BE_IMPLEMENT_CALLBACK,\
					TO_BE_IMPLEMENT_CALLBACK,\
					TO_BE_IMPLEMENT_CALLBACK,\
					TO_BE_IMPLEMENT_CALLBACK,\
					Target_ProgramCallback_STM32C0};

void Target_MainLoop(void)
{
	Target_StatusTypeDef status = TARGET_ERROR;

	/* Button programming start */
  if(u8_ButtonPushed)
  {
  	Target_LedSet(TARGET_LED_STAT_PROGRAMMING);
  	Buzzer_SetState(BUZZER_PROG_START);
  	u32_StartTime = HAL_GetTick();

    /* Reset button status for next programming */
  	u8_ButtonPushed = 0;

  	/* Set button lock flag for block when target flash operation  */
  	u8_ButtonLock = 1;

  	/* Target flash operation */
  	status = Target_Connect();
  	Target_ErrorHandle(status, "Target Connect Error");
  	if(status != TARGET_OK) return;

  	status = Target_Proteciton_Unlock();
  	Target_ErrorHandle(status, "Target Protection Unlock Error");
  	if(status != TARGET_OK) return;

  	status = Target_MassErase();
  	Target_ErrorHandle(status, "Target MassErase Error");
  	if(status != TARGET_OK) return;

  	status = Target_Program();
  	Target_ErrorHandle(status, "Target Program Error");
  	if(status != TARGET_OK) return;

  	status = Target_Verfify();
  	Target_ErrorHandle(status, "Target Verify Error");
  	if(status != TARGET_OK) return;

  	status = Target_Protection_Lock();
  	Target_ErrorHandle(status, "Target Protection Lock Error");
  	if(status != TARGET_OK) return;

  	printf("Target program completed\n");

    /* Target flash operation completed & button lock flag release  */
    u8_ButtonLock = 0;


    u32_ElasedTime = HAL_GetTick() - u32_StartTime;
    printf("Total Elapsed Programming Time: %d ms\n\n", u32_ElasedTime);
    Target_LedSet(TARGET_LED_STAT_COMPLETE);
    Buzzer_SetState(BUZZER_PROG_COMPLETE);
  }
}

Target_StatusTypeDef Target_Connect(void)
{
  uint32_t	retry = CONNECT_RETRY_COUNT;

  printf("Target Connect\n");
  hardResetTarget();
  delayMs(50);

  /* Try connecting several times */
	while ( retry-- > 0 )
  {
		TRY
			connectToTarget(&target);
			Target_Classify(&target);

			/* Reset swdErrorIndex, it cause f_open(fatfs) hang. */
			swdErrorIndex = 0;

    	/* Stop retrying */
    	return TARGET_OK;
		CATCH
    	printf("SWD Error: %s\n", getErrorString(errorCode));
    	printf("Failed to connect. Retrying...\n");
    	//delayUs(200);
    	delayMs(200);
    ENDTRY
  }

  printf("Target Probe Error\n");
  return TARGET_ERROR;
}

Target_StatusTypeDef Target_Program(void)
{
		uint8_t fbuf[256];
    size_t readcount = 0;
    FRESULT res;
    FIL HexFile;
    FILINFO fileInfo;

    printf("Target Program\n");

    /* Hex parser callback registeration */
    ihex_set_callback_func((ihex_callback_fp)*Target_ProgramCallback[target.TargetFamily]);
    ihex_reset_state();

    /* File open */
    res =  f_open(&HexFile, FIRMWARE_FILENAME_HEX, FA_READ);
    if(res != FR_OK)
    {
    	printf("f_open error\n");
    	return TARGET_ERROR;
    }

    /* File state */
    res =  f_stat(FIRMWARE_FILENAME_HEX, &fileInfo);
    if(res != FR_OK)
    {
    	printf("f_stat error\n");
    	return TARGET_ERROR;
    }
    printf("-File name: %s\n", fileInfo.fname);
    printf("-File size: %lu bytes\n", fileInfo.fsize);



    while (1)
    {
    	/* File read */
    	res = f_read(&HexFile, fbuf, sizeof(fbuf), &readcount);
      if(res != FR_OK)
      {
      	printf("f_read error\n");
      	return TARGET_ERROR;
      }

      /* If there is no data to read, end programming loop*/
    	if(readcount ==  0)
    	{
    		res = f_close(&HexFile);
        if(res != FR_OK)
        {
        	printf("f_close error\n");
        	return TARGET_ERROR;
        }
    		break;
    	}
    	/* If data is available, hex parsing & swd flash write */
    	else
    	{
    		/* If readcount smaller then sizeof(buf), add null termination character end of buffer */
    		if(readcount < sizeof(fbuf))
    		{
    			fbuf[readcount] = '\0';
    		}

    		/* Hex parsing & swd flash write */
    		if (!ihex_parser(fbuf, sizeof(fbuf)))
    		{
    			printf("ihex_parser error\n");
    			return TARGET_ERROR;
    		}
    	}
   }


    return TARGET_OK;
}

Target_StatusTypeDef Target_MassErase(void)
{
	printf("Target MassErase\n");
	switch(target.TargetFamily)
	{
		case TARGET_STM32C0:
			Stm32c0_Flash_Unlock();
			Stm32c0_Flash_MassErase();
			Stm32c0_Flash_Lock();
			break;
		default:
			break;
	}
	return TARGET_OK;
}





static void Target_Classify(Target_InfoTypeDef *target)
{
	uint32_t tmp = 0;

	/* Target Family Classify & Read Device ID */
	if(target->TargetDpId == STM32C0_SWDP_ID)
	{
		target->TargetFamily = TARGET_STM32C0;
		tmp = readMem(STM32C0_REG_DEVICE_ID);

	}
	else if(target->TargetDpId == STM32H7_SWDP_ID)
	{
		target->TargetFamily = TARGET_STM32H7;
		tmp = readMem(STM32H7_REG_DEVICE_ID);
	}
	else
	{
		/* Do Nothing */
	}

	target->TargetDevId = tmp & 0xFFF;
	target->TargetRevId = tmp >> 16;

	printf("Target Device is ");
	switch(target->TargetDevId)
	{
		case STM32C0_DEV_ID1:
			printf("STM32C011xx");
			break;
		case STM32C0_DEV_ID2:
			printf("STM32C031xx");
			break;
		case STM32H7_DEV_ID1:
			printf("STM32H7Rx/7Sx\n");
			break;
		case STM32H7_DEV_ID2:
			printf("STM32H7A3/7B3/7B0\n");
			break;
		case STM32H7_DEV_ID3:
			printf("STM32H72x, STM32H73x\n");
			break;
		case STM32H7_DEV_ID4:
			printf("STM32H742, STM32H743/753 and STM32H750\n");
			printf("STM32H745/755 and STM32H747/757\n");
			break;
		default:
			printf("Unknown Device ID.\n");
			//Error_Handler();
			break;
	}

	printf(" | ");
	//if stm32c0...
	switch(target->TargetRevId)
	{
	case STM32C0_REV_ID1:
	//case STM32C0_REV_ID3:
		printf("Revision Code A(1.0)\n");
		break;
	case STM32C0_REV_ID2:
	//case STM32C0_REV_ID4:
		printf("Revision Code Z(1.1)\n");
		break;
	default:
		printf("Unknown Revision ID\n");
	}
}

bool Target_ProgramCallback_STM32C0(uint32_t addr, const uint8_t *buf, uint8_t bufsize)
{
	volatile uint64_t tmp[4] = {0};
	Target_StatusTypeDef status = TARGET_OK;

	/* Check valid flash address */
	/* To be implement */

	/* Convert Hex parsed data (uint8_t, Byte) to uint64_t(Double Word) */
	/* STM32C0 Flash Double Word Program Support(Standard) */
  for (int i = 0; i < 4; i++) {
  	tmp[i] = ((uint64_t*)buf)[i];
  }

  /*Flash unlock before programming */
  status = Stm32c0_Flash_Unlock();
  if(status != TARGET_OK)
  	return false;

  /*Flash programming double word */
  for(uint32_t i = 0; i < 4; i++)
	{
		status = Stm32c0_Flash_Program(addr + (i*8), tmp[i]);
		if(status != TARGET_OK)
		{
			Stm32c0_Flash_Lock();
			return false;
		}
		/* If bufsize only one Double word, quit loop */
			if (bufsize <= (i + 1) * 8)
			break;
	}

  /*Flash lock after programming */
  status = Stm32c0_Flash_Lock();
	if(status != TARGET_OK)
		return false;

	return true;
}

bool Target_VerifyCallback(uint32_t addr, const uint8_t *buf, uint8_t bufsize)
{
	uint8_t tmp[32];
	uint32_t u32_ReadData[8];

	/* Read 4-word from target flash memory */
	for(int i = 0; i < (bufsize/4); i++)
	{
		u32_ReadData[i] = readMem(addr + (i*4));
	}

	/* Convert uint32_t to uint8_t */
  for (int i = 0; i < (bufsize / 4); i++) {
  	tmp[4 * i]     = u32_ReadData[i] & 0xFF;
  	tmp[4 * i + 1] = (u32_ReadData[i] >> 8) & 0xFF;
  	tmp[4 * i + 2] = (u32_ReadData[i] >> 16) & 0xFF;
  	tmp[4 * i + 3] = (u32_ReadData[i] >> 24) & 0xFF;
  }

  /* Compare Hex & Flash Data */
	for(int i = 0; i < bufsize; i++)
	{
		if(buf[i] != tmp[i])
		{
			printf("Verification failed at address 0x%08"PRIX32"\n", (addr + i));
			printf("Value is 0x%02"PRIX16", should have been 0x%02"PRIX16"\n", tmp[i], buf[i]);
			return false;
		}
	}
	return true;
}

Target_StatusTypeDef Target_Verfify(void)
{
	uint8_t fbuf[256];
  size_t readcount = 0;
  FRESULT res;
  FIL HexFile;

  printf("Target Verify\n");

  /* Hex parser callback registeration */
  ihex_set_callback_func((ihex_callback_fp)*Target_VerifyCallback);
  ihex_reset_state();

  /* File open */
  res =  f_open(&HexFile, FIRMWARE_FILENAME_HEX, FA_READ);
  if(res != FR_OK)
  {
  	printf("f_open error\n");
  	return TARGET_ERROR;
  }

  while (1)
  {
  	/* File read */
  	res = f_read(&HexFile, fbuf, sizeof(fbuf), &readcount);
    if(res != FR_OK)
    {
    	printf("f_read error\n");
    	return TARGET_ERROR;
    }

    /* If there is no data to read, end verfify loop*/
  	if(readcount ==  0)
  	{
  		res = f_close(&HexFile);
      if(res != FR_OK)
      {
      	printf("f_close error\n");
      	return TARGET_ERROR;
      }

  		break;
  	}
  	else
  	{
  		/* If readcount smaller then sizeof(buf), add null termination character end of buffer */
  		if(readcount < sizeof(fbuf))
  		{
  			fbuf[readcount] = '\0';
  		}

  		/* Hex parsing & swd flash verify */
  		if (!ihex_parser(fbuf, sizeof(fbuf)))
  		{
  				printf("ihex_parser error\n");
  				return TARGET_ERROR;
  		}
  	}
  }
  return TARGET_OK;
}

Target_StatusTypeDef Target_Proteciton_Unlock(void)
{
	Target_StatusTypeDef status = TARGET_ERROR;
	uint32_t Option_Status = 0;

	printf("Target protection check before flash programming.\n");

	switch(target.TargetFamily)
	{
		case TARGET_STM32C0:
			/* Read option byte */
			Option_Status = readMem(STM32C0_FLASH_OPTION_OPTR) & STM32C0_FLASH_OPTR_RDP_Msk;
			printf("Current target RDP = 0x%02"PRIX32"\n", Option_Status);

			/* If option byte Lv1 */
			if(Option_Status == STM32C0_OB_RDP_LEVEL_1)
			{
				printf("Configure target RDP Level-1(0xBB) to Level-2(0xAA) for flash programming.\n");
				/* Unlock */
				Stm32c0_Flash_Unlock();
				Stm32c0_Flash_OB_Unlock();
				Stm32c0_Flash_OB_Program(STM32C0_OB_RDP_LEVEL_0);

				printf("RDP programming OK. Option Byte launch will generate 'system reset'\n");
				Stm32c0_Flash_OB_Launch();
				Target_Connect();
				Stm32c0_Flash_OB_Lock();
				Stm32c0_Flash_Lock();

				Option_Status = readMem(STM32C0_FLASH_OPTION_OPTR);
				printf("Modified RDP = 0x%02"PRIX32"\n", Option_Status & STM32C0_FLASH_OPTR_RDP_Msk);
			}
			else if(Option_Status == STM32C0_OB_RDP_LEVEL_0)
			{
				printf("Not Locked chip\n");
			}
			else //if(Option_Status == STM32C0_OB_RDP_LEVEL_2)
			{
				printf("Locked chip or Error, it cannot program.\n");
				return TARGET_ERROR;
			}
			break;
		default:
			break;
	}
	return TARGET_OK;
}

Target_StatusTypeDef Target_Protection_Lock(void)
{
	Target_StatusTypeDef status = TARGET_ERROR;
	uint32_t Option_Status = 0;

	printf("Target protection configure.\n");

	switch(target.TargetFamily)
	{
		case TARGET_STM32C0:
				printf("Configure target RDP Level-2(0xAA) to Level-1(0xBB) for flash protection.\n");
				Stm32c0_Flash_Unlock();
				Stm32c0_Flash_OB_Unlock();
				Stm32c0_Flash_OB_Program(STM32C0_OB_RDP_LEVEL_1);
				Stm32c0_Flash_OB_Launch();
				Target_Connect();
				Stm32c0_Flash_OB_Lock();
				Stm32c0_Flash_Lock();

				Option_Status = readMem(STM32C0_FLASH_OPTION_OPTR);
				printf("Modified RDP = 0x%02"PRIX32"\n", Option_Status & STM32C0_FLASH_OPTR_RDP_Msk);

			break;
		default:
			break;
	}
	return TARGET_OK;
}



void Target_BuutonPush(void)
{
  if(u8_ButtonLock == 0)
  {
  	u8_ButtonPushed = 1;
  }
  else
  {
  	/* Do Nothing */
  }
}

static void Target_ErrorHandle(Target_StatusTypeDef status, const char *errorMessage)
{
    if (status != TARGET_OK)
    {
        printf("%s\n", errorMessage);
        Target_LedSet(TARGET_LED_STAT_FAILED);
        Buzzer_SetState(BUZZER_PROG_FAILED);
        u8_ButtonLock = 0;
    }
}

void Target_LedSet(LedStatus status)
{
	LED_SetState(status);
}

PRINTF_REDIRECTION
{
  HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 0xFFFF);
  return ch;
}


