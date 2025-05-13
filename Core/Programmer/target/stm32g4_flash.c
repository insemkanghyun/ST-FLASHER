#include <stdint.h>
#include "main.h"
#include "stm32g4_flash.h"
#include "utils.h"

/**
  * @brief  Perform a mass erase.
  * @retval Target Status
  */
bool Stm32g4_Flash_MassErase(void)
{
	bool status;

	return  status;
}
bool Stm32g4_Flash_Program(uint32_t Address, uint64_t Data);
bool Stm32g4_Flash_Unlock(void);
bool Stm32g4_Flash_Lock(void);

bool Stm32g4_Flash_WaitOperation(uint32_t Timeout)
{
	  uint32_t error;
	  uint32_t tmp = 0;

	  return TARGET_OK;
}

bool Stm32g4_Flash_OB_Lock(void);
bool Stm32g4_Flash_OB_Unlock(void);
uint32_t Stm32g4_Flash_OB_GetUser(void);
void Stm32g4_Flash_OB_OptrConfig(uint32_t UserType, uint32_t UserConfig, uint32_t RDPLevel);
void Stm32g4_Flash_OB_Launch(void);
bool Stm32g4_Flash_OB_Program(uint32_t RDPLevel);
