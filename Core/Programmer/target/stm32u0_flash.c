#include <stdint.h>
#include "main.h"
#include "stm32u0_flash.h"
#include "target.h"
#include "utils.h"

/**
  * @brief  Perform a mass erase.
  * @retval Target Status
  */
bool Stm32u0_Flash_MassErase(void)
{
	bool status;
	uint32_t tmp = 0;

	/* Wait for last operation to be completed */
	status = Stm32u0_Flash_WaitOperation(STM32U0_FLASH_TIMEOUT_VALUE);

	if(status == TARGET_OK)
	{
	  /* Set the Mass Erase Bit and start bit */
		tmp = readMem(STM32U0_FLASH_CR);
		writeMem(STM32U0_FLASH_CR, (STM32U0_FLASH_CR_STRT | STM32U0_FLASH_CR_MER1)|tmp);
		status = Stm32u0_Flash_WaitOperation(STM32U0_FLASH_TIMEOUT_VALUE);
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
bool Stm32u0_Flash_Program(uint32_t Address, uint64_t Data)
{
	bool status;
	uint32_t tmp = 0;

	/* Wait for last operation to be completed */
	//Remove for fast programming
	status = Stm32u0_Flash_WaitOperation(STM32U0_FLASH_TIMEOUT_VALUE);

	if (status == TARGET_OK)
	{
		/* Set PG bit */
		tmp = readMem(STM32U0_FLASH_CR);
		writeMem(STM32U0_FLASH_CR, STM32U0_FLASH_CR_PG|tmp);

		/* Program first word */
		writeMem(Address, 	(uint32_t)Data);

		/* Program second word */
		writeMem(Address+4, (uint32_t)(Data>>32));
	}

	/* Wait for last operation to be completed */
	status = Stm32u0_Flash_WaitOperation(STM32U0_FLASH_TIMEOUT_VALUE);

	/* If the program operation is completed, disable the PG or FSTPG Bit */
	tmp = readMem(STM32U0_FLASH_CR);
	writeMem(STM32U0_FLASH_CR, tmp & (~STM32U0_FLASH_CR_PG));

	/* return status */
	return status;
}

/**
  * @brief  Unlock the FLASH control register access.
  * @retval Target Status
  */
bool Stm32u0_Flash_Unlock(void)
{
	bool status = TARGET_OK;
	uint32_t tmp = 0;

	tmp = readMem(STM32U0_FLASH_CR)& STM32U0_FLASH_CR_LOCK;
	if(tmp != 0x00U)
	{
		/* Authorize the FLASH Registers access */
		writeMem(STM32U0_FLASH_KEYR, STM32U0_FLASH_KEY1);
		writeMem(STM32U0_FLASH_KEYR, STM32U0_FLASH_KEY2);

		/* verify Flash is unlock */
		tmp = readMem(STM32U0_FLASH_CR)& STM32U0_FLASH_CR_LOCK;
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
bool Stm32u0_Flash_Lock(void)
{
	bool status = TARGET_ERROR;
	uint32_t tmp = 0;

	/* Set the LOCK Bit to lock the FLASH Registers access */
	tmp = readMem(STM32U0_FLASH_CR);
	writeMem(STM32U0_FLASH_CR, STM32U0_FLASH_CR_LOCK|tmp);

	/* verify Flash is locked */
	tmp = readMem(STM32U0_FLASH_CR) & STM32U0_FLASH_CR_LOCK;
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
bool Stm32u0_Flash_WaitOperation(uint32_t Timeout)
{
	  uint32_t error;
	  uint32_t tmp = 0;

	  /* Wait for the FLASH operation to complete by polling on BUSY flag to be reset.
	     Even if the FLASH operation fails, the BUSY flag will be reset and an error
	     flag will be set */
	  uint32_t timeout = HAL_GetTick() + Timeout;

	  /* Wait if any operation is ongoing */
	  tmp = readMem(STM32U0_FLASH_SR) & STM32U0_FLASH_SR_BSY1;
	  while(tmp != 0x00U)
	  {
	    if (HAL_GetTick() >= timeout)
	    {
	      return TARGET_ERROR;
	    }
	    tmp = readMem(STM32U0_FLASH_SR) & STM32U0_FLASH_SR_BSY1;
	  }

	  /* check flash errors */
	  error = readMem(STM32U0_FLASH_SR) & STM32U0_FLASH_SR_ERRORS;

	  /* clear error flags */
	  writeMem(STM32U0_FLASH_SR, error);

	  /* Wait for control register to be written */
	  timeout = HAL_GetTick() + Timeout;
	  tmp = readMem(STM32U0_FLASH_SR) & STM32U0_FLASH_SR_CFGBSY;
	  while(tmp != 0x00U)
	  {
	    if (HAL_GetTick() >= timeout)
	    {
	      return TARGET_ERROR;
	    }
	    tmp = readMem(STM32U0_FLASH_SR) & STM32U0_FLASH_SR_CFGBSY;
	  }

	  return TARGET_OK;
}


/**
  * @brief  Lock the FLASH Option Bytes Registers access.
  * @retval HAL Status
  */
bool Stm32u0_Flash_OB_Lock(void)
{
	bool status = TARGET_ERROR;
	uint32_t tmp = 0;

	/* Set the OPTLOCK Bit to lock the FLASH Option Byte Registers access */
	tmp = readMem(STM32U0_FLASH_CR);
	writeMem(STM32U0_FLASH_CR, STM32U0_FLASH_CR_OPTLOCK|tmp);

	/* verify option bytes are locked */
	tmp = readMem(STM32U0_FLASH_CR) & STM32U0_FLASH_CR_OPTLOCK;
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
bool Stm32u0_Flash_OB_Unlock(void)
{
	bool status = TARGET_ERROR;
	uint32_t tmp = 0;

	tmp = readMem(STM32U0_FLASH_CR)& STM32U0_FLASH_CR_OPTLOCK;
	if(tmp != 0x00U)
	{
		/* Authorizes the Option Byte register programming */
		writeMem(STM32U0_FLASH_OPTKEYR, STM32U0_FLASH_OPTKEY1);
		writeMem(STM32U0_FLASH_OPTKEYR, STM32U0_FLASH_OPTKEY2);

		/* verify option bytes are unlocked */
		tmp = readMem(STM32U0_FLASH_CR)& STM32U0_FLASH_CR_OPTLOCK;
		if(tmp == 0x00U)
    {
      status = TARGET_OK;
    }
	}
	return status;
}

/**
  * @brief  Return the FLASH User Option Byte value.
  * @retval The FLASH User Option Bytes values. It will be a combination of all the following values:
  *         @arg @ref OB_USER_BOR_ENABLE,
  *         @ref OB_USER_BOR_LEVEL,
  *         @ref OB_USER_nRST_STOP,
  *         @ref OB_USER_nRST_STANDBY,
  *         @ref OB_USER_nRST_SHUTDOWN,
  *         @ref OB_USER_IWDG_SW,
  *         @ref OB_USER_IWDG_STOP,
  *         @ref OB_USER_IWDG_STANDBY,
  *         @ref OB_USER_WWDG_SW,
  *         @ref OB_USER_SRAM_PARITY,
  *         @ref OB_USER_nBOOT_SEL,
  *         @ref OB_USER_nBOOT1,
  *         @ref OB_USER_nBOOT0,
  *         @ref OB_USER_INPUT_RESET_HOLDER
  *         @ref OB_USER_SECURE_MUXING_EN
  *         @ref OB_USER_HSE_NOT_REMAPPED (*)
  *
  * @note   (*) available only on STM32C071xx devices.
  */
uint32_t Stm32u0_Flash_OB_GetUser(void)
{
	uint32_t tmp = 0;
	tmp = readMem(STM32U0_FLASH_OPTR);
	uint32_t user = ((tmp & ~STM32U0_FLASH_OPTR_RDP) & STM32U0_OB_USER_ALL);
	return user;
}

/**
  * @brief  Set user & RDP configuration
  * @note   !!! Warning : When enabling OB_RDP level 2 it is no more possible
  *         to go back to level 1 or 0 !!!
  * @param  UserType  The FLASH User Option Bytes to be modified.
  *         This parameter can be a combination of @ref FLASH_OB_USER_Type
  * @param  UserConfig  The FLASH User Option Bytes values.
  *         This parameter can be a combination of:
  *         @arg @ref OB_USER_BOR_ENABLE,
  *         @arg @ref OB_USER_BOR_LEVEL,
  *         @arg @ref OB_USER_nRST_STOP,
  *         @arg @ref OB_USER_nRST_STANDBY,
  *         @arg @ref OB_USER_nRST_SHUTDOWN,
  *         @arg @ref OB_USER_IWDG_SW,
  *         @arg @ref OB_USER_IWDG_STOP,
  *         @arg @ref OB_USER_IWDG_STANDBY,
  *         @arg @ref OB_USER_WWDG_SW,
  *         @arg @ref OB_USER_SRAM_PARITY,
  *         @arg @ref OB_USER_nBOOT_SEL,
  *         @arg @ref OB_USER_nBOOT1,
  *         @arg @ref OB_USER_nBOOT0,
  *         @arg @ref OB_USER_INPUT_RESET_HOLDER
  *         @arg @ref OB_USER_SECURE_MUXING_EN
  *         @arg @ref OB_USER_HSE_NOT_REMAPPED (*)
  * @param  RDPLevel  specifies the read protection level.
  *         This parameter can be one of the following values:
  *            @arg @ref OB_RDP_LEVEL_0 No protection
  *            @arg @ref OB_RDP_LEVEL_1 Memory Read protection
  *            @arg @ref OB_RDP_LEVEL_2 Full chip protection
  * @retval None
  *
  * @note   (*) available only on STM32C071xx devices.
  */
void Stm32u0_Flash_OB_OptrConfig(uint32_t UserType, uint32_t UserConfig, uint32_t RDPLevel)
{
	uint32_t optr;
	uint32_t tmp = 0;;

	/* Configure the RDP level in the option bytes register */
	optr = readMem(STM32U0_FLASH_OPTR);
	optr &= ~(UserType | STM32U0_FLASH_OPTR_RDP);	// Clear old configurations.
	tmp = optr | UserConfig | RDPLevel;				// Set new configurations.
	writeMem(STM32U0_FLASH_OPTR, tmp);
}

/**
  * @brief  Launch the option byte loading.
  * @retval HAL Status
  */

void Stm32u0_Flash_OB_Launch(void)
{
	uint32_t tmp = 0;

  /* Set the bit to force the option byte reloading */
	/* Option byte launch generates Option byte reset */
	tmp = readMem(STM32U0_FLASH_CR);
	writeMem(STM32U0_FLASH_CR, STM32U0_FLASH_CR_OBL_LAUNCH|tmp);
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
bool Stm32u0_Flash_OB_Program(uint32_t RDPLevel)
{
	uint32_t optr;
	bool status;
	uint32_t tmp = 0;

	/* Only modify RDP so get current user data */
	optr = Stm32u0_Flash_OB_GetUser();
	Stm32u0_Flash_OB_OptrConfig(optr, optr, RDPLevel);

	/* Wait for last operation to be completed */
	status = Stm32u0_Flash_WaitOperation(STM32U0_FLASH_TIMEOUT_VALUE);

	if(status == TARGET_OK)
	{
    /* Set OPTSTRT Bit */
		tmp = readMem(STM32U0_FLASH_CR);
		writeMem(STM32U0_FLASH_CR, STM32U0_FLASH_CR_OPTSTRT|tmp);

    /* Wait for last operation to be completed */
    status = Stm32u0_Flash_WaitOperation(STM32U0_FLASH_TIMEOUT_VALUE);

    /* If the option byte program operation is completed, disable the OPTSTRT Bit */
    tmp = readMem(STM32U0_FLASH_CR);
    writeMem(STM32U0_FLASH_CR, ~STM32U0_FLASH_CR_OPTSTRT&tmp);
	}

	return status;
}
