#include "stm32h7_flash.h"
#include "utils.h"

/**
  * @brief  Wait for a FLASH operation to complete.
  * @param  Timeout maximum flash operation timeout
  * @param  Bank flash FLASH_BANK_1 or FLASH_BANK_2
  * @retval HAL_StatusTypeDef HAL Status
  */
bool Stm32h7_Flash_WaitOperation(uint32_t Timeout, uint32_t Bank)
{
  /* Determine the appropriate status register and flags based on the Bank */
  uint32_t sr_reg = (Bank == STM32H7_FLASH_BANK_1) ? STM32H7_FLASH_SR1 : STM32H7_FLASH_SR2;
  uint32_t qw_flag = (Bank == STM32H7_FLASH_BANK_1) ? STM32H7_FLASH_FLAG_QW_BANK1 : STM32H7_FLASH_FLAG_QW_BANK2;
  uint32_t all_errors_flag = (Bank == STM32H7_FLASH_BANK_1) ? STM32H7_FLASH_FLAG_ALL_ERRORS_BANK1 : STM32H7_FLASH_FLAG_ALL_ERRORS_BANK2;
  uint32_t eop_flag = (Bank == STM32H7_FLASH_BANK_1) ? STM32H7_FLASH_FLAG_EOP_BANK1 : STM32H7_FLASH_FLAG_EOP_BANK2;

  uint32_t tickstart = HAL_GetTick();
  uint32_t tmp = readMem(sr_reg) & qw_flag;

  /* Wait for the QW flag to be reset */
  while (tmp != 0U)
  {
    if (Timeout != STM32H7_MAX_DELAY)
    {
      if (((HAL_GetTick() - tickstart) > Timeout) || (Timeout == 0U))
      {
        return TARGET_ERROR;
      }
    }
    tmp = readMem(sr_reg) & qw_flag;
  }

  /* Get Error Flags */
  uint32_t errorflag = readMem(sr_reg) & all_errors_flag;

  /* Clear Error Flags with appropriate masking */
  if (Bank == STM32H7_FLASH_BANK_2)
  {
    writeMem(sr_reg, errorflag & 0x7FFFFFFFU);  // Apply mask for Bank 2
  }
  else
  {
    writeMem(sr_reg, errorflag);
  }

  /* Check FLASH End of Operation flag  */
  tmp = readMem(sr_reg) & eop_flag;
  if (tmp != 0)
  {
  	/* Clear FLASH End of Operation pending bit */
    if (Bank == FLASH_BANK_2)
    {
      writeMem(sr_reg, eop_flag & 0x7FFFFFFFU);  // Apply mask for Bank 2
    }
    else
    {
      writeMem(sr_reg, eop_flag);
    }
  }

  return TARGET_OK;
}
/**
  * @brief  Mass erase of FLASH memory
  * @param  VoltageRange The device program/erase parallelism.
  *          This parameter can be one of the following values:
  *            @arg FLASH_VOLTAGE_RANGE_1 : Flash program/erase by 8 bits
  *            @arg FLASH_VOLTAGE_RANGE_2 : Flash program/erase by 16 bits
  *            @arg FLASH_VOLTAGE_RANGE_3 : Flash program/erase by 32 bits
  *            @arg FLASH_VOLTAGE_RANGE_4 : Flash program/erase by 64 bits
  *
  * @param  Banks Banks to be erased
  *          This parameter can be one of the following values:
  *            @arg FLASH_BANK_1: Bank1 to be erased
  *            @arg FLASH_BANK_2: Bank2 to be erased
  *            @arg FLASH_BANK_BOTH: Bank1 and Bank2 to be erased
  *
  * @retval HAL Status
  */
void Stm32h7_Flash_MassErase(uint32_t VoltageRange, uint32_t Banks)
{
	uint32_t tmp = 0;

  /* Flash Mass Erase */
  if((Banks & STM32H7_FLASH_BANK_BOTH) == STM32H7_FLASH_BANK_BOTH)
  {
    /* Reset Program/erase VoltageRange for Bank1 and Bank2 */
  	tmp = readMem(STM32H7_FLASH_CR1) & (~STM32H7_FLASH_CR_PSIZE);
  	writeMem(STM32H7_FLASH_CR1, tmp);
  	tmp = readMem(STM32H7_FLASH_CR2) & (~STM32H7_FLASH_CR_PSIZE);
  	writeMem(STM32H7_FLASH_CR2, tmp);

    /* Set voltage range */
  	tmp = readMem(STM32H7_FLASH_CR1) | VoltageRange;
  	writeMem(STM32H7_FLASH_CR1, tmp);
  	tmp = readMem(STM32H7_FLASH_CR2) | VoltageRange;
  	writeMem(STM32H7_FLASH_CR2, tmp);

    /* Set Mass Erase Bit */
  	tmp = readMem(STM32H7_FLASH_OPTCR) | STM32H7_FLASH_OPTCR_MER;
  	writeMem(STM32H7_FLASH_OPTCR, tmp);
  }
  else
  {
    /* Proceed to erase Flash Bank  */
    if((Banks & STM32H7_FLASH_BANK_1) == STM32H7_FLASH_BANK_1)
    {
      /* Set Program/erase VoltageRange for Bank1 */
    	tmp = readMem(STM32H7_FLASH_CR1) & (~STM32H7_FLASH_CR_PSIZE);
    	writeMem(STM32H7_FLASH_CR1, tmp);
    	tmp = readMem(STM32H7_FLASH_CR1) | VoltageRange;
    	writeMem(STM32H7_FLASH_CR1, tmp);

      /* Erase Bank1 */
    	tmp = readMem(STM32H7_FLASH_CR1) | (STM32H7_FLASH_CR_BER | STM32H7_FLASH_CR_START);
    	writeMem(STM32H7_FLASH_CR1, tmp);
    }

    if((Banks & STM32H7_FLASH_BANK_2) == STM32H7_FLASH_BANK_2)
    {
      /* Set Program/erase VoltageRange for Bank2 */
    	tmp = readMem(STM32H7_FLASH_CR2) & (~STM32H7_FLASH_CR_PSIZE);
    	writeMem(STM32H7_FLASH_CR2, tmp);
    	tmp = readMem(STM32H7_FLASH_CR2) | VoltageRange;
    	writeMem(STM32H7_FLASH_CR2, tmp);

      /* Erase Bank2 */
    	tmp = readMem(STM32H7_FLASH_CR2) | (STM32H7_FLASH_CR_BER | STM32H7_FLASH_CR_START);
    	writeMem(STM32H7_FLASH_CR2, tmp);
    }
  }
}

bool Stm32h7_Flash_Unlock(uint32_t Banks)
{
    uint32_t tmp = 0;

    /* BANK1 언락 처리 */
    if ((Banks & STM32H7_FLASH_BANK_1) == STM32H7_FLASH_BANK_1)
    {
        tmp = readMem(STM32H7_FLASH_CR1) & STM32H7_FLASH_CR_LOCK;
        if (tmp != 0U)
        {
            /* FLASH Bank1 레지스터 접근 허용 */
            writeMem(STM32H7_FLASH_KEYR1, STM32H7_FLASH_KEY1);
            writeMem(STM32H7_FLASH_KEYR1, STM32H7_FLASH_KEY2);

            /* FLASH Bank1 언락 확인 */
            tmp = readMem(STM32H7_FLASH_CR1) & STM32H7_FLASH_CR_LOCK;
            if (tmp != 0U)
            {
                return TARGET_ERROR;
            }
        }
    }

    /* BANK2 언락 처리 */
    if ((Banks & STM32H7_FLASH_BANK_2) == STM32H7_FLASH_BANK_2)
    {
        tmp = readMem(STM32H7_FLASH_CR2) & STM32H7_FLASH_CR_LOCK;
        if (tmp != 0U)
        {
            /* FLASH Bank2 레지스터 접근 허용 */
            writeMem(STM32H7_FLASH_KEYR2, STM32H7_FLASH_KEY1);
            writeMem(STM32H7_FLASH_KEYR2, STM32H7_FLASH_KEY2);

            /* FLASH Bank2 언락 확인 */
            tmp = readMem(STM32H7_FLASH_CR2) & STM32H7_FLASH_CR_LOCK;
            if (tmp != 0U)
            {
                return TARGET_ERROR;
            }
        }
    }

    return TARGET_OK;
}

bool Stm32h7_Flash_Lock(uint32_t Banks)
{
  uint32_t tmp = 0;

	if ((Banks & STM32H7_FLASH_BANK_1) == STM32H7_FLASH_BANK_1)
	{
		tmp = readMem(STM32H7_FLASH_CR1) | STM32H7_FLASH_CR_LOCK;
		writeMem(STM32H7_FLASH_CR1, tmp);
	}

  if ((Banks & STM32H7_FLASH_BANK_2) == STM32H7_FLASH_BANK_2)
  {
		tmp = readMem(STM32H7_FLASH_CR2) | STM32H7_FLASH_CR_LOCK;
		writeMem(STM32H7_FLASH_CR2, tmp);
  }

  return TARGET_OK;
}

/**
  * @brief  Program a flash word at a specified address
  * @param  TypeProgram Indicate the way to program at a specified address.
  *         This parameter can be a value of @ref FLASH_Type_Program
  * @param  FlashAddress specifies the address to be programmed.
  *         This parameter shall be aligned to the Flash word:
  *          - 256 bits for STM32H74x/5X devices (8x 32bits words)
  *          - 128 bits for STM32H7Ax/BX devices (4x 32bits words)
  *          - 256 bits for STM32H72x/3X devices (8x 32bits words)
  * @param  DataAddress specifies the address of data to be programmed.
  *         This parameter shall be 32-bit aligned
  *
  * @retval HAL_StatusTypeDef HAL Status
  */
bool Stm32h7_Flash_Program(uint32_t FlashAddress, uint32_t DataAddress, uint32_t WordSize)
{
	bool status;
  uint32_t bank;
  uint32_t dest_addr = FlashAddress;
  uint32_t *src_addr = ( uint32_t*)DataAddress;
  uint8_t row_index = WordSize;
  uint32_t tmp;

  /* Check the parameters */
  if(IS_STM32H7_FLASH_PROGRAM_ADDRESS_BANK1(FlashAddress))
  {
    bank = STM32H7_FLASH_BANK_1;
  }
  else if(IS_STM32H7_FLASH_PROGRAM_ADDRESS_BANK2(FlashAddress))
  {
    bank = STM32H7_FLASH_BANK_2;
  }
  else
  {
    return TARGET_ERROR;
  }

  /* Wait for last operation to be completed */
  status = Stm32h7_Flash_WaitOperation((uint32_t)STM32H7_FLASH_TIMEOUT_VALUE, bank);

  /* Set PG bit */
  if(status == TARGET_OK)
  {
    if(bank == STM32H7_FLASH_BANK_1)
    {
    	tmp = readMem(STM32H7_FLASH_CR1) | STM32H7_FLASH_CR_PG;
    	writeMem(STM32H7_FLASH_CR1, tmp);
    }
    else
    {
    	tmp = readMem(STM32H7_FLASH_CR2) | STM32H7_FLASH_CR_PG;
    	writeMem(STM32H7_FLASH_CR2, tmp);
    }
  }
  /* Program the flash word */
  do
  {
  	writeMem(dest_addr, *src_addr++);
  	dest_addr+=4;
  	row_index--;
  }while (row_index != 0U);
  status = Stm32h7_Flash_WaitOperation((uint32_t)STM32H7_FLASH_TIMEOUT_VALUE, bank);

  /* If the program operation is completed, disable the PG */
  if(bank == STM32H7_FLASH_BANK_1)
  {
  	tmp = readMem(STM32H7_FLASH_CR1);
  	writeMem(STM32H7_FLASH_CR1, tmp & (~STM32H7_FLASH_CR_PG));

  }
  else
  {
  	tmp = readMem(STM32H7_FLASH_CR2);
  	writeMem(STM32H7_FLASH_CR2, tmp & (~STM32H7_FLASH_CR_PG));
  }

  return TARGET_OK;
}

