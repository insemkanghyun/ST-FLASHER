/*
 * stm32c4_flash.h
 *
 *  Created on: Jan 24, 2025
 *      Author: kangh
 */

#ifndef PROGRAMMER_TARGET_STM32G4_FLASH_H_
#define PROGRAMMER_TARGET_STM32G4_FLASH_H_

#include "target.h"
#include <stdbool.h>

/**
  * @brief FLASH Registers
  */
#define STM32G4_PERIPH_BASE			(0x40000000UL)
#define STM32G4_AHBPERIPH_BASE		(STM32G4_PERIPH_BASE + 0x00020000UL)
#define STM32G4_FLASH_R_BASE		(STM32G4_AHBPERIPH_BASE + 0x00002000UL)
#define STM32G4_FLASH               (STM32G4_FLASH_R_BASE)
#define STM32C4_FLASH_CR			(STM32G4_FLASH) + 0x14
#define STM32C4_FLASH_SR			(STM32G4_FLASH) + 0x10
#define STM32C4_FLASH_KEYR			(STM32G4_FLASH) + 0x08
#define STM32C4_FLASH_OPTR			(STM32G4_FLASH) + 0x20
#define STM32C4_FLASH_OPTKEYR		(STM32G4_FLASH) + 0x0C
#define STM32C4_FLASH_OPTION_OPTR	0x1FFFF800

#define STM32G4_FLASH_TIMEOUT_VALUE	1000U          /*!< FLASH Execution Timeout, 1 s */

/** @defgroup FLASH_Keys FLASH Keys
  * @{
  */
#define STM32G4_FLASH_KEY1                      0x45670123U   /*!< Flash key1 */
#define STM32G4_FLASH_KEY2                      0xCDEF89ABU   /*!< Flash key2: used with FLASH_KEY1
                                                           to unlock the FLASH registers access */
#define STM32G4_FLASH_OPTKEY1                   0x08192A3BU   /*!< Flash option byte key1 */
#define STM32G4_FLASH_OPTKEY2                   0x4C5D6E7FU   /*!< Flash option byte key2: used with FLASH_OPTKEY1
                                                           to allow option bytes operations */
/*******************  Bits definition for FLASH_CR register  ******************/
#define STM32G4_FLASH_CR_PG_Pos                   (0U)
#define STM32G4_FLASH_CR_PG_Msk                   (0x1UL << STM32G4_FLASH_CR_PG_Pos)           /*!< 0x00000001 */
#define STM32G4_FLASH_CR_PG                       STM32G4_FLASH_CR_PG_Msk
#define STM32G4_FLASH_CR_PER_Pos                  (1U)
#define STM32G4_FLASH_CR_PER_Msk                  (0x1UL << STM32G4_FLASH_CR_PER_Pos)          /*!< 0x00000002 */
#define STM32G4_FLASH_CR_PER                      STM32G4_FLASH_CR_PER_Msk
#define STM32G4_FLASH_CR_MER1_Pos                 (2U)
#define STM32G4_FLASH_CR_MER1_Msk                 (0x1UL << STM32G4_FLASH_CR_MER1_Pos)         /*!< 0x00000004 */
#define STM32G4_FLASH_CR_MER1                     STM32G4_FLASH_CR_MER1_Msk
#define STM32G4_FLASH_CR_PNB_Pos                  (3U)
#define STM32G4_FLASH_CR_PNB_Msk                  (0x3FUL << STM32G4_FLASH_CR_PNB_Pos)         /*!< 0x000001F8 */
#define STM32G4_FLASH_CR_PNB                      STM32G4_FLASH_CR_PNB_Msk
#define STM32G4_FLASH_CR_STRT_Pos                 (16U)
#define STM32G4_FLASH_CR_STRT_Msk                 (0x1UL << STM32G4_FLASH_CR_STRT_Pos)         /*!< 0x00010000 */
#define STM32G4_FLASH_CR_STRT                     STM32G4_FLASH_CR_STRT_Msk
#define STM32G4_FLASH_CR_OPTSTRT_Pos              (17U)
#define STM32G4_FLASH_CR_OPTSTRT_Msk              (0x1UL << STM32G4_FLASH_CR_OPTSTRT_Pos)      /*!< 0x00020000 */
#define STM32G4_FLASH_CR_OPTSTRT                  STM32G4_FLASH_CR_OPTSTRT_Msk
#define STM32G4_FLASH_CR_FSTPG_Pos                (18U)
#define STM32G4_FLASH_CR_FSTPG_Msk                (0x1UL << STM32G4_FLASH_CR_FSTPG_Pos)        /*!< 0x00040000 */
#define STM32G4_FLASH_CR_FSTPG                    STM32G4_FLASH_CR_FSTPG_Msk
#define STM32G4_FLASH_CR_EOPIE_Pos                (24U)
#define STM32G4_FLASH_CR_EOPIE_Msk                (0x1UL << STM32G4_FLASH_CR_EOPIE_Pos)        /*!< 0x01000000 */
#define STM32G4_FLASH_CR_EOPIE                    STM32G4_FLASH_CR_EOPIE_Msk
#define STM32G4_FLASH_CR_ERRIE_Pos                (25U)
#define STM32G4_FLASH_CR_ERRIE_Msk                (0x1UL << STM32G4_FLASH_CR_ERRIE_Pos)        /*!< 0x02000000 */
#define STM32G4_FLASH_CR_ERRIE                    STM32G4_FLASH_CR_ERRIE_Msk
#define STM32G4_FLASH_CR_RDERRIE_Pos              (26U)
#define STM32G4_FLASH_CR_RDERRIE_Msk              (0x1UL << STM32G4_FLASH_CR_RDERRIE_Pos)      /*!< 0x04000000 */
#define STM32G4_FLASH_CR_RDERRIE                  STM32G4_FLASH_CR_RDERRIE_Msk
#define STM32G4_FLASH_CR_OBL_LAUNCH_Pos           (27U)
#define STM32G4_FLASH_CR_OBL_LAUNCH_Msk           (0x1UL << STM32G4_FLASH_CR_OBL_LAUNCH_Pos)   /*!< 0x08000000 */
#define STM32G4_FLASH_CR_OBL_LAUNCH               STM32G4_FLASH_CR_OBL_LAUNCH_Msk
#define STM32G4_FLASH_CR_SEC_PROT1_Pos            (28U)
#define STM32G4_FLASH_CR_SEC_PROT1_Msk            (0x1UL << STM32G4_FLASH_CR_SEC_PROT1_Pos)    /*!< 0x10000000 */
#define STM32G4_FLASH_CR_SEC_PROT1                STM32G4_FLASH_CR_SEC_PROT1_Msk
#define STM32G4_FLASH_CR_OPTLOCK_Pos              (30U)
#define STM32G4_FLASH_CR_OPTLOCK_Msk              (0x1UL << STM32G4_FLASH_CR_OPTLOCK_Pos)      /*!< 0x40000000 */
#define STM32G4_FLASH_CR_OPTLOCK                  STM32G4_FLASH_CR_OPTLOCK_Msk
#define STM32G4_FLASH_CR_LOCK_Pos                 (31U)
#define STM32G4_FLASH_CR_LOCK_Msk                 (0x1UL << STM32G4_FLASH_CR_LOCK_Pos)         /*!< 0x80000000 */
#define STM32G4_FLASH_CR_LOCK                     STM32G4_FLASH_CR_LOCK_Msk

/*******************  Bits definition for FLASH_SR register  ******************/
#define STM32G4_FLASH_SR_EOP_Pos                  (0U)
#define STM32G4_FLASH_SR_EOP_Msk                  (0x1UL << STM32G4_FLASH_SR_EOP_Pos)          /*!< 0x00000001 */
#define STM32G4_FLASH_SR_EOP                      STM32G4_FLASH_SR_EOP_Msk
#define STM32G4_FLASH_SR_OPERR_Pos                (1U)
#define STM32G4_FLASH_SR_OPERR_Msk                (0x1UL << STM32G4_FLASH_SR_OPERR_Pos)        /*!< 0x00000002 */
#define STM32G4_FLASH_SR_OPERR                    STM32G4_FLASH_SR_OPERR_Msk
#define STM32G4_FLASH_SR_PROGERR_Pos              (3U)
#define STM32G4_FLASH_SR_PROGERR_Msk              (0x1UL << STM32G4_FLASH_SR_PROGERR_Pos)      /*!< 0x00000008 */
#define STM32G4_FLASH_SR_PROGERR                  STM32G4_FLASH_SR_PROGERR_Msk
#define STM32G4_FLASH_SR_WRPERR_Pos               (4U)
#define STM32G4_FLASH_SR_WRPERR_Msk               (0x1UL << STM32G4_FLASH_SR_WRPERR_Pos)       /*!< 0x00000010 */
#define STM32G4_FLASH_SR_WRPERR                   STM32G4_FLASH_SR_WRPERR_Msk
#define STM32G4_FLASH_SR_PGAERR_Pos               (5U)
#define STM32G4_FLASH_SR_PGAERR_Msk               (0x1UL << STM32G4_FLASH_SR_PGAERR_Pos)       /*!< 0x00000020 */
#define STM32G4_FLASH_SR_PGAERR                   STM32G4_FLASH_SR_PGAERR_Msk
#define STM32G4_FLASH_SR_SIZERR_Pos               (6U)
#define STM32G4_FLASH_SR_SIZERR_Msk               (0x1UL << STM32G4_FLASH_SR_SIZERR_Pos)       /*!< 0x00000040 */
#define STM32G4_FLASH_SR_SIZERR                   STM32G4_FLASH_SR_SIZERR_Msk
#define STM32G4_FLASH_SR_PGSERR_Pos               (7U)
#define STM32G4_FLASH_SR_PGSERR_Msk               (0x1UL << STM32G4_FLASH_SR_PGSERR_Pos)       /*!< 0x00000080 */
#define STM32G4_FLASH_SR_PGSERR                   STM32G4_FLASH_SR_PGSERR_Msk
#define STM32G4_FLASH_SR_MISERR_Pos               (8U)
#define STM32G4_FLASH_SR_MISERR_Msk               (0x1UL << STM32G4_FLASH_SR_MISERR_Pos)       /*!< 0x00000100 */
#define STM32G4_FLASH_SR_MISERR                   STM32G4_FLASH_SR_MISERR_Msk
#define STM32G4_FLASH_SR_FASTERR_Pos              (9U)
#define STM32G4_FLASH_SR_FASTERR_Msk              (0x1UL << STM32G4_FLASH_SR_FASTERR_Pos)      /*!< 0x00000200 */
#define STM32G4_FLASH_SR_FASTERR                  STM32G4_FLASH_SR_FASTERR_Msk
#define STM32G4_FLASH_SR_RDERR_Pos                (14U)
#define STM32G4_FLASH_SR_RDERR_Msk                (0x1UL << STM32G4_FLASH_SR_RDERR_Pos)        /*!< 0x00004000 */
#define STM32G4_FLASH_SR_RDERR                    STM32G4_FLASH_SR_RDERR_Msk
#define STM32G4_FLASH_SR_OPTVERR_Pos              (15U)
#define STM32G4_FLASH_SR_OPTVERR_Msk              (0x1UL << STM32G4_FLASH_SR_OPTVERR_Pos)      /*!< 0x00008000 */
#define STM32G4_FLASH_SR_OPTVERR                  STM32G4_FLASH_SR_OPTVERR_Msk
#define STM32G4_FLASH_SR_BSY_Pos                  (16U)
#define STM32G4_FLASH_SR_BSY_Msk                  (0x1UL << STM32G4_FLASH_SR_BSY_Pos)          /*!< 0x00010000 */
#define STM32G4_FLASH_SR_BSY                      STM32G4_FLASH_SR_BSY_Msk

/*******************  Bits definition for FLASH_OPTR register  ***************/
#define STM32G4_FLASH_OPTR_RDP_Pos                (0U)
#define STM32G4_FLASH_OPTR_RDP_Msk                (0xFFUL << STM32G4_FLASH_OPTR_RDP_Pos)       /*!< 0x000000FF */
#define STM32G4_FLASH_OPTR_RDP                    STM32G4_FLASH_OPTR_RDP_Msk
#define STM32G4_FLASH_OPTR_BOR_LEV_Pos            (8U)
#define STM32G4_FLASH_OPTR_BOR_LEV_Msk            (0x7UL << STM32G4_FLASH_OPTR_BOR_LEV_Pos)    /*!< 0x00000700 */
#define STM32G4_FLASH_OPTR_BOR_LEV                STM32G4_FLASH_OPTR_BOR_LEV_Msk
#define STM32G4_FLASH_OPTR_BOR_LEV_0              (0x0UL << STM32G4_FLASH_OPTR_BOR_LEV_Pos)    /*!< 0x00000000 */
#define STM32G4_FLASH_OPTR_BOR_LEV_1              (0x1UL << STM32G4_FLASH_OPTR_BOR_LEV_Pos)    /*!< 0x00000100 */
#define STM32G4_FLASH_OPTR_BOR_LEV_2              (0x2UL << STM32G4_FLASH_OPTR_BOR_LEV_Pos)    /*!< 0x00000200 */
#define STM32G4_FLASH_OPTR_BOR_LEV_3              (0x3UL << STM32G4_FLASH_OPTR_BOR_LEV_Pos)    /*!< 0x00000300 */
#define STM32G4_FLASH_OPTR_BOR_LEV_4              (0x4UL << STM32G4_FLASH_OPTR_BOR_LEV_Pos)    /*!< 0x00000400 */
#define STM32G4_FLASH_OPTR_nRST_STOP_Pos          (12U)
#define STM32G4_FLASH_OPTR_nRST_STOP_Msk          (0x1UL << STM32G4_FLASH_OPTR_nRST_STOP_Pos)  /*!< 0x00001000 */
#define STM32G4_FLASH_OPTR_nRST_STOP              STM32G4_FLASH_OPTR_nRST_STOP_Msk
#define STM32G4_FLASH_OPTR_nRST_STDBY_Pos         (13U)
#define STM32G4_FLASH_OPTR_nRST_STDBY_Msk         (0x1UL << STM32G4_FLASH_OPTR_nRST_STDBY_Pos) /*!< 0x00002000 */
#define STM32G4_FLASH_OPTR_nRST_STDBY             STM32G4_FLASH_OPTR_nRST_STDBY_Msk
#define STM32G4_FLASH_OPTR_nRST_SHDW_Pos          (14U)
#define STM32G4_FLASH_OPTR_nRST_SHDW_Msk          (0x1UL << STM32G4_FLASH_OPTR_nRST_SHDW_Pos)  /*!< 0x00004000 */
#define STM32G4_FLASH_OPTR_nRST_SHDW              STM32G4_FLASH_OPTR_nRST_SHDW_Msk
#define STM32G4_FLASH_OPTR_IWDG_SW_Pos            (16U)
#define STM32G4_FLASH_OPTR_IWDG_SW_Msk            (0x1UL << STM32G4_FLASH_OPTR_IWDG_SW_Pos)    /*!< 0x00010000 */
#define STM32G4_FLASH_OPTR_IWDG_SW                STM32G4_FLASH_OPTR_IWDG_SW_Msk
#define STM32G4_FLASH_OPTR_IWDG_STOP_Pos          (17U)
#define STM32G4_FLASH_OPTR_IWDG_STOP_Msk          (0x1UL << STM32G4_FLASH_OPTR_IWDG_STOP_Pos)  /*!< 0x00020000 */
#define STM32G4_FLASH_OPTR_IWDG_STOP              STM32G4_FLASH_OPTR_IWDG_STOP_Msk
#define STM32G4_FLASH_OPTR_IWDG_STDBY_Pos         (18U)
#define STM32G4_FLASH_OPTR_IWDG_STDBY_Msk         (0x1UL << STM32G4_FLASH_OPTR_IWDG_STDBY_Pos) /*!< 0x00040000 */
#define STM32G4_FLASH_OPTR_IWDG_STDBY             STM32G4_FLASH_OPTR_IWDG_STDBY_Msk
#define STM32G4_FLASH_OPTR_WWDG_SW_Pos            (19U)
#define STM32G4_FLASH_OPTR_WWDG_SW_Msk            (0x1UL << STM32G4_FLASH_OPTR_WWDG_SW_Pos)    /*!< 0x00080000 */
#define STM32G4_FLASH_OPTR_WWDG_SW                STM32G4_FLASH_OPTR_WWDG_SW_Msk
#define STM32G4_FLASH_OPTR_nBOOT1_Pos             (23U)
#define STM32G4_FLASH_OPTR_nBOOT1_Msk             (0x1UL << STM32G4_FLASH_OPTR_nBOOT1_Pos)     /*!< 0x00800000 */
#define STM32G4_FLASH_OPTR_nBOOT1                 STM32G4_FLASH_OPTR_nBOOT1_Msk
#define STM32G4_FLASH_OPTR_SRAM_PE_Pos            (24U)
#define STM32G4_FLASH_OPTR_SRAM_PE_Msk            (0x1UL << STM32G4_FLASH_OPTR_SRAM_PE_Pos)    /*!< 0x01000000 */
#define STM32G4_FLASH_OPTR_SRAM_PE                STM32G4_FLASH_OPTR_SRAM_PE_Msk
#define STM32G4_FLASH_OPTR_CCMSRAM_RST_Pos        (25U)
#define STM32G4_FLASH_OPTR_CCMSRAM_RST_Msk        (0x1UL << STM32G4_FLASH_OPTR_CCMSRAM_RST_Pos)/*!< 0x02000000 */
#define STM32G4_FLASH_OPTR_CCMSRAM_RST            STM32G4_FLASH_OPTR_CCMSRAM_RST_Msk
#define STM32G4_FLASH_OPTR_nSWBOOT0_Pos           (26U)
#define STM32G4_FLASH_OPTR_nSWBOOT0_Msk           (0x1UL << STM32G4_FLASH_OPTR_nSWBOOT0_Pos)   /*!< 0x04000000 */
#define STM32G4_FLASH_OPTR_nSWBOOT0               STM32G4_FLASH_OPTR_nSWBOOT0_Msk
#define STM32G4_FLASH_OPTR_nBOOT0_Pos             (27U)
#define STM32G4_FLASH_OPTR_nBOOT0_Msk             (0x1UL << STM32G4_FLASH_OPTR_nBOOT0_Pos)     /*!< 0x08000000 */
#define STM32G4_FLASH_OPTR_nBOOT0                 STM32G4_FLASH_OPTR_nBOOT0_Msk
#define STM32G4_FLASH_OPTR_NRST_MODE_Pos          (28U)
#define STM32G4_FLASH_OPTR_NRST_MODE_Msk          (0x3UL << STM32G4_FLASH_OPTR_NRST_MODE_Pos)  /*!< 0x30000000 */
#define STM32G4_FLASH_OPTR_NRST_MODE              STM32G4_FLASH_OPTR_NRST_MODE_Msk
#define STM32G4_FLASH_OPTR_NRST_MODE_0            (0x1UL << STM32G4_FLASH_OPTR_NRST_MODE_Pos)  /*!< 0x10000000 */
#define STM32G4_FLASH_OPTR_NRST_MODE_1            (0x2UL << STM32G4_FLASH_OPTR_NRST_MODE_Pos)  /*!< 0x20000000 */
#define STM32G4_FLASH_OPTR_IRHEN_Pos              (30U)
#define STM32G4_FLASH_OPTR_IRHEN_Msk              (0x1UL << STM32G4_FLASH_OPTR_IRHEN_Pos)      /*!< 0x40000000 */
#define STM32G4_FLASH_OPTR_IRHEN                  STM32G4_FLASH_OPTR_IRHEN_Msk

/** @defgroup FLASH_OB_Read_Protection FLASH Option Bytes Read Protection
  * @{
  */
#define STM32G4_OB_RDP_LEVEL_0            0xAAU
#define STM32G4_OB_RDP_LEVEL_1            0xBBU
#define STM32G4_OB_RDP_LEVEL_2            0xCCU                    /*!< Warning: When enabling read protection level 2
                                                                it's no more possible to go back to level 1 or 0 */

/** @defgroup FLASH_Flags FLASH Flags Definition
  * @{
  */
#define STM32G4_FLASH_FLAG_EOP            STM32G4_FLASH_SR_EOP             /*!< STM32G4_FLASH End of operation flag */
#define STM32G4_FLASH_FLAG_OPERR          STM32G4_FLASH_SR_OPERR           /*!< STM32G4_FLASH Operation error flag */
#define STM32G4_FLASH_FLAG_PROGERR        STM32G4_FLASH_SR_PROGERR         /*!< STM32G4_FLASH Programming error flag */
#define STM32G4_FLASH_FLAG_WRPERR         STM32G4_FLASH_SR_WRPERR          /*!< STM32G4_FLASH Write protection error flag */
#define STM32G4_FLASH_FLAG_PGAERR         STM32G4_FLASH_SR_PGAERR          /*!< STM32G4_FLASH Programming alignment error flag */
#define STM32G4_FLASH_FLAG_SIZERR         STM32G4_FLASH_SR_SIZERR          /*!< STM32G4_FLASH Size error flag  */
#define STM32G4_FLASH_FLAG_PGSERR         STM32G4_FLASH_SR_PGSERR          /*!< STM32G4_FLASH Programming sequence error flag */
#define STM32G4_FLASH_FLAG_MISERR         STM32G4_FLASH_SR_MISERR          /*!< STM32G4_FLASH Fast programming data miss error flag */
#define STM32G4_FLASH_FLAG_FASTERR        STM32G4_FLASH_SR_FASTERR         /*!< STM32G4_FLASH Fast programming error flag */
#define STM32G4_FLASH_FLAG_RDERR          STM32G4_FLASH_SR_RDERR           /*!< STM32G4_FLASH PCROP read error flag */
#define STM32G4_FLASH_FLAG_OPTVERR        STM32G4_FLASH_SR_OPTVERR         /*!< STM32G4_FLASH Option validity error flag  */
#define STM32G4_FLASH_FLAG_BSY            STM32G4_FLASH_SR_BSY             /*!< STM32G4_FLASH Busy flag */
#define STM32G4_FLASH_FLAG_ECCC           STM32G4_FLASH_ECCR_ECCC          /*!< STM32G4_FLASH ECC correction in 64 LSB bits */
#define STM32G4_FLASH_FLAG_ECCD           STM32G4_FLASH_ECCR_ECCD          /*!< STM32G4_FLASH ECC detection in 64 LSB bits */
#if defined (STM32G4_FLASH_OPTR_DBANK)
#define STM32G4_FLASH_FLAG_ECCC2          STM32G4_FLASH_ECCR_ECCC2         /*!< STM32G4_FLASH ECC correction in 64 MSB bits (mode 128 bits only) */
#define STM32G4_FLASH_FLAG_ECCD2          STM32G4_FLASH_ECCR_ECCD2         /*!< STM32G4_FLASH ECC detection in 64 MSB bits (mode 128 bits only) */
#endif

#define STM32G4_FLASH_FLAG_SR_ERRORS      (STM32G4_FLASH_FLAG_OPERR   | STM32G4_FLASH_FLAG_PROGERR | STM32G4_FLASH_FLAG_WRPERR | \
                                   STM32G4_FLASH_FLAG_PGAERR  | STM32G4_FLASH_FLAG_SIZERR  | STM32G4_FLASH_FLAG_PGSERR | \
                                   STM32G4_FLASH_FLAG_MISERR  | STM32G4_FLASH_FLAG_FASTERR | STM32G4_FLASH_FLAG_RDERR  | \
                                   STM32G4_FLASH_FLAG_OPTVERR)

bool Stm32g4_Flash_MassErase(void);
bool Stm32g4_Flash_Program(uint32_t Address, uint64_t Data);
bool Stm32g4_Flash_Unlock(void);
bool Stm32g4_Flash_Lock(void);
bool Stm32g4_Flash_WaitOperation(uint32_t Timeout);
bool Stm32g4_Flash_OB_Lock(void);
bool Stm32g4_Flash_OB_Unlock(void);
uint32_t Stm32g4_Flash_OB_GetUser(void);
void Stm32g4_Flash_OB_OptrConfig(uint32_t UserType, uint32_t UserConfig, uint32_t RDPLevel);
void Stm32g4_Flash_OB_Launch(void);
bool Stm32g4_Flash_OB_Program(uint32_t RDPLevel);

#endif /* PROGRAMMER_TARGET_STM32G4_FLASH_H_ */
