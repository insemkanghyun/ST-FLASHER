#include <stdint.h>
#include "main.h"
#include "stm32g0_flash.h"
#include "target.h"
#include "utils.h"

/**
  * @brief  Perform a mass erase.
  * @retval Target Status
  */
bool Stm32g0_Flash_MassErase(void)
{
	bool status;
	uint32_t tmp = 0;

	/* Wait for last operation to be completed */
	status = Stm32g0_Flash_WaitOperation(STM32G0_FLASH_TIMEOUT_VALUE);

	if(status == TARGET_OK)
	{
	  /* Set the Mass Erase Bit and start bit */
		tmp = readMem(STM32G0_FLASH_CR);
		writeMem(STM32G0_FLASH_CR, (STM32G0_FLASH_CR_STRT | STM32G0_FLASH_CR_MER1)|tmp);
		status = Stm32g0_Flash_WaitOperation(STM32G0_FLASH_TIMEOUT_VALUE);
	}
	return  status;
}

/**
  * @brief  Program double word of a row at a specified address.
  * @param  Address Specifies the address to be programmed.
  * @param  Data Specifies the data to be programmed
  *               This parameter is the data for the double word program.
  * @retval bool Target Status
  */
bool Stm32g0_Flash_Program(uint32_t Address, uint64_t Data)
{
	bool status;
	uint32_t tmp = 0;

	/* Wait for last operation to be completed */
	//Remove for fast programming
	status = Stm32g0_Flash_WaitOperation(STM32G0_FLASH_TIMEOUT_VALUE);
		//status = TARGET_OK;

	if (status == TARGET_OK)
	{
		/* Set PG bit */
		tmp = readMem(STM32G0_FLASH_CR);
		writeMem(STM32G0_FLASH_CR, STM32G0_FLASH_CR_PG|tmp);

		/* Program first word */
		writeMem(Address, 	(uint32_t)Data);

		/* Program second word */
		writeMem(Address+4, (uint32_t)(Data>>32));
	}

  	/* Wait for last operation to be completed */
  	status = Stm32g0_Flash_WaitOperation(STM32G0_FLASH_TIMEOUT_VALUE);

    /* If the program operation is completed, disable the PG or FSTPG Bit */
  	tmp = readMem(STM32G0_FLASH_CR);
  	writeMem(STM32G0_FLASH_CR, tmp & (~STM32G0_FLASH_CR_PG));

	/* return status */
	return status;
}

/**
  * @brief  Unlock the FLASH control register access.
  * @retval Target Status
  */
bool Stm32g0_Flash_Unlock(void)
{
	bool status = TARGET_OK;
	uint32_t tmp = 0;

	tmp = readMem(STM32G0_FLASH_CR)& STM32G0_FLASH_CR_LOCK;
	if(tmp != 0x00U)
	{
    /* Authorize the FLASH Registers access */
		writeMem(STM32G0_FLASH_KEYR, STM32G0_FLASH_KEY1);
		writeMem(STM32G0_FLASH_KEYR, STM32G0_FLASH_KEY2);

		/* verify Flash is unlock */
		tmp = readMem(STM32G0_FLASH_CR)& STM32G0_FLASH_CR_LOCK;
		if(tmp != 0x00U)
    {
      status = TARGET_ERROR;
    }
	}
	return status;
}

/**
  * @brief  Lock the FLASH control register access.
  * @retval Target Status
  */
bool Stm32g0_Flash_Lock(void)
{
	bool status = TARGET_ERROR;
	uint32_t tmp = 0;

	/* Wait for last operation to be completed */
	Stm32g0_Flash_WaitOperation(STM32G0_FLASH_TIMEOUT_VALUE);

	/* Set the LOCK Bit to lock the FLASH Registers access */
	tmp = readMem(STM32G0_FLASH_CR);
	writeMem(STM32G0_FLASH_CR, STM32G0_FLASH_CR_LOCK|tmp);

	/* verify Flash is locked */
	tmp = readMem(STM32G0_FLASH_CR) & STM32G0_FLASH_CR_LOCK;
	if(tmp != 0x00U)
	{
		status = TARGET_OK;
	}
	return status;
}

/**
  * @brief  Wait for a FLASH operation to complete.
  * @param  Timeout maximum flash operation timeout
  * @retval bool Status
  */
bool Stm32g0_Flash_WaitOperation(uint32_t Timeout)
{
  uint32_t error;
  uint32_t tmp = 0;

  /* Wait for the FLASH operation to complete by polling on BUSY flag to be reset.
     Even if the FLASH operation fails, the BUSY flag will be reset and an error
     flag will be set */
  uint32_t timeout = HAL_GetTick() + Timeout;

  /* Wait if any operation is ongoing */
  tmp = readMem(STM32G0_FLASH_SR) & STM32G0_FLASH_FLAG_BSY;
  while(tmp != 0x00U)
  {
    if (HAL_GetTick() >= timeout)
    {
      return TARGET_ERROR;
    }
    tmp = readMem(STM32G0_FLASH_SR) & STM32G0_FLASH_FLAG_BSY;
  }

  /* check flash errors */
  error = readMem(STM32G0_FLASH_SR) & STM32G0_FLASH_SR_ERRORS;

  /* clear error flags */
  writeMem(STM32G0_FLASH_SR, error);

  /* Wait for control register to be written */
  timeout = HAL_GetTick() + Timeout;
  tmp = readMem(STM32G0_FLASH_SR) & STM32G0_FLASH_FLAG_CFGBSY;
  while(tmp != 0x00U)
  {
    if (HAL_GetTick() >= timeout)
    {
      return TARGET_ERROR;
    }
    tmp = readMem(STM32G0_FLASH_SR) & STM32G0_FLASH_FLAG_CFGBSY;
  }

  return TARGET_OK;
}


/**
  * @brief  Lock the FLASH Option Bytes Registers access.
  * @retval HAL Status
  */
bool Stm32g0_Flash_OB_Lock(void)
{
	bool status = TARGET_ERROR;
	uint32_t tmp = 0;

	/* Wait for last operation to be completed */
	Stm32g0_Flash_WaitOperation(STM32G0_FLASH_TIMEOUT_VALUE);

	/* Set the OPTLOCK Bit to lock the FLASH Option Byte Registers access */
	tmp = readMem(STM32G0_FLASH_CR);
	writeMem(STM32G0_FLASH_CR, STM32G0_FLASH_CR_OPTLOCK|tmp);

	/* verify option bytes are locked */
	tmp = readMem(STM32G0_FLASH_CR) & STM32G0_FLASH_CR_OPTLOCK;
	if(tmp != 0x00U)
	{
		status = TARGET_OK;
	}
	return status;
}

/**
  * @brief  Unlock the FLASH Option Bytes Registers access.
  * @retval HAL Status
  */
bool Stm32g0_Flash_OB_Unlock(void)
{
	bool status = TARGET_ERROR;
	uint32_t tmp = 0;

	tmp = readMem(STM32G0_FLASH_CR)& STM32G0_FLASH_CR_OPTLOCK;
	if(tmp != 0x00U)
	{
		/* Authorizes the Option Byte register programming */
		writeMem(STM32G0_FLASH_OPTKEYR, STM32G0_FLASH_OPTKEY1);
		writeMem(STM32G0_FLASH_OPTKEYR, STM32G0_FLASH_OPTKEY2);

		/* verify option bytes are unlocked */
		tmp = readMem(STM32G0_FLASH_CR)& STM32G0_FLASH_CR_OPTLOCK;
		if(tmp == 0x00U)
		{
		  status = TARGET_OK;
		}
	}
	return status;
}

uint32_t Stm32g0_Flash_OB_GetRDP(void)
{
    // Return only the RDP field value from OPTR.
    // Caller can compare with 0xAA/0xBB/0xCC to determine level.
    uint32_t optr = readMem(STM32G0_FLASH_OPTR);
    return optr & STM32G0_FLASH_OPTR_RDP_Msk;
}

/**
  * @brief  Launch the option byte loading.
  * @retval HAL Status
  */
void Stm32g0_Flash_OB_Launch(void)
{
    // Trigger an option byte reload which causes an immediate system reset.
    // After this write, target will reset and SWD must reconnect.
    uint32_t cr = readMem(STM32G0_FLASH_CR);
    writeMem(STM32G0_FLASH_CR, cr | STM32G0_FLASH_CR_OBL_LAUNCH);
}

/**
  * @brief  Program Option bytes.
  * @param  pOBInit Pointer to an @ref FLASH_OBProgramInitTypeDef structure that
  *         contains the configuration information for the programming.
  * @note   To configure any option bytes, the option lock bit OPTLOCK must be
  *         cleared with the call of @ref HAL_FLASH_OB_Unlock() function.
  * @note   New option bytes configuration will be taken into account only
  *         - after an option bytes launch through the call of @ref HAL_FLASH_OB_Launch()
  *         - a Power On Reset
  *         - an exit from Standby or Shutdown mode.
  * @retval HAL Status
  */
bool Stm32g0_Flash_OB_Program(uint32_t RDPLevel)
{
	bool status;
	uint32_t tmp = 0;

    uint32_t optr = readMem(STM32G0_FLASH_OPTR);
    optr = (optr & ~STM32G0_FLASH_OPTR_RDP_Msk) | (RDPLevel & STM32G0_FLASH_OPTR_RDP_Msk);
    writeMem(STM32G0_FLASH_OPTR, optr);

	/* Wait for last operation to be completed */
	status = Stm32g0_Flash_WaitOperation(STM32G0_FLASH_TIMEOUT_VALUE);

	if(status == TARGET_OK)
	{
		/* Set OPTSTRT Bit */
		tmp = readMem(STM32G0_FLASH_CR);
		writeMem(STM32G0_FLASH_CR, STM32G0_FLASH_CR_OPTSTRT|tmp);

		/* Wait for last operation to be completed */
		status = Stm32g0_Flash_WaitOperation(STM32G0_FLASH_TIMEOUT_VALUE);

		/* If the option byte program operation is completed, disable the OPTSTRT Bit */
		tmp = readMem(STM32G0_FLASH_CR);
		writeMem(STM32G0_FLASH_CR, ~STM32G0_FLASH_CR_OPTSTRT&tmp);
	}
    return status;
}
