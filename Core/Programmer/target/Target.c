#include "Target.h"
#include "stm32c0_flash.h"
#include "stm32h7_flash.h"
#include "swd\utils.h"
#include "FileTransferCheck.h"
#include <stdarg.h>
#include <main.h>


Target_InfoTypeDef target = { .TargetChipErased = false };
extern UART_HandleTypeDef huart1;
extern RTC_HandleTypeDef hrtc;

#define PRINTF_REDIRECTION	int __io_putchar(int ch)
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
static bool Target_ProgramCallback_STM32H7(uint32_t addr, const uint8_t *buf, uint8_t bufsize);
static bool Target_ProgramCallback_STM32C0(uint32_t addr, const uint8_t *buf, uint8_t bufsize);
static bool (*Target_ProgramCallback[])(uint32_t addr, const uint8_t *buf, uint8_t bufsize)={\
					TO_BE_IMPLEMENT_CALLBACK,\
					TO_BE_IMPLEMENT_CALLBACK,\
					TO_BE_IMPLEMENT_CALLBACK,\
					TO_BE_IMPLEMENT_CALLBACK,\
					TO_BE_IMPLEMENT_CALLBACK,\
					Target_ProgramCallback_STM32C0,\
					TO_BE_IMPLEMENT_CALLBACK,\
					TO_BE_IMPLEMENT_CALLBACK,\
					TO_BE_IMPLEMENT_CALLBACK,\
					TO_BE_IMPLEMENT_CALLBACK,\
					TO_BE_IMPLEMENT_CALLBACK,\
					TO_BE_IMPLEMENT_CALLBACK,\
					TO_BE_IMPLEMENT_CALLBACK,\
					TO_BE_IMPLEMENT_CALLBACK,\
					TO_BE_IMPLEMENT_CALLBACK,\
					Target_ProgramCallback_STM32H7,\
};

static uint32_t Target_GetFlashStartAddress(void)
{
    switch(target.TargetFamily)
    {
        case TARGET_STM32C0:
            return 0x08000000;
            break;
        case TARGET_STM32F4:
            return 0x08000000;
        case TARGET_STM32H7:
            return 0x08000000;
            break;
        /* Add other target families and their start addresses as needed */
        default:
            log_message("Unknown target family. Using default start address 0x08000000.\n");
            return 0x08000000;
    }
}

static void Target_GetDeviceId_STM32C0(Target_InfoTypeDef *target)
{
    target->TargetFamily = TARGET_STM32C0;
    uint32_t tmp = readMem(STM32C0_REG_DEVICE_ID);
    target->TargetDevId = tmp & 0xFFF;
    target->TargetRevId = tmp >> 16;
}

static void Target_GetDeviceId_STM32H7(Target_InfoTypeDef *target)
{
    target->TargetFamily = TARGET_STM32H7;
    uint32_t tmp = readMem(STM32H7_REG_DEVICE_ID);
    target->TargetDevId = tmp & 0xFFF;
    target->TargetRevId = tmp >> 16;
}

static void Target_GetDviceInfo_STM32C0(Target_InfoTypeDef *target)
{
    switch(target->TargetDevId)
    {
        case STM32C0_DEV_ID_0x443:
            log_message("STM32C011xx\n");
            break;
        case STM32C0_DEV_ID_0x453:
            log_message("STM32C031xx\n");
            break;
        default:
            log_message("Unknown STM32C0 Device ID: 0x%03X\n", target->TargetDevId);
            break;
    }
}

static void Target_GetDviceInfo_STM32H7(Target_InfoTypeDef *target)
{
    switch(target->TargetDevId)
    {
        case STM32H7_DEV_ID_0x480:
            log_message("STM32H7A3/7B3/7B0\n");
            uint32_t tmp = readMem(STM32H7_FLASH_SIZE_REG2);
            target->TargetIsDualBank = (tmp == STM32H7_FLASH_SIZE_128KB) ? STM32H7_FLASH_SUPPORT_SINGLEBANK : STM32H7_FLASH_SUPPORT_DUALBANK;
            log_message("Flash Bank: %s\n", (target->TargetIsDualBank == STM32H7_FLASH_SUPPORT_DUALBANK) ? "Dual" : "Single");
            break;
        case STM32H7_DEV_ID_0x483:
            log_message("STM32H72x, STM32H73x\n");
            target->TargetIsDualBank = STM32H7_FLASH_SUPPORT_SINGLEBANK;
            log_message("Flash Bank: Single \n");
            break;
        case STM32H7_DEV_ID_0x450:
            log_message("STM32H742, STM32H743/753, STM32H750, STM32H745/755, STM32H747/757\n");
            tmp = readMem(STM32H7_FLASH_SIZE_REG1);
            target->TargetIsDualBank = (tmp == STM32H7_FLASH_SIZE_128KB) ? STM32H7_FLASH_SUPPORT_SINGLEBANK : STM32H7_FLASH_SUPPORT_DUALBANK;
            log_message("Flash Bank: %s\n", (target->TargetIsDualBank == STM32H7_FLASH_SUPPORT_DUALBANK) ? "Dual" : "Single");
            break;
        default:
            log_message("Unknown STM32H7 Device ID: 0x%03X\n", target->TargetDevId);
            break;
    }
}

static void Target_GetInfo(Target_InfoTypeDef *target)
{
    /* Target Family Classify & Read Device ID */
    switch(target->TargetDpId)
    {
        case STM32C0_SWDP_ID:
        		Target_GetDeviceId_STM32C0(target);
            break;
        case STM32H7_SWDP_ID:
        		Target_GetDeviceId_STM32H7(target);
            break;
        default:
            /* Unsupported Device */
            log_message("Unsupported Device Family\n");
            return;
    }

    /* Device Classification and Flash Bank Configuration */
    switch(target->TargetFamily)
    {
        case TARGET_STM32C0:
        		Target_GetDviceInfo_STM32C0(target);
            break;
        case TARGET_STM32H7:
        		Target_GetDviceInfo_STM32H7(target);
            break;
        default:
            log_message("Unknown Target Family\n");
            break;
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
			Target_GetInfo(&target);

			/* Reset swdErrorIndex, it cause f_open(fatfs) hang. */
			swdErrorIndex = 0;

    	/* Stop retrying */
    	return TARGET_OK;
		CATCH
    	log_message("SWD Error: %s\n", getErrorString(errorCode));
    	log_message("Failed to connect. Retrying...\n");
    	delayMs(200);
    	return TARGET_ERROR;
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

static bool Target_ProgramBin_STM32H7(uint32_t start_address, const uint8_t *data, uint32_t data_size)
{
    uint32_t address = start_address;
    uint32_t chunk_size = (target.TargetDevId == 0x480) ? 16 : 32; // 0x480이면 16바이트, 아니면 32바이트
    uint32_t remaining_size = data_size;

    while (remaining_size > 0)
    {
        // 현재 청크 크기를 결정 (남은 데이터가 chunk_size보다 적으면 남은 만큼만 처리)
				uint8_t buffer[32];
				memset(buffer, 0xFF, sizeof(buffer)); // 모든 요소를 0xFF로 설정
        uint32_t size_to_program = (remaining_size >= chunk_size) ? chunk_size : remaining_size;

        // 데이터 복사
        memcpy(buffer, data, size_to_program);

#ifdef DEBUG_USE_BIN_PROG_PRINT
        // 디버그 출력
        log_message("STM32H7 Flash: Address 0x%08lX, Data: ", address);
        for (uint32_t i = 0; i < 32; i++)
        {
        	printf("%02" PRIX16 " ", data[i]);
        }
        printf("\n");
        log_message("STM32H7 Flash: Address 0x%08lX, buffer: ", address);
        for (uint32_t i = 0; i < 32; i++)
        {
        	printf("%02" PRIX16 " ", buffer[i]);
        }
        printf("\n");
#endif

        // Stm32h7_Flash_Program 호출
        if (Stm32h7_Flash_Program(address, (uint32_t)buffer, chunk_size) != TARGET_OK)
        {
            log_message("Error: Failed to program STM32H7 flash at address 0x%08lX\n", address);
            //Stm32h7_Flash_Lock();
            return TARGET_ERROR;
        }

        // 다음 주소 및 남은 크기 갱신
        address += chunk_size;
        data += size_to_program;
        remaining_size -= size_to_program;
    }

    return TARGET_OK;
}


static bool Target_ProgramBin_STM32C0(uint32_t start_address, const uint8_t *data, uint32_t data_size)
{
    uint32_t address = start_address;
    uint32_t chunk_size = 8; // 64비트 단위로 프로그래밍
    uint32_t remaining_size = data_size;

    while (remaining_size > 0)
    {
        // 현재 청크 크기를 결정 (남은 데이터가 8바이트보다 적으면 남은 만큼만 처리)
        uint64_t word = 0xFFFFFFFFFFFFFFFF;  // 기본 패딩 값 0xFF로 초기화
        uint32_t size_to_program = (remaining_size >= chunk_size) ? chunk_size : remaining_size;

        // 데이터 복사 (패딩 필요 시 0xFF로 유지됨)
        memcpy(&word, data, size_to_program);
#ifdef DEBUG_USE_BIN_PROG_PRINT
        // 주소와 데이터 출력 (데이터는 바이트 단위로)
        log_message("STM32C0 Flash: Address 0x%08lX, Data: ", address);
        for (uint32_t i = 0; i < size_to_program; i++) {
        	printf("%02" PRIX16 " ", data[i]);
        }
        printf("\n");
#endif
        // Stm32c0_Flash_Program 호출
        if (Stm32c0_Flash_Program(address, word) != TARGET_OK)
        {
          log_message("Error: Failed to program STM32C0 flash at address 0x%08lX\n", address);
          Stm32c0_Flash_Lock();
          return TARGET_ERROR;
        }

        // 다음 주소 및 남은 크기 갱신
        address += chunk_size;
        data += size_to_program;
        remaining_size -= size_to_program;
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
    uint32_t address = Target_GetFlashStartAddress();

    log_message("Target ProgramBin\n");

    // 펌웨어 파일 정보 확인
    res = f_stat(FIRMWARE_FILENAME_BIN, &fileInfo);
    if (res != FR_OK)
    {
        log_message("Error: Firmware file not found\n");
        return TARGET_ERROR;
    }
    log_message("Firmware File: %s, Size: %lu bytes\n", fileInfo.fname, fileInfo.fsize);

    // 펌웨어 파일 열기
    res = f_open(&file, FIRMWARE_FILENAME_BIN, FA_READ);
    if (res != FR_OK)
    {
        log_message("Error: Could not open firmware file\n");
        return TARGET_ERROR;
    }

    // 파일 읽기 및 플래시 쓰기
    while (1)
    {
        // 파일 읽기
        res = f_read(&file, fbuf, sizeof(fbuf), &readcount);
        if (res != FR_OK)
        {
            log_message("Error: Failed to read firmware file\n");
            f_close(&file);
            return TARGET_ERROR;
        }

        // 파일 끝 체크
        if (readcount == 0)
        {
            f_close(&file);
            break;
        }

        // 타겟에 따라 플래시 쓰기 함수 호출
        switch (target.TargetFamily)
        {
            case TARGET_STM32C0:
                if (Target_ProgramBin_STM32C0(address, fbuf, readcount) != TARGET_OK)
                {
									 log_message("Error: STM32C0 programming failed\n");
									 f_close(&file);
									 return TARGET_ERROR;
                }
                break;

            case TARGET_STM32H7:
                if (Target_ProgramBin_STM32H7(address, fbuf, readcount) != TARGET_OK)
                {
                    log_message("Error: STM32H7 programming failed\n");
                    f_close(&file);
                    return TARGET_ERROR;
                }
                break;

            default:
                log_message("Error: Unsupported target family\n");
                f_close(&file);
                return TARGET_ERROR;
        }

        // 다음 주소 갱신
        address += readcount;
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

static void Target_MassErase_STM32C0(void)
{
    Stm32c0_Flash_Unlock();
    Stm32c0_Flash_MassErase();
    Stm32c0_Flash_Lock();
}

static void Target_MassErase_STM32H7(bool isDualBank)
{
    if (isDualBank)
    {
        Stm32h7_Flash_Unlock(STM32H7_FLASH_BANK_1);
        Stm32h7_Flash_Unlock(STM32H7_FLASH_BANK_2);
        //Stm32h7_Flash_MassErase(STM32H7_FLASH_VOLTAGE_RANGE_4, STM32H7_FLASH_BANK_BOTH);
        Stm32h7_Flash_MassErase(STM32H7_FLASH_VOLTAGE_RANGE_4, STM32H7_FLASH_BANK_1);
        Stm32h7_Flash_MassErase(STM32H7_FLASH_VOLTAGE_RANGE_4, STM32H7_FLASH_BANK_2);
        Stm32h7_Flash_Lock(STM32H7_FLASH_BANK_1);
        Stm32h7_Flash_Lock(STM32H7_FLASH_BANK_2);
    }
    else
    {
        Stm32h7_Flash_Unlock(STM32H7_FLASH_BANK_1);
        Stm32h7_Flash_MassErase(STM32H7_FLASH_VOLTAGE_RANGE_4, STM32H7_FLASH_BANK_1);
        Stm32h7_Flash_Lock(STM32H7_FLASH_BANK_1);
    }
}


static bool Target_MassErase(void)
{
	if(target.TargetChipErased  == 0)
	{
    log_message("Target MassErase\n");
    switch(target.TargetFamily)
    {
        case TARGET_STM32C0:
        		Target_MassErase_STM32C0();
            break;
        case TARGET_STM32H7:
        		Target_MassErase_STM32H7(target.TargetIsDualBank == STM32H7_FLASH_SUPPORT_DUALBANK);
            break;
        default:
            log_message("Unsupported Target Family for Mass Erase\n");
            return false;
    }
    log_message("Target MassErase Done.\n");
	}
	else
	{
		log_message("No need to mass erase(RDP Regression).\n");
		target.TargetChipErased = 0;
	}

	return TARGET_OK;
}

static bool Target_ProgramCallback_STM32C0(uint32_t addr, const uint8_t *buf, uint8_t bufsize)
{
	uint64_t tmp;
	bool status = true;

	/* Flash programming double word */
	for (uint32_t i = 0; i < bufsize; i += 8)
	{
		// tmp 초기화하여 0xFF으로 채움
		tmp = 0xFFFFFFFFFFFFFFFF;

		// 남은 바이트가 8바이트 미만일 경우, 전체를 0xFF으로 패딩 처리
		if (bufsize - i >= 8)
		{
			// 8바이트 단위로 변환
			tmp = ((uint64_t*)(&buf[i]))[0];
		} else {
			// 남은 바이트만 복사하고 나머지 부분은 0xFF로 유지
			memcpy(&tmp, &buf[i], bufsize - i);
		}

		// Double word 프로그래밍
		status = Stm32c0_Flash_Program(addr + i, tmp);
		if (status != TARGET_OK)
		{
			Stm32c0_Flash_Lock();
			return false;
		}
	}
	return true;
}

// Callback function to program the Stm32h7 flash memory
static bool Target_ProgramCallback_STM32H7(uint32_t address, const uint8_t *data, uint8_t data_size)
{
    // Assume data_size is a multiple of 2 and <= 32
    static uint8_t combined_data[32];
    static uint8_t combined_offset = 0;
    static uint32_t current_address;

    // 입력 값 검증
    if (data_size % 2 != 0 || data_size > 32) {
        //return false;
    }

    // combined_offset이 0일 때 현재 주소를 업데이트
    if (combined_offset == 0) {
        current_address = address;
    }

    // 데이터를 combined_data에 복사
    memcpy(combined_data + combined_offset, data, data_size);
    combined_offset += data_size;

    // 프로그래밍할 워드 크기 결정 (TargetDevId에 따라)
    uint8_t words_to_program = (target.TargetDevId == 0x480) ? 4 : 8;
    uint8_t bytes_to_program = words_to_program * 4;

    // 충분한 데이터가 쌓였을 때 플래시 프로그래밍 수행
    if (combined_offset >= bytes_to_program) {

        // 플래시 메모리 프로그래밍
        if (Stm32h7_Flash_Program(current_address, (uint32_t)combined_data, words_to_program) != TARGET_OK) {
            return false;
        }
        // 프로그래밍한 바이트 수만큼 주소 증가
        current_address += bytes_to_program;

        // 남은 데이터를 combined_data의 시작 부분으로 이동
        combined_offset -= bytes_to_program;
        if (combined_offset > 0) {
            memmove(combined_data, combined_data + bytes_to_program, combined_offset);
        }
    }

    // 마지막 호출이며 combined_offset이 0이 아닐 때, 남은 데이터를 패딩하여 프로그래밍
    if (data_size == 0 && combined_offset > 0) {
        // 0xFF로 패딩하여 워드 크기를 맞춤
        uint8_t padding_size = (target.TargetDevId == 0x480) ? 16 : 32;
        memset(combined_data + combined_offset, 0xFF, padding_size - combined_offset);
        combined_offset = padding_size;

        // 플래시 메모리 프로그래밍
        words_to_program = (combined_offset == 32) ? 8 : 4;
        if (Stm32h7_Flash_Program(current_address, (uint32_t)combined_data, words_to_program) != TARGET_OK) {
            return false;
        }

        // 프로그래밍한 바이트 수만큼 주소 증가
        current_address += padding_size;

        // combined_offset 초기화
        combined_offset = 0;
    }
    return true;
}

#if 1
static bool Target_VerifyCallback(uint32_t addr, const uint8_t *buf, uint8_t bufsize)
{
  uint8_t tmp[32];
  uint32_t u32_ReadData[8];

  /* Read required words from target flash memory */
  int num_words = (bufsize + 3) / 4;  // Calculate the number of words needed to cover bufsize bytes
  for(int i = 0; i < num_words; i++)
  {
      u32_ReadData[i] = readMem(addr + (i * 4));
  }

  /* Convert uint32_t to uint8_t */
  for (int i = 0; i < num_words; i++) {
      tmp[4 * i]     = u32_ReadData[i] & 0xFF;
      tmp[4 * i + 1] = (u32_ReadData[i] >> 8) & 0xFF;
      tmp[4 * i + 2] = (u32_ReadData[i] >> 16) & 0xFF;
      tmp[4 * i + 3] = (u32_ReadData[i] >> 24) & 0xFF;
  }

  /* Compare Hex & Flash Data */
  for(int i = 0; i < bufsize; i++)
  {
#if DEBUG_USE_VERIFY_PRINT
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
#else
static bool Target_VerifyCallback(uint32_t addr, const uint8_t *buf, uint8_t bufsize)
{
    uint32_t flash_words[8];  // Up to 32 bytes of data (8 words * 4 bytes per word)
    int total_words = (bufsize + 3) / 4;  // Number of 32-bit words needed

    /* Step 1: Read flash memory data */
    for (int i = 0; i < total_words; i++)
    {
        flash_words[i] = readMem(addr + (i * 4));  // Read each 32-bit word from flash
    }

    /* Step 2: Compare buffer with flash data */
    for (int i = 0; i < bufsize; i++)
    {
        // Extract the corresponding byte from flash_words
        uint8_t flash_byte = (flash_words[i / 4] >> ((i % 4) * 8)) & 0xFF;

#if DEBUG_USE_VERIFY_PRINT
        // Debug print for address and values being compared
        log_message("Address: 0x%08"PRIX32"\n", (addr + i));
        log_message("Flash: 0x%02"PRIX16", BIN File: 0x%02"PRIX16"\n", flash_byte, buf[i]);
#endif

        // Compare flash byte with buffer byte
        if (flash_byte != buf[i])
        {
            // Log mismatch details and return failure
            log_message("Verification failed at address 0x%08"PRIX32"\n", (addr + i));
            log_message("Value is 0x%02"PRIX16", should have been 0x%02"PRIX16"\n", flash_byte, buf[i]);
            return false;
        }
    }

    /* Verification successful */
    return true;
}
#endif

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

                /* Limit chunk size to 16 bytes */
                if(chunk_size > 16)
                    chunk_size = 16;

                /* Adjust chunk_size to be a multiple of 4 */
                size_t adjusted_chunk_size = (chunk_size / 4) * 4;

                /* If less than 4 bytes remain, handle separately */
                if(adjusted_chunk_size == 0)
                {
                    uint8_t tmp[4] = {0xFF, 0xFF, 0xFF, 0xFF};
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
static bool Target_Protection_Unlock_STM32H7(void)
{
    uint32_t Option_Status = 0;

    /* Read option byte */
    Option_Status = readMem(STM32H7_FLASH_OPTSR_CUR) & STM32H7_FLASH_OPTSR_RDP_Msk;
    log_message("Current target RDP = 0x%02" PRIX32 "\n", Option_Status);

    /* If option byte Level 1 */
    if(Option_Status == STM32H7_FLASH_OB_RDP_LEVEL_1)
    {
        log_message("Configure target RDP Level-1(0xBB) to Level-0(0xAA) for flash programming.\n");

        /* Unlock */
        Stm32h7_Flash_Unlock(STM32H7_FLASH_BANK_1);
        Stm32h7_Flash_Unlock(STM32H7_FLASH_BANK_2);

        Stm32h7_Flash_OB_Unlock();
        Stm32h7_Flash_OB_Program(STM32H7_FLASH_OB_RDP_LEVEL_0);

        log_message("RDP programming OK.\n");
        Stm32h7_Flash_OB_Launch();
        //Target_Connect();
        Stm32h7_Flash_OB_Lock();
        Stm32h7_Flash_Lock(STM32H7_FLASH_BANK_1);
        Stm32h7_Flash_Lock(STM32H7_FLASH_BANK_2);

        Option_Status = readMem(STM32H7_FLASH_OPTSR_CUR);
        log_message("Modified RDP = 0x%02" PRIX32 "\n", Option_Status & STM32H7_FLASH_OPTSR_RDP_Msk);
        target.TargetChipErased = 1;
    }
    else if(Option_Status == STM32H7_FLASH_OB_RDP_LEVEL_0)
    {
        log_message("No need to configure RDP Level.\n");
    }
    else // if(Option_Status == STM32H7_FLASH_OB_RDP_LEVEL_2)
    {
        log_message("Locked chip or Error, it cannot program.\n");
        return TARGET_ERROR;
    }

    return TARGET_OK;
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

        log_message("RDP programming OK.\n");
        Stm32c0_Flash_OB_Launch();
        Target_Connect();
        Stm32c0_Flash_OB_Lock();
        Stm32c0_Flash_Lock();

        Option_Status = readMem(STM32C0_FLASH_OPTION_OPTR);
        log_message("Modified RDP = 0x%02" PRIX32 "\n", Option_Status & STM32C0_FLASH_OPTR_RDP_Msk);
        target.TargetChipErased = 1;
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
        case TARGET_STM32C0:
          return Target_Protection_Unlock_STM32C0();
        case TARGET_STM32H7:
        	return Target_Protection_Unlock_STM32H7();
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

/* Function to lock protection for STM32H7 */
static bool Target_Protection_Lock_STM32H7(void)
{
    uint32_t Option_Status = 0;

    log_message("Configuring target RDP Level-0 (0xAA) to Level-1 (0xBB) for flash protection.\n");
    Stm32h7_Flash_Unlock(STM32H7_FLASH_BANK_1);
    Stm32h7_Flash_Unlock(STM32H7_FLASH_BANK_2);
    Stm32h7_Flash_OB_Unlock();
    Stm32h7_Flash_OB_Program(STM32H7_FLASH_OB_RDP_LEVEL_1);
    Stm32h7_Flash_OB_Launch();
    //Target_Connect();
    Stm32h7_Flash_OB_Lock();
    Stm32h7_Flash_Lock(STM32H7_FLASH_BANK_2);
    Stm32h7_Flash_Lock(STM32H7_FLASH_BANK_1);


    Option_Status = readMem(STM32H7_FLASH_OPTSR_CUR);
    log_message("Modified RDP = 0x%02" PRIX32 "\n", Option_Status & STM32H7_FLASH_OPTSR_RDP_Msk);

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
            case TARGET_STM32H7:
            		return Target_Protection_Lock_STM32H7();
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

static void Target_FlashUnlock(void)
{
  log_message("Target flash memory global unlock before flash programming.\n");

  switch(target.TargetFamily)
  {
      case TARGET_STM32C0:
      	Stm32c0_Flash_Unlock();
				break;
      case TARGET_STM32H7:
      		if(target.TargetIsDualBank == STM32H7_FLASH_SUPPORT_DUALBANK)
      		{
						Stm32h7_Flash_Unlock(STM32H7_FLASH_BANK_1);
						Stm32h7_Flash_Unlock(STM32H7_FLASH_BANK_2);
      		}
      		else
      		{
						Stm32h7_Flash_Unlock(STM32H7_FLASH_BANK_1);
      		}
					break;
      default:
          log_message("Not implemented.\n");
          break;
  }
}

static void Target_FlashLock(void)
{
  log_message("Target flash memory global lock after flash programming.\n");

  switch(target.TargetFamily)
  {
      case TARGET_STM32C0:
      		Stm32c0_Flash_Lock();
					break;
      case TARGET_STM32H7:
      		if(target.TargetIsDualBank == STM32H7_FLASH_SUPPORT_DUALBANK)
      		{
  					Stm32h7_Flash_Lock(STM32H7_FLASH_BANK_1);
  					Stm32h7_Flash_Lock(STM32H7_FLASH_BANK_2);
      		}
      		else
      		{
  					Stm32h7_Flash_Lock(STM32H7_FLASH_BANK_1);
      		}
					break;
      default:
          log_message("Not implemented.\n");
          break;
  }
}

void Target_MainLoop(void)
{
	bool status = TARGET_ERROR;

	int u32_StartTime = 0;
	int u32_ElasedTime = 0;

	/* Check button pushed */
	Button_Update();

	/* Check File Transfer status */
	FileTransferCheck();

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

  	Target_FlashUnlock();
  	status = Target_Program();
  	Target_FlashLock();
  	Target_ErrorHandle(status, "Target Program Error");
  	if(status != TARGET_OK) return;

  	status = Target_Verify();
  	Target_ErrorHandle(status, "Target Verify Error");
  	if(status != TARGET_OK) return;

  	status = Target_Protection_Lock();
  	Target_ErrorHandle(status, "Target Protection Lock Error");
  	if(status != TARGET_OK) return;

  	log_message("Target program completed\n");

  	log_message("Target Application Start!\n");
  	resetTarget();
  	hardResetTarget();

    u32_ElasedTime = HAL_GetTick() - u32_StartTime;
    log_message("Total Elapsed Programming Time: %d ms\n\n", u32_ElasedTime);
    LED_SetState(TARGET_LED_STAT_COMPLETE);
    Buzzer_SetState(BUZZER_PROG_COMPLETE);
  }
}
