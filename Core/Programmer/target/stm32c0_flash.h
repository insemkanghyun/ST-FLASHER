#ifndef STM32C0_FLASH_H_
#define STM32C0_FLASH_H_

#include "target.h"
#include <stdbool.h>

/**
  * @brief FLASH Address
  */
#define STM32C0_FLASH_START_ADDRESS						0x08000000UL

/**
  * @brief FLASH Registers
  */
#define STM32C0_PERIPH_BASE										(0x40000000UL)
#define STM32C0_AHBPERIPH_BASE								(STM32C0_PERIPH_BASE + 0x00020000UL)
#define STM32C0_FLASH_R_BASE									(STM32C0_AHBPERIPH_BASE + 0x00002000UL)
#define STM32C0_FLASH               					(STM32C0_FLASH_R_BASE)
#define STM32C0_FLASH_CR											(STM32C0_FLASH_R_BASE) + 0x14
#define STM32C0_FLASH_SR											(STM32C0_FLASH_R_BASE) + 0x10
#define STM32C0_FLASH_KEYR										(STM32C0_FLASH_R_BASE) + 0x08
#define STM32C0_FLASH_OPTR										(STM32C0_FLASH_R_BASE) + 0x20
#define STM32C0_FLASH_OPTKEYR									(STM32C0_FLASH_R_BASE) + 0x0C
#define STM32C0_FLASH_OPTION_OPTR							0x1FFF7800

#define STM32C0_FLASH_TIMEOUT_VALUE						1000U          /*!< FLASH Execution Timeout, 1 s */

/** @defgroup FLASH_Keys FLASH Keys
  * @{
  */
#define STM32C0_FLASH_KEY1                      0x45670123U   /*!< Flash key1 */
#define STM32C0_FLASH_KEY2                      0xCDEF89ABU   /*!< Flash key2: used with FLASH_KEY1
                                                           to unlock the FLASH registers access */
#define STM32C0_FLASH_OPTKEY1                   0x08192A3BU   /*!< Flash option byte key1 */
#define STM32C0_FLASH_OPTKEY2                   0x4C5D6E7FU   /*!< Flash option byte key2: used with FLASH_OPTKEY1
                                                           to allow option bytes operations */

/*******************  Bits definition for FLASH_CR register  ******************/
#define STM32C0_FLASH_CR_PG_Pos                        (0U)
#define STM32C0_FLASH_CR_PG_Msk                        (0x1UL << STM32C0_FLASH_CR_PG_Pos)          /*!< 0x00000001 */
#define STM32C0_FLASH_CR_PG                            STM32C0_FLASH_CR_PG_Msk
#define STM32C0_FLASH_CR_PER_Pos                       (1U)
#define STM32C0_FLASH_CR_PER_Msk                       (0x1UL << STM32C0_FLASH_CR_PER_Pos)         /*!< 0x00000002 */
#define STM32C0_FLASH_CR_PER                           STM32C0_FLASH_CR_PER_Msk
#define STM32C0_FLASH_CR_MER1_Pos                      (2U)
#define STM32C0_FLASH_CR_MER1_Msk                      (0x1UL << STM32C0_FLASH_CR_MER1_Pos)        /*!< 0x00000004 */
#define STM32C0_FLASH_CR_MER1                          STM32C0_FLASH_CR_MER1_Msk
#define STM32C0_FLASH_CR_PNB_Pos                       (3U)
#define STM32C0_FLASH_CR_PNB_Msk                       (0xFUL << STM32C0_FLASH_CR_PNB_Pos)        /*!< 0x000001F8 */
#define STM32C0_FLASH_CR_PNB                           STM32C0_FLASH_CR_PNB_Msk
#define STM32C0_FLASH_CR_STRT_Pos                      (16U)
#define STM32C0_FLASH_CR_STRT_Msk                      (0x1UL << STM32C0_FLASH_CR_STRT_Pos)        /*!< 0x00010000 */
#define STM32C0_FLASH_CR_STRT                          STM32C0_FLASH_CR_STRT_Msk
#define STM32C0_FLASH_CR_OPTSTRT_Pos                   (17U)
#define STM32C0_FLASH_CR_OPTSTRT_Msk                   (0x1UL << STM32C0_FLASH_CR_OPTSTRT_Pos)     /*!< 0x00020000 */
#define STM32C0_FLASH_CR_OPTSTRT                       STM32C0_FLASH_CR_OPTSTRT_Msk
#define STM32C0_FLASH_CR_FSTPG_Pos                     (18U)
#define STM32C0_FLASH_CR_FSTPG_Msk                     (0x1UL << STM32C0_FLASH_CR_FSTPG_Pos)       /*!< 0x00040000 */
#define STM32C0_FLASH_CR_FSTPG                         STM32C0_FLASH_CR_FSTPG_Msk
#define STM32C0_FLASH_CR_EOPIE_Pos                     (24U)
#define STM32C0_FLASH_CR_EOPIE_Msk                     (0x1UL << STM32C0_FLASH_CR_EOPIE_Pos)       /*!< 0x01000000 */
#define STM32C0_FLASH_CR_EOPIE                         STM32C0_FLASH_CR_EOPIE_Msk
#define STM32C0_FLASH_CR_ERRIE_Pos                     (25U)
#define STM32C0_FLASH_CR_ERRIE_Msk                     (0x1UL << STM32C0_FLASH_CR_ERRIE_Pos)       /*!< 0x02000000 */
#define STM32C0_FLASH_CR_ERRIE                         STM32C0_FLASH_CR_ERRIE_Msk
#define STM32C0_FLASH_CR_RDERRIE_Pos                   (26U)
#define STM32C0_FLASH_CR_RDERRIE_Msk                   (0x1UL << STM32C0_FLASH_CR_RDERRIE_Pos)     /*!< 0x04000000 */
#define STM32C0_FLASH_CR_RDERRIE                       STM32C0_FLASH_CR_RDERRIE_Msk
#define STM32C0_FLASH_CR_OBL_LAUNCH_Pos                (27U)
#define STM32C0_FLASH_CR_OBL_LAUNCH_Msk                (0x1UL << STM32C0_FLASH_CR_OBL_LAUNCH_Pos)  /*!< 0x08000000 */
#define STM32C0_FLASH_CR_OBL_LAUNCH                    STM32C0_FLASH_CR_OBL_LAUNCH_Msk
#define STM32C0_FLASH_CR_SEC_PROT_Pos                  (28U)
#define STM32C0_FLASH_CR_SEC_PROT_Msk                  (0x1UL << STM32C0_FLASH_CR_SEC_PROT_Pos)    /*!< 0x10000000 */
#define STM32C0_FLASH_CR_SEC_PROT                      STM32C0_FLASH_CR_SEC_PROT_Msk
#define STM32C0_FLASH_CR_OPTLOCK_Pos                   (30U)
#define STM32C0_FLASH_CR_OPTLOCK_Msk                   (0x1UL << STM32C0_FLASH_CR_OPTLOCK_Pos)     /*!< 0x40000000 */
#define STM32C0_FLASH_CR_OPTLOCK                       STM32C0_FLASH_CR_OPTLOCK_Msk
#define STM32C0_FLASH_CR_LOCK_Pos                      (31U)
#define STM32C0_FLASH_CR_LOCK_Msk                      (0x1UL << STM32C0_FLASH_CR_LOCK_Pos)        /*!< 0x80000000 */
#define STM32C0_FLASH_CR_LOCK                          STM32C0_FLASH_CR_LOCK_Msk


/*******************  Bits definition for FLASH_SR register  ******************/
#define STM32C0_FLASH_SR_EOP_Pos                       (0U)
#define STM32C0_FLASH_SR_EOP_Msk                       (0x1UL << STM32C0_FLASH_SR_EOP_Pos)         /*!< 0x00000001 */
#define STM32C0_FLASH_SR_EOP                           STM32C0_FLASH_SR_EOP_Msk
#define STM32C0_FLASH_SR_OPERR_Pos                     (1U)
#define STM32C0_FLASH_SR_OPERR_Msk                     (0x1UL << STM32C0_FLASH_SR_OPERR_Pos)       /*!< 0x00000002 */
#define STM32C0_FLASH_SR_OPERR                         STM32C0_FLASH_SR_OPERR_Msk
#define STM32C0_FLASH_SR_PROGERR_Pos                   (3U)
#define STM32C0_FLASH_SR_PROGERR_Msk                   (0x1UL << STM32C0_FLASH_SR_PROGERR_Pos)     /*!< 0x00000008 */
#define STM32C0_FLASH_SR_PROGERR                       STM32C0_FLASH_SR_PROGERR_Msk
#define STM32C0_FLASH_SR_WRPERR_Pos                    (4U)
#define STM32C0_FLASH_SR_WRPERR_Msk                    (0x1UL << STM32C0_FLASH_SR_WRPERR_Pos)      /*!< 0x00000010 */
#define STM32C0_FLASH_SR_WRPERR                        STM32C0_FLASH_SR_WRPERR_Msk
#define STM32C0_FLASH_SR_PGAERR_Pos                    (5U)
#define STM32C0_FLASH_SR_PGAERR_Msk                    (0x1UL << STM32C0_FLASH_SR_PGAERR_Pos)      /*!< 0x00000020 */
#define STM32C0_FLASH_SR_PGAERR                        STM32C0_FLASH_SR_PGAERR_Msk
#define STM32C0_FLASH_SR_SIZERR_Pos                    (6U)
#define STM32C0_FLASH_SR_SIZERR_Msk                    (0x1UL << STM32C0_FLASH_SR_SIZERR_Pos)      /*!< 0x00000040 */
#define STM32C0_FLASH_SR_SIZERR                        STM32C0_FLASH_SR_SIZERR_Msk
#define STM32C0_FLASH_SR_PGSERR_Pos                    (7U)
#define STM32C0_FLASH_SR_PGSERR_Msk                    (0x1UL << STM32C0_FLASH_SR_PGSERR_Pos)      /*!< 0x00000080 */
#define STM32C0_FLASH_SR_PGSERR                        STM32C0_FLASH_SR_PGSERR_Msk
#define STM32C0_FLASH_SR_MISERR_Pos                    (8U)
#define STM32C0_FLASH_SR_MISERR_Msk                    (0x1UL << STM32C0_FLASH_SR_MISERR_Pos)      /*!< 0x00000100 */
#define STM32C0_FLASH_SR_MISERR                        STM32C0_FLASH_SR_MISERR_Msk
#define STM32C0_FLASH_SR_FASTERR_Pos                   (9U)
#define STM32C0_FLASH_SR_FASTERR_Msk                   (0x1UL << STM32C0_FLASH_SR_FASTERR_Pos)     /*!< 0x00000200 */
#define STM32C0_FLASH_SR_FASTERR                       STM32C0_FLASH_SR_FASTERR_Msk
#define STM32C0_FLASH_SR_RDERR_Pos                     (14U)
#define STM32C0_FLASH_SR_RDERR_Msk                     (0x1UL << STM32C0_FLASH_SR_RDERR_Pos)       /*!< 0x00004000 */
#define STM32C0_FLASH_SR_RDERR                         STM32C0_FLASH_SR_RDERR_Msk
#define STM32C0_FLASH_SR_OPTVERR_Pos                   (15U)
#define STM32C0_FLASH_SR_OPTVERR_Msk                   (0x1UL << STM32C0_FLASH_SR_OPTVERR_Pos)     /*!< 0x00008000 */
#define STM32C0_FLASH_SR_OPTVERR                       STM32C0_FLASH_SR_OPTVERR_Msk
#define STM32C0_FLASH_SR_BSY1_Pos                      (16U)
#define STM32C0_FLASH_SR_BSY1_Msk                      (0x1UL << STM32C0_FLASH_SR_BSY1_Pos)        /*!< 0x00010000 */
#define STM32C0_FLASH_SR_BSY1                          STM32C0_FLASH_SR_BSY1_Msk
#define STM32C0_FLASH_SR_CFGBSY_Pos                    (18U)
#define STM32C0_FLASH_SR_CFGBSY_Msk                    (0x1UL << STM32C0_FLASH_SR_CFGBSY_Pos)      /*!< 0x00040000 */
#define STM32C0_FLASH_SR_CFGBSY                        STM32C0_FLASH_SR_CFGBSY_Msk


/** @defgroup FLASH_Flags FLASH Flags Definition
  * @{
  */
#define STM32C0_FLASH_FLAG_EOP                  STM32C0_FLASH_SR_EOP      /*!< FLASH End of operation flag */
#define STM32C0_FLASH_FLAG_OPERR                STM32C0_FLASH_SR_OPERR    /*!< FLASH Operation error flag */
#define STM32C0_FLASH_FLAG_PROGERR              STM32C0_FLASH_SR_PROGERR  /*!< FLASH Programming error flag */
#define STM32C0_FLASH_FLAG_WRPERR               STM32C0_FLASH_SR_WRPERR   /*!< FLASH Write protection error flag */
#define STM32C0_FLASH_FLAG_PGAERR               STM32C0_FLASH_SR_PGAERR   /*!< FLASH Programming alignment error flag */
#define STM32C0_FLASH_FLAG_SIZERR               STM32C0_FLASH_SR_SIZERR   /*!< FLASH Size error flag  */
#define STM32C0_FLASH_FLAG_PGSERR               STM32C0_FLASH_SR_PGSERR   /*!< FLASH Programming sequence error flag */
#define STM32C0_FLASH_FLAG_MISERR               STM32C0_FLASH_SR_MISERR   /*!< FLASH Fast programming data miss error flag */
#define STM32C0_FLASH_FLAG_FASTERR              STM32C0_FLASH_SR_FASTERR  /*!< FLASH Fast programming error flag */
#define STM32C0_FLASH_FLAG_RDERR                STM32C0_FLASH_SR_RDERR    /*!< FLASH PCROP read error flag */
#define STM32C0_FLASH_FLAG_OPTVERR              STM32C0_FLASH_SR_OPTVERR  /*!< FLASH Option validity error flag */
#define STM32C0_FLASH_FLAG_BSY                  STM32C0_FLASH_SR_BSY1     /*!< FLASH Operation Busy flag */
#define STM32C0_FLASH_FLAG_CFGBSY               STM32C0_FLASH_SR_CFGBSY   /*!< FLASH Configuration Busy flag */
#define STM32C0_FLASH_FLAG_PESD                 STM32C0_FLASH_SR_PESD     /*!< FLASH Programming/erase operation suspended */

#define STM32C0_FLASH_FLAG_ALL_ERRORS           (STM32C0_FLASH_FLAG_OPERR   | STM32C0_FLASH_FLAG_PROGERR | STM32C0_FLASH_FLAG_WRPERR | \
                                         STM32C0_FLASH_FLAG_PGAERR  | STM32C0_FLASH_FLAG_SIZERR  | STM32C0_FLASH_FLAG_PGSERR | \
                                         STM32C0_FLASH_FLAG_MISERR  | STM32C0_FLASH_FLAG_FASTERR | STM32C0_FLASH_FLAG_RDERR  | \
                                         STM32C0_FLASH_FLAG_OPTVERR)


#define STM32C0_FLASH_FLAG_SR_ERROR             (STM32C0_FLASH_FLAG_OPERR  | STM32C0_FLASH_FLAG_PROGERR | STM32C0_FLASH_FLAG_WRPERR |  \
		STM32C0_FLASH_FLAG_PGAERR | STM32C0_FLASH_FLAG_SIZERR  | STM32C0_FLASH_FLAG_PGSERR |   \
		STM32C0_FLASH_FLAG_MISERR | STM32C0_FLASH_FLAG_FASTERR | STM32C0_FLASH_FLAG_RDERR |   \
		STM32C0_FLASH_FLAG_OPTVERR)     /*!< All SR error flags */

/*******************  Bits definition for FLASH_OPTR register  ****************/
#define STM32C0_FLASH_OPTR_RDP_Pos                     (0U)
#define STM32C0_FLASH_OPTR_RDP_Msk                     (0xFFUL << STM32C0_FLASH_OPTR_RDP_Pos)        /*!< 0x000000FF */
#define STM32C0_FLASH_OPTR_RDP                         STM32C0_FLASH_OPTR_RDP_Msk
#define STM32C0_FLASH_OPTR_BOR_EN_Pos                  (8U)
#define STM32C0_FLASH_OPTR_BOR_EN_Msk                  (0x1UL << STM32C0_FLASH_OPTR_BOR_EN_Pos)      /*!< 0x00000100 */
#define STM32C0_FLASH_OPTR_BOR_EN                      STM32C0_FLASH_OPTR_BOR_EN_Msk
#define STM32C0_FLASH_OPTR_BORR_LEV_Pos                (9U)
#define STM32C0_FLASH_OPTR_BORR_LEV_Msk                (0x3UL << STM32C0_FLASH_OPTR_BORR_LEV_Pos)    /*!< 0x00000600 */
#define STM32C0_FLASH_OPTR_BORR_LEV                    STM32C0_FLASH_OPTR_BORR_LEV_Msk
#define STM32C0_FLASH_OPTR_BORR_LEV_0                  (0x1UL << STM32C0_FLASH_OPTR_BORR_LEV_Pos)    /*!< 0x00000200 */
#define STM32C0_FLASH_OPTR_BORR_LEV_1                  (0x2UL << STM32C0_FLASH_OPTR_BORR_LEV_Pos)    /*!< 0x00000400 */
#define STM32C0_FLASH_OPTR_BORF_LEV_Pos                (11U)
#define STM32C0_FLASH_OPTR_BORF_LEV_Msk                (0x3UL << STM32C0_FLASH_OPTR_BORF_LEV_Pos)    /*!< 0x00001800 */
#define STM32C0_FLASH_OPTR_BORF_LEV                    STM32C0_FLASH_OPTR_BORF_LEV_Msk
#define STM32C0_FLASH_OPTR_BORF_LEV_0                  (0x1UL << STM32C0_FLASH_OPTR_BORF_LEV_Pos)    /*!< 0x00000800 */
#define STM32C0_FLASH_OPTR_BORF_LEV_1                  (0x2UL << STM32C0_FLASH_OPTR_BORF_LEV_Pos)    /*!< 0x00001000 */
#define STM32C0_FLASH_OPTR_nRST_STOP_Pos               (13U)
#define STM32C0_FLASH_OPTR_nRST_STOP_Msk               (0x1UL << STM32C0_FLASH_OPTR_nRST_STOP_Pos)   /*!< 0x00002000 */
#define STM32C0_FLASH_OPTR_nRST_STOP                   STM32C0_FLASH_OPTR_nRST_STOP_Msk
#define STM32C0_FLASH_OPTR_nRST_STDBY_Pos              (14U)
#define STM32C0_FLASH_OPTR_nRST_STDBY_Msk              (0x1UL << STM32C0_FLASH_OPTR_nRST_STDBY_Pos)  /*!< 0x00004000 */
#define STM32C0_FLASH_OPTR_nRST_STDBY                  STM32C0_FLASH_OPTR_nRST_STDBY_Msk
#define STM32C0_FLASH_OPTR_nRST_SHDW_Pos               (15U)
#define STM32C0_FLASH_OPTR_nRST_SHDW_Msk               (0x1UL << STM32C0_FLASH_OPTR_nRST_SHDW_Pos)   /*!< 0x00008000 */
#define STM32C0_FLASH_OPTR_nRST_SHDW                   STM32C0_FLASH_OPTR_nRST_SHDW_Msk
#define STM32C0_FLASH_OPTR_IWDG_SW_Pos                 (16U)
#define STM32C0_FLASH_OPTR_IWDG_SW_Msk                 (0x1UL << STM32C0_FLASH_OPTR_IWDG_SW_Pos)     /*!< 0x00010000 */
#define STM32C0_FLASH_OPTR_IWDG_SW                     STM32C0_FLASH_OPTR_IWDG_SW_Msk
#define STM32C0_FLASH_OPTR_IWDG_STOP_Pos               (17U)
#define STM32C0_FLASH_OPTR_IWDG_STOP_Msk               (0x1UL << STM32C0_FLASH_OPTR_IWDG_STOP_Pos)   /*!< 0x00020000 */
#define STM32C0_FLASH_OPTR_IWDG_STOP                   STM32C0_FLASH_OPTR_IWDG_STOP_Msk
#define STM32C0_FLASH_OPTR_IWDG_STDBY_Pos              (18U)
#define STM32C0_FLASH_OPTR_IWDG_STDBY_Msk              (0x1UL << STM32C0_FLASH_OPTR_IWDG_STDBY_Pos)  /*!< 0x00040000 */
#define STM32C0_FLASH_OPTR_IWDG_STDBY                  STM32C0_FLASH_OPTR_IWDG_STDBY_Msk
#define STM32C0_FLASH_OPTR_WWDG_SW_Pos                 (19U)
#define STM32C0_FLASH_OPTR_WWDG_SW_Msk                 (0x1UL << STM32C0_FLASH_OPTR_WWDG_SW_Pos)     /*!< 0x00080000 */
#define STM32C0_FLASH_OPTR_WWDG_SW                     STM32C0_FLASH_OPTR_WWDG_SW_Msk
#define STM32C0_FLASH_OPTR_HSE_NOT_REMAPPED_Pos        (21U)
#define STM32C0_FLASH_OPTR_HSE_NOT_REMAPPED_Msk        (0x1UL << STM32C0_FLASH_OPTR_HSE_NOT_REMAPPED_Pos) /*!< 0x00200000 */
#define STM32C0_FLASH_OPTR_HSE_NOT_REMAPPED            STM32C0_FLASH_OPTR_HSE_NOT_REMAPPED_Msk
#define STM32C0_FLASH_OPTR_RAM_PARITY_CHECK_Pos        (22U)
#define STM32C0_FLASH_OPTR_RAM_PARITY_CHECK_Msk        (0x1UL << STM32C0_FLASH_OPTR_RAM_PARITY_CHECK_Pos) /*!< 0x00400000 */
#define STM32C0_FLASH_OPTR_RAM_PARITY_CHECK            STM32C0_FLASH_OPTR_RAM_PARITY_CHECK_Msk
#define STM32C0_FLASH_OPTR_SECURE_MUXING_EN_Pos        (23U)
#define STM32C0_FLASH_OPTR_SECURE_MUXING_EN_Msk        (0x1UL << STM32C0_FLASH_OPTR_SECURE_MUXING_EN_Pos) /*!< 0x00800000 */
#define STM32C0_FLASH_OPTR_SECURE_MUXING_EN            STM32C0_FLASH_OPTR_SECURE_MUXING_EN_Msk
#define STM32C0_FLASH_OPTR_nBOOT_SEL_Pos               (24U)
#define STM32C0_FLASH_OPTR_nBOOT_SEL_Msk               (0x1UL << STM32C0_FLASH_OPTR_nBOOT_SEL_Pos)  /*!< 0x01000000 */
#define STM32C0_FLASH_OPTR_nBOOT_SEL                   STM32C0_FLASH_OPTR_nBOOT_SEL_Msk
#define STM32C0_FLASH_OPTR_nBOOT1_Pos                  (25U)
#define STM32C0_FLASH_OPTR_nBOOT1_Msk                  (0x1UL << STM32C0_FLASH_OPTR_nBOOT1_Pos)     /*!< 0x02000000 */
#define STM32C0_FLASH_OPTR_nBOOT1                      STM32C0_FLASH_OPTR_nBOOT1_Msk
#define STM32C0_FLASH_OPTR_nBOOT0_Pos                  (26U)
#define STM32C0_FLASH_OPTR_nBOOT0_Msk                  (0x1UL << STM32C0_FLASH_OPTR_nBOOT0_Pos)     /*!< 0x04000000 */
#define STM32C0_FLASH_OPTR_nBOOT0                      STM32C0_FLASH_OPTR_nBOOT0_Msk
#define STM32C0_FLASH_OPTR_NRST_MODE_Pos               (27U)
#define STM32C0_FLASH_OPTR_NRST_MODE_Msk               (0x3UL << STM32C0_FLASH_OPTR_NRST_MODE_Pos)  /*!< 0x18000000 */
#define STM32C0_FLASH_OPTR_NRST_MODE                   STM32C0_FLASH_OPTR_NRST_MODE_Msk
#define STM32C0_FLASH_OPTR_NRST_MODE_0                 (0x1UL << STM32C0_FLASH_OPTR_NRST_MODE_Pos)  /*!< 0x08000000 */
#define STM32C0_FLASH_OPTR_NRST_MODE_1                 (0x2UL << STM32C0_FLASH_OPTR_NRST_MODE_Pos)  /*!< 0x10000000 */
#define STM32C0_FLASH_OPTR_IRHEN_Pos                   (29U)
#define STM32C0_FLASH_OPTR_IRHEN_Msk                   (0x1UL << STM32C0_FLASH_OPTR_IRHEN_Pos)      /*!< 0x20000000 */
#define STM32C0_FLASH_OPTR_IRHEN                       STM32C0_FLASH_OPTR_IRHEN_Msk


/** @defgroup FLASH_OB_Read_Protection FLASH Option Bytes Read Protection
  * @{
  */
#define STM32C0_OB_RDP_LEVEL_0                  0x000000AAU
#define STM32C0_OB_RDP_LEVEL_1                  0x000000BBU
#define STM32C0_OB_RDP_LEVEL_2                  0x000000CCU  /*!< Warning: When enabling read protection level 2
                                                          it is no more possible to go back to level 1 or 0 */
/**
  * @}
  */

/** @defgroup FLASH_OB_USER_Type FLASH Option Bytes User Type
  * @{
  */
#define STM32C0_OB_USER_BOR_EN                  STM32C0_FLASH_OPTR_BOR_EN                           /*!< BOR reset enable */
#define STM32C0_OB_USER_BOR_LEV                 (STM32C0_FLASH_OPTR_BORF_LEV | STM32C0_FLASH_OPTR_BORR_LEV) /*!< BOR reset Level */
#define STM32C0_OB_USER_NRST_STOP               STM32C0_FLASH_OPTR_nRST_STOP                        /*!< Reset generated when entering
                                                                                         the stop mode */
#define STM32C0_OB_USER_NRST_STDBY              STM32C0_FLASH_OPTR_nRST_STDBY                       /*!< Reset generated when entering
                                                                                         the standby mode */
#define STM32C0_OB_USER_NRST_SHDW               STM32C0_FLASH_OPTR_nRST_SHDW                        /*!< Reset generated when entering
                                                                                         the shutdown mode */
#define STM32C0_OB_USER_IWDG_SW                 STM32C0_FLASH_OPTR_IWDG_SW                          /*!< Independent watchdog selection */
#define STM32C0_OB_USER_IWDG_STOP               STM32C0_FLASH_OPTR_IWDG_STOP                        /*!< Independent watchdog counter
                                                                                         freeze in stop mode */
#define STM32C0_OB_USER_IWDG_STDBY              STM32C0_FLASH_OPTR_IWDG_STDBY                       /*!< Independent watchdog counter
                                                                                         freeze in standby mode */
#define STM32C0_OB_USER_WWDG_SW                 STM32C0_FLASH_OPTR_WWDG_SW                          /*!< Window watchdog selection */
#if defined(STM32C0_FLASH_OPTR_HSE_NOT_REMAPPED)
#define STM32C0_OB_USER_HSE_NOT_REMAPPED        STM32C0_FLASH_OPTR_HSE_NOT_REMAPPED                 /*!< Remap HSE source from PF to PC */
#endif /* FLASH_OPTR_HSE_NOT_REMAPPED */
#define STM32C0_OB_USER_RAM_PARITY_CHECK        STM32C0_FLASH_OPTR_RAM_PARITY_CHECK                 /*!< Sram parity check control */
#define STM32C0_OB_USER_SECURE_MUXING_EN        STM32C0_FLASH_OPTR_SECURE_MUXING_EN                 /*!< Multiple-bonding security enable */
#define STM32C0_OB_USER_NBOOT_SEL               STM32C0_FLASH_OPTR_nBOOT_SEL                        /*!< Boot Selection */
#define STM32C0_OB_USER_NBOOT1                  STM32C0_FLASH_OPTR_nBOOT1                           /*!< nBoot1 configuration */
#define STM32C0_OB_USER_NBOOT0                  STM32C0_FLASH_OPTR_nBOOT0                           /*!< nBoot0 configuration */
#define STM32C0_OB_USER_NRST_MODE               STM32C0_FLASH_OPTR_NRST_MODE                        /*!< Reset pin configuration */
#define STM32C0_OB_USER_INPUT_RESET_HOLDER      STM32C0_FLASH_OPTR_IRHEN                            /*!< Internal reset holder enable */

#if defined(STM32C0_FLASH_OPTR_HSE_NOT_REMAPPED)
#define STM32C0_OB_USER_ALL                     (STM32C0_OB_USER_BOR_EN           | STM32C0_OB_USER_BOR_LEV    | STM32C0_OB_USER_NRST_STOP | \
                                         STM32C0_OB_USER_NRST_STDBY       | STM32C0_OB_USER_NRST_SHDW  | STM32C0_OB_USER_IWDG_SW   | \
                                         STM32C0_OB_USER_IWDG_STOP        | STM32C0_OB_USER_IWDG_STDBY | STM32C0_OB_USER_WWDG_SW   | \
                                         STM32C0_OB_USER_HSE_NOT_REMAPPED | STM32C0_OB_USER_RAM_PARITY_CHECK |               \
                                         STM32C0_OB_USER_SECURE_MUXING_EN | STM32C0_OB_USER_NBOOT_SEL  | STM32C0_OB_USER_NBOOT1    | \
                                         STM32C0_OB_USER_NBOOT0           | STM32C0_OB_USER_NRST_MODE  | STM32C0_OB_USER_INPUT_RESET_HOLDER) /*!< all option bits */
#else
#define STM32C0_OB_USER_ALL                     (STM32C0_OB_USER_BOR_EN           | STM32C0_OB_USER_BOR_LEV    | STM32C0_OB_USER_NRST_STOP | \
                                         STM32C0_OB_USER_NRST_STDBY       | STM32C0_OB_USER_NRST_SHDW  | STM32C0_OB_USER_IWDG_SW   | \
                                         STM32C0_OB_USER_IWDG_STOP        | STM32C0_OB_USER_IWDG_STDBY | STM32C0_OB_USER_WWDG_SW   | \
                                         STM32C0_OB_USER_RAM_PARITY_CHECK | STM32C0_OB_USER_SECURE_MUXING_EN |               \
                                         STM32C0_OB_USER_NBOOT_SEL        | STM32C0_OB_USER_NBOOT1     |                     \
                                         STM32C0_OB_USER_NBOOT0           | STM32C0_OB_USER_NRST_MODE  | STM32C0_OB_USER_INPUT_RESET_HOLDER) /*!< all option bits */
#endif /* FLASH_OPTR_HSE_NOT_REMAPPED */

bool Stm32c0_Flash_MassErase(void);
bool Stm32c0_Flash_Program(uint32_t Address, uint64_t Data);
bool Stm32c0_Flash_Unlock(void);
bool Stm32c0_Flash_Lock(void);
bool Stm32c0_Flash_WaitOperation(uint32_t Timeout);
bool Stm32c0_Flash_OB_Lock(void);
bool Stm32c0_Flash_OB_Unlock(void);
uint32_t Stm32c0_Flash_OB_GetUser(void);
void Stm32c0_Flash_OB_OptrConfig(uint32_t UserType, uint32_t UserConfig, uint32_t RDPLevel);
void Stm32c0_Flash_OB_Launch(void);
bool Stm32c0_Flash_OB_Program(uint32_t RDPLevel);
#endif /* STM32C0_FLASH_H_ */
