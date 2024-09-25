#include "Target.h"
#include "stm32c0_flash.h"
#include "swd\utils.h"
#include <stdarg.h>


Target_InfoTypeDef target;
extern UART_HandleTypeDef huart1;
extern RTC_HandleTypeDef hrtc;

#define PRINTF_REDIRECTION	int __io_putchar(int ch)
//#define printf(...) //for printf remove
PRINTF_REDIRECTION
{
  HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 0xFFFF);
  return ch;
}

void log_message(const char *format, ...)
{
    char buffer[256];
    va_list args;
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);

    // RTC에서 현재 시간 가져오기
    RTC_TimeTypeDef sTime;
    RTC_DateTypeDef sDate;
    uint32_t subseconds;
    uint32_t milliseconds;

    HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
    HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BIN);

    // 서브초 값 가져오기
    subseconds = sTime.SubSeconds;
    uint32_t SecondFraction = hrtc.Init.SynchPrediv;

    // 밀리초 계산
    milliseconds = ((SecondFraction - subseconds) * 1000) / (SecondFraction + 1);

    // 시간 문자열 포맷팅
    char time_str[32];
    snprintf(time_str, sizeof(time_str), "%04d-%02d-%02d %02d:%02d:%02d.%03lu",
             2000 + sDate.Year, sDate.Month, sDate.Date,
             sTime.Hours, sTime.Minutes, sTime.Seconds, milliseconds);

    // 타임스탬프와 메시지 출력
    printf("[%s] %s", time_str, buffer);
}

/* iHex_Parser Callback */
#define TO_BE_IMPLEMENT_CALLBACK 0
static bool Target_ProgramCallback_STM32C0(uint32_t addr, const uint8_t *buf, uint8_t bufsize);
static bool (*Target_ProgramCallback[])(uint32_t addr, const uint8_t *buf, uint8_t bufsize)={\
					TO_BE_IMPLEMENT_CALLBACK,\
					TO_BE_IMPLEMENT_CALLBACK,\
					TO_BE_IMPLEMENT_CALLBACK,\
					TO_BE_IMPLEMENT_CALLBACK,\
					TO_BE_IMPLEMENT_CALLBACK,\
					Target_ProgramCallback_STM32C0};

static uint32_t Target_GetFlashStartAddress(void)
{
    switch(target.TargetFamily)
    {
        case TARGET_STM32C0:
            return 0x08000000;
            break;
        case TARGET_STM32F4:
            return 0x08000000;
            break;
        /* Add other target families and their start addresses as needed */
        default:
            log_message("Unknown target family. Using default start address 0x08000000.\n");
            return 0x08000000;
    }
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

	switch(target->TargetDevId)
	{
		case STM32C0_DEV_ID1:
			log_message("STM32C011xx\n");
			break;
		case STM32C0_DEV_ID2:
			log_message("STM32C031xx\n");
			break;
		case STM32H7_DEV_ID1:
			log_message("STM32H7Rx/7Sx\n");
			break;
		case STM32H7_DEV_ID2:
			log_message("STM32H7A3/7B3/7B0\n");
			break;
		case STM32H7_DEV_ID3:
			log_message("STM32H72x, STM32H73x\n");
			break;
		case STM32H7_DEV_ID4:
			log_message("STM32H742, STM32H743/753 and STM32H750\n");
			log_message("STM32H745/755 and STM32H747/757\n");
			break;
		default:
			log_message("Unknown Device ID.\n");
			//Error_Handler();
			break;
	}

	//if stm32c0...
	switch(target->TargetRevId)
	{
	case STM32C0_REV_ID1:
	//case STM32C0_REV_ID3:
		log_message("Revision Code A(1.0)\n");
		break;
	case STM32C0_REV_ID2:
	//case STM32C0_REV_ID4:
		log_message("Revision Code Z(1.1)\n");
		break;
	default:
		log_message("Unknown Revision ID\n");
	}
}

static bool Target_Connect(void)
{
  uint32_t	retry = CONNECT_RETRY_COUNT;

  log_message("Target Connect\n");
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
    	log_message("SWD Error: %s\n", getErrorString(errorCode));
    	log_message("Failed to connect. Retrying...\n");
    	//delayUs(200);
    	delayMs(200);
    ENDTRY
  }

  log_message("Target Probe Error\n");
  return TARGET_ERROR;
}

/* Function to program HEX files */
static bool Target_ProgramHex(void)
{
    uint8_t fbuf[256];
    size_t readcount = 0;
    FRESULT res;
    FIL file;
    FILINFO fileInfo;

    /* Get file info */
    res = f_stat(FIRMWARE_FILENAME_HEX, &fileInfo);
    if (res != FR_OK)
    {
        log_message("f_stat error\n");
        return TARGET_ERROR;
    }
    log_message("Firmware File Information\n");
    log_message("(1) File name: %s\n", fileInfo.fname);
    log_message("(2) File size: %lu bytes\n", fileInfo.fsize);

    /* Open the HEX file */
    res = f_open(&file, FIRMWARE_FILENAME_HEX, FA_READ);
    if (res != FR_OK)
    {
        log_message("f_open error\n");
        return TARGET_ERROR;
    }

    /* Hex parser callback registration */
    ihex_set_callback_func((ihex_callback_fp)*Target_ProgramCallback[target.TargetFamily]);
    ihex_reset_state();

    /* Process the HEX file */
    while (1)
    {
        /* Read from the file */
        res = f_read(&file, fbuf, sizeof(fbuf), &readcount);
        if (res != FR_OK)
        {
            log_message("f_read error\n");
            f_close(&file);
            return TARGET_ERROR;
        }

        /* End of file */
        if (readcount == 0)
        {
            res = f_close(&file);
            if (res != FR_OK)
            {
                log_message("f_close error\n");
                return TARGET_ERROR;
            }
            break;
        }
        else
        {
            /* Null-terminate if needed */
            if (readcount < sizeof(fbuf))
            {
                fbuf[readcount] = '\0';
            }

            /* Parse HEX data and program */
            if (!ihex_parser(fbuf, readcount))
            {
                log_message("ihex_parser error\n");
                f_close(&file);
                return TARGET_ERROR;
            }
        }
    }

    return TARGET_OK;
}

/* Function to program BIN files */
static bool Target_ProgramBin(void)
{
    uint8_t fbuf[256];
    size_t readcount = 0;
    FRESULT res;
    FIL file;
    FILINFO fileInfo;
    uint32_t address = Target_GetFlashStartAddress(); // Starting address for binary programming

    /* Get file info */
    res = f_stat(FIRMWARE_FILENAME_BIN, &fileInfo);
    if (res != FR_OK)
    {
        log_message("f_stat error\n");
        return TARGET_ERROR;
    }
    log_message("Firmware File Information\n");
    log_message("(1) File name: %s\n", fileInfo.fname);
    log_message("(2) File size: %lu bytes\n", fileInfo.fsize);

    /* Open the BIN file */
    res = f_open(&file, FIRMWARE_FILENAME_BIN, FA_READ);
    if (res != FR_OK)
    {
        log_message("f_open error\n");
        return TARGET_ERROR;
    }

    /* Program the BIN file */
    while (1)
    {
        /* Read from the file */
        res = f_read(&file, fbuf, sizeof(fbuf), &readcount);
        if (res != FR_OK)
        {
            log_message("f_read error\n");
            f_close(&file);
            return TARGET_ERROR;
        }

        /* End of file */
        if (readcount == 0)
        {
            res = f_close(&file);
            if (res != FR_OK)
            {
                log_message("f_close error\n");
                return TARGET_ERROR;
            }
            break;
        }
        else
        {
            size_t i = 0;
            uint32_t current_address = address;

            while (i < readcount)
            {
                /* Determine the size to process (up to 16 bytes) */
                size_t chunk_size = (readcount - i) >= 16 ? 16 : (readcount - i);

                /* Call the programming function */
                if (!Target_ProgramCallback_STM32C0(current_address, &fbuf[i], (uint8_t)chunk_size))
                {
                    log_message("Flash write error\n");
                    f_close(&file);
                    return TARGET_ERROR;
                }

                current_address += chunk_size;
                i += chunk_size;
            }
            address = current_address; // Update the base address for next read
        }
    }

    return TARGET_OK;
}

/* Main function to program the target */
static bool Target_Program(void)
{
    FRESULT res;
    FILINFO fileInfo;

    log_message("Target Program\n");

    /* Check if HEX file exists */
    res = f_stat(FIRMWARE_FILENAME_HEX, &fileInfo);
    if (res == FR_OK)
    {
        /* HEX file exists */
        return Target_ProgramHex();
    }
    else
    {
        /* HEX file does not exist, check for BIN file */
        res = f_stat(FIRMWARE_FILENAME_BIN, &fileInfo);
        if (res == FR_OK)
        {
            /* BIN file exists */
            return Target_ProgramBin();
        }
        else
        {
            /* Neither HEX nor BIN file exists */
            log_message("No firmware file found\n");
            return TARGET_ERROR;
        }
    }
}

static bool Target_MassErase(void)
{
	log_message("Target MassErase\n");
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

static bool Target_ProgramCallback_STM32C0(uint32_t addr, const uint8_t *buf, uint8_t bufsize)
{
	volatile uint64_t tmp[4] = {0};
	bool status = TARGET_OK;

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

static bool Target_VerifyCallback(uint32_t addr, const uint8_t *buf, uint8_t bufsize)
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

#if 0 //debug message
		log_message("Address: 0x%08"PRIX32"\n", (addr + i));
		log_message("Flash: 0x%02"PRIX16", BIN File: 0x%02"PRIX16"\n", tmp[i], buf[i]);
#endif
		if(buf[i] != tmp[i])
		{
			log_message("Verification failed at address 0x%08"PRIX32"\n", (addr + i));
			log_message("Value is 0x%02"PRIX16", should have been 0x%02"PRIX16"\n", tmp[i], buf[i]);
			return false;
		}
	}
	return true;
}

/* Function to verify HEX files */
static bool Target_VerifyHex(void)
{
    uint8_t fbuf[256];
    size_t readcount = 0;
    FRESULT res;
    FIL file;
    FILINFO fileInfo;

    /* Get file info */
    res = f_stat(FIRMWARE_FILENAME_HEX, &fileInfo);
    if(res != FR_OK)
    {
        log_message("f_stat error\n");
        return TARGET_ERROR;
    }

    /* Open the HEX file */
    res = f_open(&file, FIRMWARE_FILENAME_HEX, FA_READ);
    if(res != FR_OK)
    {
        log_message("f_open error\n");
        return TARGET_ERROR;
    }

    /* Hex parser callback registration */
    ihex_set_callback_func((ihex_callback_fp)*Target_VerifyCallback);
    ihex_reset_state();

    /* Process the HEX file */
    while (1)
    {
        /* Read from the file */
        res = f_read(&file, fbuf, sizeof(fbuf), &readcount);
        if(res != FR_OK)
        {
            log_message("f_read error\n");
            f_close(&file);
            return TARGET_ERROR;
        }

        /* End of file */
        if(readcount == 0)
        {
            res = f_close(&file);
            if(res != FR_OK)
            {
                log_message("f_close error\n");
                return TARGET_ERROR;
            }
            break;
        }
        else
        {
            /* Null-terminate if needed */
            if(readcount < sizeof(fbuf))
            {
                fbuf[readcount] = '\0';
            }

            /* Parse HEX data and verify */
            if (!ihex_parser(fbuf, readcount))
            {
                log_message("ihex_parser error\n");
                f_close(&file);
                return TARGET_ERROR;
            }
        }
    }

    return TARGET_OK;
}

/* Function to verify BIN files */
static bool Target_VerifyBin(void)
{
    uint8_t fbuf[256];
    size_t readcount = 0;
    FRESULT res;
    FIL file;
    FILINFO fileInfo;
    uint32_t address = Target_GetFlashStartAddress(); // Starting address for verification

    /* Get file info */
    res = f_stat(FIRMWARE_FILENAME_BIN, &fileInfo);
    if(res != FR_OK)
    {
        log_message("f_stat error\n");
        return TARGET_ERROR;
    }

    /* Open the BIN file */
    res = f_open(&file, FIRMWARE_FILENAME_BIN, FA_READ);
    if(res != FR_OK)
    {
        log_message("f_open error\n");
        return TARGET_ERROR;
    }

    /* Verify the BIN file */
    while (1)
    {
        /* Read from the file */
        res = f_read(&file, fbuf, sizeof(fbuf), &readcount);
        if(res != FR_OK)
        {
            log_message("f_read error\n");
            f_close(&file);
            return TARGET_ERROR;
        }

        /* End of file */
        if(readcount == 0)
        {
            res = f_close(&file);
            if(res != FR_OK)
            {
                log_message("f_close error\n");
                return TARGET_ERROR;
            }
            break;
        }
        else
        {
            size_t i = 0;

            while (i < readcount)
            {
                /* Determine the size to process */
                size_t chunk_size = readcount - i;

                /* Limit chunk size to 32 bytes */
                if(chunk_size > 32)
                    chunk_size = 32;

                /* Adjust chunk_size to be a multiple of 4 */
                size_t adjusted_chunk_size = (chunk_size / 4) * 4;

                /* If less than 4 bytes remain, handle separately */
                if(adjusted_chunk_size == 0)
                {
                    uint8_t tmp[4] = {0};
                    size_t remaining = readcount - i;
                    memcpy(tmp, &fbuf[i], remaining);

                    /* Pad remaining bytes with zeros if necessary */
                    if(!Target_VerifyCallback(address + i, tmp, 4))
                    {
                        log_message("Verification failed\n");
                        f_close(&file);
                        return TARGET_ERROR;
                    }
                    i += remaining;
                }
                else
                {
                    /* Call the Target_VerifyCallback function */
                    if(!Target_VerifyCallback(address + i, &fbuf[i], (uint8_t)adjusted_chunk_size))
                    {
                        log_message("Verification failed\n");
                        f_close(&file);
                        return TARGET_ERROR;
                    }
                    i += adjusted_chunk_size;
                }
            }

            address += readcount;
        }
    }

    return TARGET_OK;
}

/* Main function to verify the target */
static bool Target_Verify(void)
{
    FRESULT res;
    FILINFO fileInfo;

    log_message("Target Verify\n");

    /* Check if HEX file exists */
    res = f_stat(FIRMWARE_FILENAME_HEX, &fileInfo);
    if(res == FR_OK)
    {
        /* HEX file exists */
        return Target_VerifyHex();
    }
    else
    {
        /* HEX file does not exist, check for BIN file */
        res = f_stat(FIRMWARE_FILENAME_BIN, &fileInfo);
        if(res == FR_OK)
        {
            /* BIN file exists */
            return Target_VerifyBin();
        }
        else
        {
            /* Neither HEX nor BIN file exists */
            log_message("No firmware file found\n");
            return TARGET_ERROR;
        }
    }
}


/* Function to unlock protection for STM32C0 */
static bool Target_Protection_Unlock_STM32C0(void)
{
    uint32_t Option_Status = 0;

    /* Read option byte */
    Option_Status = readMem(STM32C0_FLASH_OPTION_OPTR) & STM32C0_FLASH_OPTR_RDP_Msk;
    log_message("Current target RDP = 0x%02" PRIX32 "\n", Option_Status);

    /* If option byte Level 1 */
    if(Option_Status == STM32C0_OB_RDP_LEVEL_1)
    {
        log_message("Configure target RDP Level-1(0xBB) to Level-0(0xAA) for flash programming.\n");

        /* Unlock */
        Stm32c0_Flash_Unlock();
        Stm32c0_Flash_OB_Unlock();
        Stm32c0_Flash_OB_Program(STM32C0_OB_RDP_LEVEL_0);

        log_message("RDP programming OK. Option Byte launch will generate 'system reset'\n");
        Stm32c0_Flash_OB_Launch();
        Target_Connect();
        Stm32c0_Flash_OB_Lock();
        Stm32c0_Flash_Lock();

        Option_Status = readMem(STM32C0_FLASH_OPTION_OPTR);
        log_message("Modified RDP = 0x%02" PRIX32 "\n", Option_Status & STM32C0_FLASH_OPTR_RDP_Msk);
    }
    else if(Option_Status == STM32C0_OB_RDP_LEVEL_0)
    {
        log_message("No need to configure RDP Level.\n");
    }
    else // if(Option_Status == STM32C0_OB_RDP_LEVEL_2)
    {
        log_message("Locked chip or Error, it cannot program.\n");
        return TARGET_ERROR;
    }

    return TARGET_OK;
}

/* Main function to unlock protection */
static bool Target_Protection_Unlock(void)
{
    log_message("Target protection check before flash programming.\n");

    switch(target.TargetFamily)
    {
        /* STM32C0 */
        case TARGET_STM32C0:
            return Target_Protection_Unlock_STM32C0();
            break;
        default:
            log_message("Target family not supported for protection unlock.\n");
            break;
    }
    return TARGET_OK;
}

/* Function to lock protection for STM32C0 */
static bool Target_Protection_Lock_STM32C0(void)
{
    uint32_t Option_Status = 0;

    log_message("Configuring target RDP Level-0 (0xAA) to Level-1 (0xBB) for flash protection.\n");
    Stm32c0_Flash_Unlock();
    Stm32c0_Flash_OB_Unlock();
    Stm32c0_Flash_OB_Program(STM32C0_OB_RDP_LEVEL_1);
    Stm32c0_Flash_OB_Launch();
    Target_Connect();
    Stm32c0_Flash_OB_Lock();
    Stm32c0_Flash_Lock();

    Option_Status = readMem(STM32C0_FLASH_OPTION_OPTR);
    log_message("Modified RDP = 0x%02" PRIX32 "\n", Option_Status & STM32C0_FLASH_OPTR_RDP_Msk);

    return TARGET_OK;
}

/* Main function to lock protection */
static bool Target_Protection_Lock(void)
{
    FRESULT res;
    FILINFO fileInfo;

    log_message("Target protection configure.\n");

    /* Check if the protection option file exists */
    res = f_stat(FIRMWARE_PROTECT_OPTION_FILE, &fileInfo);
    if (res == FR_OK)
    {
        log_message("Protection option available.\n");

        /* Proceed with protection lock only if the file exists */
        switch(target.TargetFamily)
        {
            /* STM32C0 */
            case TARGET_STM32C0:
                return Target_Protection_Lock_STM32C0();
                break;
            default:
                log_message("Target family not supported for protection lock.\n");
                break;
        }
    }
    else
    {
        log_message("Protection option file not found. Skipping protection lock.\n");
    }

    return TARGET_OK;
}

static void Target_ErrorHandle(bool status, const char *errorMessage)
{
    if (status != TARGET_OK)
    {
        log_message("%s\n", errorMessage);
        LED_SetState(TARGET_LED_STAT_FAILED);
        Buzzer_SetState(BUZZER_PROG_FAILED);
    }
}

void Target_MainLoop(void)
{
	bool status = TARGET_ERROR;
	int u32_StartTime = 0;
	int u32_ElasedTime = 0;

	/* Check button pushed */
	Button_Update();

	/* Button programming start */
  if(Button_WasPressed() == 1)
  {
  	LED_SetState(TARGET_LED_STAT_PROGRAMMING);
  	Buzzer_SetState(BUZZER_PROG_START);
  	u32_StartTime = HAL_GetTick();

  	/* Target flash operation */
  	status = Target_Connect();
  	Target_ErrorHandle(status, "Target Connect Error");
  	if(status != TARGET_OK) return;

  	status = Target_Protection_Unlock();
  	Target_ErrorHandle(status, "Target Protection Unlock Error");
  	if(status != TARGET_OK) return;

  	status = Target_MassErase();
  	Target_ErrorHandle(status, "Target MassErase Error");
  	if(status != TARGET_OK) return;

  	status = Target_Program();
  	Target_ErrorHandle(status, "Target Program Error");
  	if(status != TARGET_OK) return;

  	status = Target_Verify();
  	Target_ErrorHandle(status, "Target Verify Error");
  	if(status != TARGET_OK) return;

  	status = Target_Protection_Lock();
  	Target_ErrorHandle(status, "Target Protection Lock Error");
  	if(status != TARGET_OK) return;

  	log_message("Target program completed\n");

  	/* Need some debug... not working */
  	log_message("Target Application Start!\n");
  	resetTarget();
  	hardResetTarget();

    u32_ElasedTime = HAL_GetTick() - u32_StartTime;
    log_message("Total Elapsed Programming Time: %d ms\n\n", u32_ElasedTime);
    LED_SetState(TARGET_LED_STAT_COMPLETE);
    Buzzer_SetState(BUZZER_PROG_COMPLETE);
  }
}
