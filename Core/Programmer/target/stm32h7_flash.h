/*
 * stm32h7_flash.h
 *
 *  Created on: Jul 5, 2024
 *      Author: kangh
 */

#ifndef STM32H7_FLASH_H_
#define STM32H7_FLASH_H_

#include <stdbool.h>
#include <stdint.h>


/**
  * @brief FLASH Registers
  */
#define STM32H7_PERIPH_BASE										(0x40000000UL)
#define STM32H7_D1_AHB1PERIPH_BASE						(STM32H7_PERIPH_BASE + 0x12000000UL)
#define STM32H7_FLASH_R_BASE									(STM32H7_D1_AHB1PERIPH_BASE + 0x00002000UL)
#define STM32H7_FLASH               					(STM32H7_FLASH_R_BASE)
#define STM32H7_FLASH_KEYR1										(STM32H7_FLASH_R_BASE) + 0x04
#define STM32H7_FLASH_OPTKEYR									(STM32H7_FLASH_R_BASE) + 0x08
#define STM32H7_FLASH_CR1											(STM32H7_FLASH_R_BASE) + 0x0C
#define STM32H7_FLASH_SR1											(STM32H7_FLASH_R_BASE) + 0x10
#define STM32H7_FLASH_CCR1										(STM32H7_FLASH_R_BASE) + 0x14
#define STM32H7_FLASH_OPTCR										(STM32H7_FLASH_R_BASE) + 0x18
#define STM32H7_FLASH_OPTSR_CUR								(STM32H7_FLASH_R_BASE) + 0x1C
#define STM32H7_FLASH_OPTSR_PRG								(STM32H7_FLASH_R_BASE) + 0x20
#define STM32H7_FLASH_OPTCCR									(STM32H7_FLASH_R_BASE) + 0x24
#define STM32H7_FLASH_KEYR2										(STM32H7_FLASH_R_BASE) + 0x104
#define STM32H7_FLASH_CR2											(STM32H7_FLASH_R_BASE) + 0x10C
#define STM32H7_FLASH_SR2											(STM32H7_FLASH_R_BASE) + 0x110
#define STM32H7_FLASH_CCR2										(STM32H7_FLASH_R_BASE) + 0x114

/** @defgroup FLASH_Keys FLASH Keys
  * @{
  */
#define STM32H7_FLASH_KEY1                 0x45670123U
#define STM32H7_FLASH_KEY2                 0xCDEF89ABU
#define STM32H7_FLASH_OPT_KEY1             0x08192A3BU
#define STM32H7_FLASH_OPT_KEY2             0x4C5D6E7FU


#define STM32H7_FLASH_OPTION_OPTR							0x1FFF7800
/******************************************************************************/
/*                                                                            */
/*                                    FLASH                                   */
/*                                                                            */
/******************************************************************************/
/*
* @brief FLASH Global Defines
*/
#define STM32H7_FLASH_SIZE_DATA_REGISTER             0x1FF1E880U
#define STM32H7_FLASH_SECTOR_TOTAL                   8U                    /* 8 sectors */
#define STM32H7_FLASH_SIZE                           ((((*((uint16_t *)STM32H7_FLASH_SIZE_DATA_REGISTER)) == 0xFFFFU)) ? 0x200000U : \
                                             ((((*((uint16_t *)STM32H7_FLASH_SIZE_DATA_REGISTER)) == 0x0000U)) ? 0x200000U : \
                                             (((uint32_t)(*((uint16_t *)STM32H7_FLASH_SIZE_DATA_REGISTER)) & (0x0FFFU)) << 10U)))  /* 2 MB   */
#define STM32H7_FLASH_BANK_SIZE                      (STM32H7_FLASH_SIZE >> 1)     /* 1 MB   */
#define STM32H7_FLASH_SECTOR_SIZE                    0x00020000UL          /* 128 KB */
#define FLASH_LATENCY_DEFAULT                STM32H7_FLASH_ACR_LATENCY_7WS /* FLASH Seven Latency cycles */
#define STM32H7_FLASH_NB_32BITWORD_IN_FLASHWORD_8WORD      8U                    /* 256 bits */
#define STM32H7_FLASH_NB_32BITWORD_IN_FLASHWORD_4WORD      4U                    /* 256 bits */
#define STM32H7_DUAL_BANK                                                  /* Dual-bank Flash */

/* Dual Bank Support Device */
/*
stm32h7a3xx
stm32h7a3xxq
stm32h7b3xx
stm32h7b3xxq
stm32h742xx
stm32h743xx
stm32h745xg
stm32h745xx
stm32h747xg
stm32h747xx
stm32h753xx
stm32h755xx
stm32h757xx
 */
#define STM32H7_MAX_DELAY 0xFFFFFFFFU

#define STM32H7_FLASH_BANK1_BASE          (0x08000000UL) /*!< Base address of : (up to 128 KB) Flash Bank1 accessible over AXI                        */
#define STM32H7_FLASH_BANK2_BASE          (0x08100000UL) /*!< For legacy only , Flash bank 2 not available on STM32H750xx value line          */
#define STM32H7_FLASH_END                 (0x0801FFFFUL) /*!< FLASH end address */

#define FLASH_NB_32BITWORD_IN_FLASHWORD_4WORD      4U                    /* 128 bits */
#define FLASH_NB_32BITWORD_IN_FLASHWORD_8WORD      				8U                    /* 256 bits */

#if defined (STM32H7_DUAL_BANK)
#define IS_STM32H7_FLASH_PROGRAM_ADDRESS_BANK1(ADDRESS) (((ADDRESS) >= STM32H7_FLASH_BANK1_BASE) && ((ADDRESS) < STM32H7_FLASH_BANK2_BASE))
#define IS_STM32H7_FLASH_PROGRAM_ADDRESS_BANK2(ADDRESS) (((ADDRESS) >= STM32H7_FLASH_BANK2_BASE ) && ((ADDRESS) <= STM32H7_FLASH_END))
#else
#define IS_STM32H7_FLASH_PROGRAM_ADDRESS_BANK1(ADDRESS) (((ADDRESS) >= FLASH_BANK1_BASE) && ((ADDRESS) <= FLASH_END))
#endif /* DUAL_BANK */

#define STM32H7_FLASH_TIMEOUT_VALUE              50000U /* 50 s */

/*******************  Bits definition for FLASH_ACR register  **********************/
#define STM32H7_FLASH_ACR_LATENCY_Pos                (0U)
#define STM32H7_FLASH_ACR_LATENCY_Msk                (0xFUL << STM32H7_FLASH_ACR_LATENCY_Pos)  /*!< 0x0000000F: bit4 is kept only for legacy purpose */
#define STM32H7_FLASH_ACR_LATENCY                    STM32H7_FLASH_ACR_LATENCY_Msk             /*!< Read Latency */
#define STM32H7_FLASH_ACR_LATENCY_0WS                (0x00000000UL)
#define STM32H7_FLASH_ACR_LATENCY_1WS                (0x00000001UL)
#define STM32H7_FLASH_ACR_LATENCY_2WS                (0x00000002UL)
#define STM32H7_FLASH_ACR_LATENCY_3WS                (0x00000003UL)
#define STM32H7_FLASH_ACR_LATENCY_4WS                (0x00000004UL)
#define STM32H7_FLASH_ACR_LATENCY_5WS                (0x00000005UL)
#define STM32H7_FLASH_ACR_LATENCY_6WS                (0x00000006UL)
#define STM32H7_FLASH_ACR_LATENCY_7WS                (0x00000007UL)

#define STM32H7_FLASH_ACR_WRHIGHFREQ_Pos             (4U)
#define STM32H7_FLASH_ACR_WRHIGHFREQ_Msk             (0x3UL << STM32H7_FLASH_ACR_WRHIGHFREQ_Pos)  /*!< 0x00000030 */
#define STM32H7_FLASH_ACR_WRHIGHFREQ                 STM32H7_FLASH_ACR_WRHIGHFREQ_Msk             /*!< Flash signal delay */
#define STM32H7_FLASH_ACR_WRHIGHFREQ_0               (0x1UL << STM32H7_FLASH_ACR_WRHIGHFREQ_Pos)  /*!< 0x00000010 */
#define STM32H7_FLASH_ACR_WRHIGHFREQ_1               (0x2UL << STM32H7_FLASH_ACR_WRHIGHFREQ_Pos)  /*!< 0x00000020 */

/* Legacy FLASH Latency defines */
#define STM32H7_FLASH_ACR_LATENCY_8WS                (0x00000008UL)
#define STM32H7_FLASH_ACR_LATENCY_9WS                (0x00000009UL)
#define STM32H7_FLASH_ACR_LATENCY_10WS               (0x0000000AUL)
#define STM32H7_FLASH_ACR_LATENCY_11WS               (0x0000000BUL)
#define STM32H7_FLASH_ACR_LATENCY_12WS               (0x0000000CUL)
#define STM32H7_FLASH_ACR_LATENCY_13WS               (0x0000000DUL)
#define STM32H7_FLASH_ACR_LATENCY_14WS               (0x0000000EUL)
#define STM32H7_FLASH_ACR_LATENCY_15WS               (0x0000000FUL)
/*******************  Bits definition for FLASH_CR register  ***********************/
#define STM32H7_FLASH_CR_LOCK_Pos                    (0U)
#define STM32H7_FLASH_CR_LOCK_Msk                    (0x1UL << STM32H7_FLASH_CR_LOCK_Pos)      /*!< 0x00000001 */
#define STM32H7_FLASH_CR_LOCK                        STM32H7_FLASH_CR_LOCK_Msk                 /*!< Configuration lock bit */
#define STM32H7_FLASH_CR_PG_Pos                      (1U)
#define STM32H7_FLASH_CR_PG_Msk                      (0x1UL << STM32H7_FLASH_CR_PG_Pos)        /*!< 0x00000002 */
#define STM32H7_FLASH_CR_PG                          STM32H7_FLASH_CR_PG_Msk                   /*!< Internal buffer control bit */
#define STM32H7_FLASH_CR_SER_Pos                     (2U)
#define STM32H7_FLASH_CR_SER_Msk                     (0x1UL << STM32H7_FLASH_CR_SER_Pos)       /*!< 0x00000004 */
#define STM32H7_FLASH_CR_SER                         STM32H7_FLASH_CR_SER_Msk                  /*!< Sector erase request */
#define STM32H7_FLASH_CR_BER_Pos                     (3U)
#define STM32H7_FLASH_CR_BER_Msk                     (0x1UL << STM32H7_FLASH_CR_BER_Pos)       /*!< 0x00000008 */
#define STM32H7_FLASH_CR_BER                         STM32H7_FLASH_CR_BER_Msk                  /*!< Bank erase request */
#define STM32H7_FLASH_CR_PSIZE_Pos                   (4U)
#define STM32H7_FLASH_CR_PSIZE_Msk                   (0x3UL << STM32H7_FLASH_CR_PSIZE_Pos)     /*!< 0x00000030 */
#define STM32H7_FLASH_CR_PSIZE                       STM32H7_FLASH_CR_PSIZE_Msk                /*!< Program size */
/*
 * CP SIZE SUPPORT MCU
stm32h723xx
stm32h725xx
stm32h730xx
stm32h730xxq
stm32h733xx
stm32h735xx
stm32h742xx
stm32h743xx
stm32h745xg
stm32h745xx
stm32h747xg
stm32h747xx
stm32h750xx
stm32h753xx
stm32h755xx
stm32h757xx
 */
#define STM32H7_FLASH_CR_PSIZE_0                     (0x1UL << STM32H7_FLASH_CR_PSIZE_Pos)     /*!< 0x00000010 */
#define STM32H7_FLASH_CR_PSIZE_1                     (0x2UL << STM32H7_FLASH_CR_PSIZE_Pos)     /*!< 0x00000020 */
#define STM32H7_FLASH_CR_FW_Pos                      (6U)
#define STM32H7_FLASH_CR_FW_Msk                      (0x1UL << STM32H7_FLASH_CR_FW_Pos)        /*!< 0x00000040 */
#define STM32H7_FLASH_CR_FW                          STM32H7_FLASH_CR_FW_Msk                   /*!< Write forcing control bit */
#define STM32H7_FLASH_CR_START_Pos                   (7U)
#define STM32H7_FLASH_CR_START_Msk                   (0x1UL << STM32H7_FLASH_CR_START_Pos)     /*!< 0x00000080 */
#define STM32H7_FLASH_CR_START                       STM32H7_FLASH_CR_START_Msk                /*!< Erase start control bit */
#define STM32H7_FLASH_CR_SNB_Pos                     (8U)
#define STM32H7_FLASH_CR_SNB_Msk                     (0x7UL << STM32H7_FLASH_CR_SNB_Pos)       /*!< 0x00000700 */
#define STM32H7_FLASH_CR_SNB                         STM32H7_FLASH_CR_SNB_Msk                  /*!< Sector erase selection number */
#define STM32H7_FLASH_CR_SNB_0                       (0x1UL << STM32H7_FLASH_CR_SNB_Pos)       /*!< 0x00000100 */
#define STM32H7_FLASH_CR_SNB_1                       (0x2UL << STM32H7_FLASH_CR_SNB_Pos)       /*!< 0x00000200 */
#define STM32H7_FLASH_CR_SNB_2                       (0x4UL << STM32H7_FLASH_CR_SNB_Pos)       /*!< 0x00000400 */
#define STM32H7_FLASH_CR_CRC_EN_Pos                  (15U)
#define STM32H7_FLASH_CR_CRC_EN_Msk                  (0x1UL << STM32H7_FLASH_CR_CRC_EN_Pos)    /*!< 0x00008000 */
#define STM32H7_FLASH_CR_CRC_EN                      STM32H7_FLASH_CR_CRC_EN_Msk               /*!< CRC control bit */
#define STM32H7_FLASH_CR_EOPIE_Pos                   (16U)
#define STM32H7_FLASH_CR_EOPIE_Msk                   (0x1UL << STM32H7_FLASH_CR_EOPIE_Pos)     /*!< 0x00010000 */
#define STM32H7_FLASH_CR_EOPIE                       STM32H7_FLASH_CR_EOPIE_Msk                /*!< End-of-program interrupt control bit */
#define STM32H7_FLASH_CR_WRPERRIE_Pos                (17U)
#define STM32H7_FLASH_CR_WRPERRIE_Msk                (0x1UL << STM32H7_FLASH_CR_WRPERRIE_Pos)  /*!< 0x00020000 */
#define STM32H7_FLASH_CR_WRPERRIE                    STM32H7_FLASH_CR_WRPERRIE_Msk             /*!< Write protection error interrupt enable bit */
#define STM32H7_FLASH_CR_PGSERRIE_Pos                (18U)
#define STM32H7_FLASH_CR_PGSERRIE_Msk                (0x1UL << STM32H7_FLASH_CR_PGSERRIE_Pos)  /*!< 0x00040000 */
#define STM32H7_FLASH_CR_PGSERRIE                    STM32H7_FLASH_CR_PGSERRIE_Msk             /*!< Programming sequence error interrupt enable bit */
#define STM32H7_FLASH_CR_STRBERRIE_Pos               (19U)
#define STM32H7_FLASH_CR_STRBERRIE_Msk               (0x1UL << STM32H7_FLASH_CR_STRBERRIE_Pos) /*!< 0x00080000 */
#define STM32H7_FLASH_CR_STRBERRIE                   STM32H7_FLASH_CR_STRBERRIE_Msk            /*!< Strobe error interrupt enable bit */
#define STM32H7_FLASH_CR_INCERRIE_Pos                (21U)
#define STM32H7_FLASH_CR_INCERRIE_Msk                (0x1UL << STM32H7_FLASH_CR_INCERRIE_Pos)  /*!< 0x00200000 */
#define STM32H7_FLASH_CR_INCERRIE                    STM32H7_FLASH_CR_INCERRIE_Msk             /*!< Inconsistency error interrupt enable bit */
#define STM32H7_FLASH_CR_OPERRIE_Pos                 (22U)
#define STM32H7_FLASH_CR_OPERRIE_Msk                 (0x1UL << STM32H7_FLASH_CR_OPERRIE_Pos)   /*!< 0x00400000 */
#define STM32H7_FLASH_CR_OPERRIE                     STM32H7_FLASH_CR_OPERRIE_Msk              /*!< Write/erase error interrupt enable bit */
#define STM32H7_FLASH_CR_RDPERRIE_Pos                (23U)
#define STM32H7_FLASH_CR_RDPERRIE_Msk                (0x1UL << STM32H7_FLASH_CR_RDPERRIE_Pos)  /*!< 0x00800000 */
#define STM32H7_FLASH_CR_RDPERRIE                    STM32H7_FLASH_CR_RDPERRIE_Msk             /*!< Read protection error interrupt enable bit */
#define STM32H7_FLASH_CR_RDSERRIE_Pos                (24U)
#define STM32H7_FLASH_CR_RDSERRIE_Msk                (0x1UL << STM32H7_FLASH_CR_RDSERRIE_Pos)  /*!< 0x01000000 */
#define STM32H7_FLASH_CR_RDSERRIE                    STM32H7_FLASH_CR_RDSERRIE_Msk             /*!< Secure error interrupt enable bit */
#define STM32H7_FLASH_CR_SNECCERRIE_Pos              (25U)
#define STM32H7_FLASH_CR_SNECCERRIE_Msk              (0x1UL << STM32H7_FLASH_CR_SNECCERRIE_Pos) /*!< 0x02000000 */
#define STM32H7_FLASH_CR_SNECCERRIE                  STM32H7_FLASH_CR_SNECCERRIE_Msk            /*!< ECC single correction error interrupt enable bit */
#define STM32H7_FLASH_CR_DBECCERRIE_Pos              (26U)
#define STM32H7_FLASH_CR_DBECCERRIE_Msk              (0x1UL << STM32H7_FLASH_CR_DBECCERRIE_Pos) /*!< 0x04000000 */
#define STM32H7_FLASH_CR_DBECCERRIE                  STM32H7_FLASH_CR_DBECCERRIE_Msk            /*!< ECC double detection error interrupt enable bit */
#define STM32H7_FLASH_CR_CRCENDIE_Pos                (27U)
#define STM32H7_FLASH_CR_CRCENDIE_Msk                (0x1UL << STM32H7_FLASH_CR_CRCENDIE_Pos)  /*!< 0x08000000 */
#define STM32H7_FLASH_CR_CRCENDIE                    STM32H7_FLASH_CR_CRCENDIE_Msk             /*!< CRC end of calculation interrupt enable bit */
#define STM32H7_FLASH_CR_CRCRDERRIE_Pos              (28U)
#define STM32H7_FLASH_CR_CRCRDERRIE_Msk              (0x1UL << STM32H7_FLASH_CR_CRCRDERRIE_Pos) /*!< 0x10000000 */
#define STM32H7_FLASH_CR_CRCRDERRIE                  STM32H7_FLASH_CR_CRCRDERRIE_Msk            /*!< CRC read error interrupt enable bit */

/*******************  Bits definition for FLASH_SR register  ***********************/
#define STM32H7_FLASH_SR_BSY_Pos                     (0U)
#define STM32H7_FLASH_SR_BSY_Msk                     (0x1UL << STM32H7_FLASH_SR_BSY_Pos)       /*!< 0x00000001 */
#define STM32H7_FLASH_SR_BSY                         STM32H7_FLASH_SR_BSY_Msk                  /*!< Busy flag */
#define STM32H7_FLASH_SR_WBNE_Pos                    (1U)
#define STM32H7_FLASH_SR_WBNE_Msk                    (0x1UL << STM32H7_FLASH_SR_WBNE_Pos)      /*!< 0x00000002 */
#define STM32H7_FLASH_SR_WBNE                        STM32H7_FLASH_SR_WBNE_Msk                 /*!< Write buffer not empty flag */
#define STM32H7_FLASH_SR_QW_Pos                      (2U)
#define STM32H7_FLASH_SR_QW_Msk                      (0x1UL << STM32H7_FLASH_SR_QW_Pos)        /*!< 0x00000004 */
#define STM32H7_FLASH_SR_QW                          STM32H7_FLASH_SR_QW_Msk                   /*!< Wait queue flag */
#define STM32H7_FLASH_SR_CRC_BUSY_Pos                (3U)
#define STM32H7_FLASH_SR_CRC_BUSY_Msk                (0x1UL << STM32H7_FLASH_SR_CRC_BUSY_Pos)  /*!< 0x00000008 */
#define STM32H7_FLASH_SR_CRC_BUSY                    STM32H7_FLASH_SR_CRC_BUSY_Msk             /*!< CRC busy flag */
#define STM32H7_FLASH_SR_EOP_Pos                     (16U)
#define STM32H7_FLASH_SR_EOP_Msk                     (0x1UL << STM32H7_FLASH_SR_EOP_Pos)       /*!< 0x00010000 */
#define STM32H7_FLASH_SR_EOP                         STM32H7_FLASH_SR_EOP_Msk                  /*!< End-of-program flag */
#define STM32H7_FLASH_SR_WRPERR_Pos                  (17U)
#define STM32H7_FLASH_SR_WRPERR_Msk                  (0x1UL << STM32H7_FLASH_SR_WRPERR_Pos)    /*!< 0x00020000 */
#define STM32H7_FLASH_SR_WRPERR                      STM32H7_FLASH_SR_WRPERR_Msk               /*!< Write protection error flag */
#define STM32H7_FLASH_SR_PGSERR_Pos                  (18U)
#define STM32H7_FLASH_SR_PGSERR_Msk                  (0x1UL << STM32H7_FLASH_SR_PGSERR_Pos)    /*!< 0x00040000 */
#define STM32H7_FLASH_SR_PGSERR                      STM32H7_FLASH_SR_PGSERR_Msk               /*!< Programming sequence error flag */
#define STM32H7_FLASH_SR_STRBERR_Pos                 (19U)
#define STM32H7_FLASH_SR_STRBERR_Msk                 (0x1UL << STM32H7_FLASH_SR_STRBERR_Pos)   /*!< 0x00080000 */
#define STM32H7_FLASH_SR_STRBERR                     STM32H7_FLASH_SR_STRBERR_Msk              /*!< Strobe error flag */
#define STM32H7_FLASH_SR_INCERR_Pos                  (21U)
#define STM32H7_FLASH_SR_INCERR_Msk                  (0x1UL << STM32H7_FLASH_SR_INCERR_Pos)    /*!< 0x00200000 */
#define STM32H7_FLASH_SR_INCERR                      STM32H7_FLASH_SR_INCERR_Msk               /*!< Inconsistency error flag */
#define STM32H7_FLASH_SR_OPERR_Pos                   (22U)
#define STM32H7_FLASH_SR_OPERR_Msk                   (0x1UL << STM32H7_FLASH_SR_OPERR_Pos)     /*!< 0x00400000 */
#define STM32H7_FLASH_SR_OPERR                       STM32H7_FLASH_SR_OPERR_Msk                /*!< Write/erase error flag */
#define STM32H7_FLASH_SR_RDPERR_Pos                  (23U)
#define STM32H7_FLASH_SR_RDPERR_Msk                  (0x1UL << STM32H7_FLASH_SR_RDPERR_Pos)    /*!< 0x00800000 */
#define STM32H7_FLASH_SR_RDPERR                      STM32H7_FLASH_SR_RDPERR_Msk               /*!< Read protection error flag */
#define STM32H7_FLASH_SR_RDSERR_Pos                  (24U)
#define STM32H7_FLASH_SR_RDSERR_Msk                  (0x1UL << STM32H7_FLASH_SR_RDSERR_Pos)    /*!< 0x01000000 */
#define STM32H7_FLASH_SR_RDSERR                      STM32H7_FLASH_SR_RDSERR_Msk               /*!< Secure error flag */
#define STM32H7_FLASH_SR_SNECCERR_Pos                (25U)
#define STM32H7_FLASH_SR_SNECCERR_Msk                (0x1UL << STM32H7_FLASH_SR_SNECCERR_Pos)  /*!< 0x02000000 */
#define STM32H7_FLASH_SR_SNECCERR                    STM32H7_FLASH_SR_SNECCERR_Msk             /*!< Single correction error flag */
#define STM32H7_FLASH_SR_DBECCERR_Pos                (26U)
#define STM32H7_FLASH_SR_DBECCERR_Msk                (0x1UL << STM32H7_FLASH_SR_DBECCERR_Pos)  /*!< 0x04000000 */
#define STM32H7_FLASH_SR_DBECCERR                    STM32H7_FLASH_SR_DBECCERR_Msk             /*!< ECC double detection error flag */
#define STM32H7_FLASH_SR_CRCEND_Pos                  (27U)
#define STM32H7_FLASH_SR_CRCEND_Msk                  (0x1UL << STM32H7_FLASH_SR_CRCEND_Pos)    /*!< 0x08000000 */
#define STM32H7_FLASH_SR_CRCEND                      STM32H7_FLASH_SR_CRCEND_Msk               /*!< CRC end of calculation flag */
#define STM32H7_FLASH_SR_CRCRDERR_Pos                (28U)
#define STM32H7_FLASH_SR_CRCRDERR_Msk                (0x1UL << STM32H7_FLASH_SR_CRCRDERR_Pos)  /*!< 0x10000000 */
#define STM32H7_FLASH_SR_CRCRDERR                    STM32H7_FLASH_SR_CRCRDERR_Msk             /*!< CRC read error flag */

/*******************  Bits definition for FLASH_CCR register  *******************/
#define STM32H7_FLASH_CCR_CLR_EOP_Pos                (16U)
#define STM32H7_FLASH_CCR_CLR_EOP_Msk                (0x1UL << STM32H7_FLASH_CCR_CLR_EOP_Pos)  /*!< 0x00010000 */
#define STM32H7_STM32H7_FLASH_CCR_CLR_EOP                    STM32H7_FLASH_CCR_CLR_EOP_Msk             /*!< EOP flag clear bit */
#define STM32H7_FLASH_CCR_CLR_WRPERR_Pos             (17U)
#define STM32H7_FLASH_CCR_CLR_WRPERR_Msk             (0x1UL << STM32H7_FLASH_CCR_CLR_WRPERR_Pos) /*!< 0x00020000 */
#define STM32H7_FLASH_CCR_CLR_WRPERR                 STM32H7_FLASH_CCR_CLR_WRPERR_Msk            /*!< WRPERR flag clear bit */
#define STM32H7_FLASH_CCR_CLR_PGSERR_Pos             (18U)
#define STM32H7_FLASH_CCR_CLR_PGSERR_Msk             (0x1UL << STM32H7_FLASH_CCR_CLR_PGSERR_Pos) /*!< 0x00040000 */
#define STM32H7_FLASH_CCR_CLR_PGSERR                 STM32H7_FLASH_CCR_CLR_PGSERR_Msk            /*!< PGSERR flag clear bit */
#define STM32H7_FLASH_CCR_CLR_STRBERR_Pos            (19U)
#define STM32H7_FLASH_CCR_CLR_STRBERR_Msk            (0x1UL << STM32H7_FLASH_CCR_CLR_STRBERR_Pos) /*!< 0x00080000 */
#define STM32H7_FLASH_CCR_CLR_STRBERR                STM32H7_FLASH_CCR_CLR_STRBERR_Msk            /*!< STRBERR flag clear bit */
#define STM32H7_FLASH_CCR_CLR_INCERR_Pos             (21U)
#define STM32H7_FLASH_CCR_CLR_INCERR_Msk             (0x1UL << STM32H7_FLASH_CCR_CLR_INCERR_Pos) /*!< 0x00200000 */
#define STM32H7_FLASH_CCR_CLR_INCERR                 STM32H7_FLASH_CCR_CLR_INCERR_Msk            /*!< INCERR flag clear bit */
#define STM32H7_FLASH_CCR_CLR_OPERR_Pos              (22U)
#define STM32H7_FLASH_CCR_CLR_OPERR_Msk              (0x1UL << STM32H7_FLASH_CCR_CLR_OPERR_Pos) /*!< 0x00400000 */
#define STM32H7_FLASH_CCR_CLR_OPERR                  STM32H7_FLASH_CCR_CLR_OPERR_Msk            /*!< OPERR flag clear bit */
#define STM32H7_FLASH_CCR_CLR_RDPERR_Pos             (23U)
#define STM32H7_FLASH_CCR_CLR_RDPERR_Msk             (0x1UL << STM32H7_FLASH_CCR_CLR_RDPERR_Pos) /*!< 0x00800000 */
#define STM32H7_FLASH_CCR_CLR_RDPERR                 STM32H7_FLASH_CCR_CLR_RDPERR_Msk            /*!< RDPERR flag clear bit */
#define STM32H7_FLASH_CCR_CLR_RDSERR_Pos             (24U)
#define STM32H7_FLASH_CCR_CLR_RDSERR_Msk             (0x1UL << STM32H7_FLASH_CCR_CLR_RDSERR_Pos) /*!< 0x01000000 */
#define STM32H7_FLASH_CCR_CLR_RDSERR                 STM32H7_FLASH_CCR_CLR_RDSERR_Msk            /*!< RDSERR flag clear bit */
#define STM32H7_FLASH_CCR_CLR_SNECCERR_Pos           (25U)
#define STM32H7_FLASH_CCR_CLR_SNECCERR_Msk           (0x1UL << STM32H7_FLASH_CCR_CLR_SNECCERR_Pos) /*!< 0x02000000 */
#define STM32H7_FLASH_CCR_CLR_SNECCERR               STM32H7_FLASH_CCR_CLR_SNECCERR_Msk            /*!< SNECCERR flag clear bit */
#define STM32H7_FLASH_CCR_CLR_DBECCERR_Pos           (26U)
#define STM32H7_FLASH_CCR_CLR_DBECCERR_Msk           (0x1UL << STM32H7_FLASH_CCR_CLR_DBECCERR_Pos) /*!< 0x04000000 */
#define STM32H7_FLASH_CCR_CLR_DBECCERR               STM32H7_FLASH_CCR_CLR_DBECCERR_Msk            /*!< DBECCERR flag clear bit */
#define STM32H7_FLASH_CCR_CLR_CRCEND_Pos             (27U)
#define STM32H7_FLASH_CCR_CLR_CRCEND_Msk             (0x1UL << STM32H7_FLASH_CCR_CLR_CRCEND_Pos) /*!< 0x08000000 */
#define STM32H7_FLASH_CCR_CLR_CRCEND                 STM32H7_FLASH_CCR_CLR_CRCEND_Msk            /*!< CRCEND flag clear bit */
#define STM32H7_FLASH_CCR_CLR_CRCRDERR_Pos           (28U)
#define STM32H7_FLASH_CCR_CLR_CRCRDERR_Msk           (0x1UL << STM32H7_FLASH_CCR_CLR_CRCRDERR_Pos) /*!< 0x10000000 */
#define STM32H7_FLASH_CCR_CLR_CRCRDERR               STM32H7_FLASH_CCR_CLR_CRCRDERR_Msk            /*!< CRCRDERR flag clear bit */

/*******************  Bits definition for FLASH_OPTCR register  *******************/
#define STM32H7_FLASH_OPTCR_OPTLOCK_Pos              (0U)
#define STM32H7_FLASH_OPTCR_OPTLOCK_Msk              (0x1UL << STM32H7_FLASH_OPTCR_OPTLOCK_Pos)  /*!< 0x00000001 */
#define STM32H7_FLASH_OPTCR_OPTLOCK                  STM32H7_FLASH_OPTCR_OPTLOCK_Msk             /*!< FLASH_OPTCR lock option configuration bit */
#define STM32H7_FLASH_OPTCR_OPTSTART_Pos             (1U)
#define STM32H7_FLASH_OPTCR_OPTSTART_Msk             (0x1UL << STM32H7_FLASH_OPTCR_OPTSTART_Pos) /*!< 0x00000002 */
#define STM32H7_FLASH_OPTCR_OPTSTART                 STM32H7_FLASH_OPTCR_OPTSTART_Msk            /*!< Option byte start change option configuration bit */
#define STM32H7_FLASH_OPTCR_MER_Pos                  (4U)
#define STM32H7_FLASH_OPTCR_MER_Msk                  (0x1UL << STM32H7_FLASH_OPTCR_MER_Pos)      /*!< 0x00000010 */
#define STM32H7_FLASH_OPTCR_MER                      STM32H7_FLASH_OPTCR_MER_Msk                 /*!< Mass erase request */
#define STM32H7_FLASH_OPTCR_OPTCHANGEERRIE_Pos       (30U)
#define STM32H7_FLASH_OPTCR_OPTCHANGEERRIE_Msk       (0x1UL << STM32H7_FLASH_OPTCR_OPTCHANGEERRIE_Pos) /*!< 0x40000000 */
#define STM32H7_FLASH_OPTCR_OPTCHANGEERRIE           STM32H7_FLASH_OPTCR_OPTCHANGEERRIE_Msk            /*!< Option byte change error interrupt enable bit */
#define STM32H7_FLASH_OPTCR_SWAP_BANK_Pos            (31U)
#define STM32H7_FLASH_OPTCR_SWAP_BANK_Msk            (0x1UL << STM32H7_FLASH_OPTCR_SWAP_BANK_Pos) /*!< 0x80000000 */
#define STM32H7_FLASH_OPTCR_SWAP_BANK                STM32H7_FLASH_OPTCR_SWAP_BANK_Msk            /*!< Bank swapping option configuration bit */

/*******************  Bits definition for FLASH_OPTSR register  ***************/
#define STM32H7_FLASH_OPTSR_OPT_BUSY_Pos             (0U)
#define STM32H7_FLASH_OPTSR_OPT_BUSY_Msk             (0x1UL << STM32H7_FLASH_OPTSR_OPT_BUSY_Pos) /*!< 0x00000001 */
#define STM32H7_FLASH_OPTSR_OPT_BUSY                 STM32H7_FLASH_OPTSR_OPT_BUSY_Msk            /*!< Option byte change ongoing flag */
#define STM32H7_FLASH_OPTSR_BOR_LEV_Pos              (2U)
#define STM32H7_FLASH_OPTSR_BOR_LEV_Msk              (0x3UL << STM32H7_FLASH_OPTSR_BOR_LEV_Pos) /*!< 0x0000000C */
#define STM32H7_FLASH_OPTSR_BOR_LEV                  STM32H7_FLASH_OPTSR_BOR_LEV_Msk            /*!< Brownout level option status bit */
#define STM32H7_FLASH_OPTSR_BOR_LEV_0                (0x1UL << STM32H7_FLASH_OPTSR_BOR_LEV_Pos) /*!< 0x00000004 */
#define STM32H7_FLASH_OPTSR_BOR_LEV_1                (0x2UL << STM32H7_FLASH_OPTSR_BOR_LEV_Pos) /*!< 0x00000008 */
#define STM32H7_FLASH_OPTSR_IWDG1_SW_Pos             (4U)
#define STM32H7_FLASH_OPTSR_IWDG1_SW_Msk             (0x1UL << STM32H7_FLASH_OPTSR_IWDG1_SW_Pos) /*!< 0x00000010 */
#define STM32H7_FLASH_OPTSR_IWDG1_SW                 STM32H7_FLASH_OPTSR_IWDG1_SW_Msk            /*!< IWDG1 control mode option status bit */
#define STM32H7_FLASH_OPTSR_NRST_STOP_D1_Pos         (6U)
#define STM32H7_FLASH_OPTSR_NRST_STOP_D1_Msk         (0x1UL << STM32H7_FLASH_OPTSR_NRST_STOP_D1_Pos) /*!< 0x00000040 */
#define STM32H7_FLASH_OPTSR_NRST_STOP_D1             STM32H7_FLASH_OPTSR_NRST_STOP_D1_Msk            /*!< D1 domain DStop entry reset option status bit */
#define STM32H7_FLASH_OPTSR_NRST_STBY_D1_Pos         (7U)
#define STM32H7_FLASH_OPTSR_NRST_STBY_D1_Msk         (0x1UL << STM32H7_FLASH_OPTSR_NRST_STBY_D1_Pos) /*!< 0x00000080 */
#define STM32H7_FLASH_OPTSR_NRST_STBY_D1             STM32H7_FLASH_OPTSR_NRST_STBY_D1_Msk            /*!< D1 domain DStandby entry reset option status bit */
#define STM32H7_FLASH_OPTSR_RDP_Pos                  (8U)
#define STM32H7_FLASH_OPTSR_RDP_Msk                  (0xFFUL << STM32H7_FLASH_OPTSR_RDP_Pos)   /*!< 0x0000FF00 */
#define STM32H7_FLASH_OPTSR_RDP                      STM32H7_FLASH_OPTSR_RDP_Msk               /*!< Readout protection level option status byte */
#define STM32H7_FLASH_OPTSR_FZ_IWDG_STOP_Pos         (17U)
#define STM32H7_FLASH_OPTSR_FZ_IWDG_STOP_Msk         (0x1UL << STM32H7_FLASH_OPTSR_FZ_IWDG_STOP_Pos) /*!< 0x00020000 */
#define STM32H7_FLASH_OPTSR_FZ_IWDG_STOP             STM32H7_FLASH_OPTSR_FZ_IWDG_STOP_Msk            /*!< IWDG Stop mode freeze option status bit */
#define STM32H7_FLASH_OPTSR_FZ_IWDG_SDBY_Pos         (18U)
#define STM32H7_FLASH_OPTSR_FZ_IWDG_SDBY_Msk         (0x1UL << STM32H7_FLASH_OPTSR_FZ_IWDG_SDBY_Pos) /*!< 0x00040000 */
#define STM32H7_FLASH_OPTSR_FZ_IWDG_SDBY             FLASH_OPTSR_FZ_IWDG_SDBY_Msk            /*!< IWDG Standby mode freeze option status bit */
#define STM32H7_FLASH_OPTSR_ST_RAM_SIZE_Pos          (19U)
#define STM32H7_FLASH_OPTSR_ST_RAM_SIZE_Msk          (0x3UL << STM32H7_FLASH_OPTSR_ST_RAM_SIZE_Pos) /*!< 0x00180000 */
#define STM32H7_FLASH_OPTSR_ST_RAM_SIZE              STM32H7_FLASH_OPTSR_ST_RAM_SIZE_Msk            /*!< ST RAM size option status */
#define STM32H7_FLASH_OPTSR_ST_RAM_SIZE_0            (0x1UL << STM32H7_FLASH_OPTSR_ST_RAM_SIZE_Pos) /*!< 0x00080000 */
#define STM32H7_FLASH_OPTSR_ST_RAM_SIZE_1            (0x2UL << STM32H7_FLASH_OPTSR_ST_RAM_SIZE_Pos) /*!< 0x00100000 */
#define STM32H7_FLASH_OPTSR_SECURITY_Pos             (21U)
#define STM32H7_FLASH_OPTSR_SECURITY_Msk             (0x1UL << STM32H7_FLASH_OPTSR_SECURITY_Pos) /*!< 0x00200000 */
#define STM32H7_FLASH_OPTSR_SECURITY                 STM32H7_FLASH_OPTSR_SECURITY_Msk            /*!< Security enable option status bit */
#define STM32H7_FLASH_OPTSR_IO_HSLV_Pos              (29U)
#define STM32H7_FLASH_OPTSR_IO_HSLV_Msk              (0x1UL << STM32H7_FLASH_OPTSR_IO_HSLV_Pos) /*!< 0x20000000 */
#define STM32H7_FLASH_OPTSR_IO_HSLV                  STM32H7_FLASH_OPTSR_IO_HSLV_Msk            /*!< I/O high-speed at low-voltage status bit */
#define STM32H7_FLASH_OPTSR_OPTCHANGEERR_Pos         (30U)
#define STM32H7_FLASH_OPTSR_OPTCHANGEERR_Msk         (0x1UL << STM32H7_FLASH_OPTSR_OPTCHANGEERR_Pos) /*!< 0x40000000 */
#define STM32H7_FLASH_OPTSR_OPTCHANGEERR             STM32H7_FLASH_OPTSR_OPTCHANGEERR_Msk            /*!< Option byte change error flag */
#define STM32H7_FLASH_OPTSR_SWAP_BANK_OPT_Pos        (31U)
#define STM32H7_FLASH_OPTSR_SWAP_BANK_OPT_Msk        (0x1UL << STM32H7_FLASH_OPTSR_SWAP_BANK_OPT_Pos) /*!< 0x80000000 */
#define STM32H7_FLASH_OPTSR_SWAP_BANK_OPT            STM32H7_FLASH_OPTSR_SWAP_BANK_OPT_Msk            /*!< Bank swapping option status bit */

/*******************  Bits definition for FLASH_OPTCCR register  *******************/
#define STM32H7_FLASH_OPTCCR_CLR_OPTCHANGEERR_Pos    (30U)
#define STM32H7_FLASH_OPTCCR_CLR_OPTCHANGEERR_Msk    (0x1UL << STM32H7_FLASH_OPTCCR_CLR_OPTCHANGEERR_Pos) /*!< 0x40000000 */
#define STM32H7_FLASH_OPTCCR_CLR_OPTCHANGEERR        STM32H7_FLASH_OPTCCR_CLR_OPTCHANGEERR_Msk            /*!< OPTCHANGEERR reset bit */


/** @defgroup FLASHEx_Banks FLASH Banks
  * @{
  */
#define STM32H7_FLASH_BANK_1             0x01U                         /*!< Bank 1   */
#if defined (STM32H7_DUAL_BANK)
#define STM32H7_FLASH_BANK_2             0x02U                         /*!< Bank 2   */
#define STM32H7_FLASH_BANK_BOTH          (STM32H7_FLASH_BANK_1 | STM32H7_FLASH_BANK_2) /*!< Bank1 and Bank2 */
#endif /* DUAL_BANK */



/** @defgroup FLASH_Error_Code FLASH Error Code
  * @brief    FLASH Error Code
  * @{
  */
#define STM32H7_FLASH_ERROR_NONE         0x00000000U               /*!< No error                              */

#define STM32H7_FLASH_ERROR_WRP          STM32H7_FLASH_FLAG_WRPERR         /*!< Write Protection Error                */
#define STM32H7_FLASH_ERROR_PGS          STM32H7_FLASH_FLAG_PGSERR         /*!< Program Sequence Error                */
#define STM32H7_FLASH_ERROR_STRB         STM32H7_FLASH_FLAG_STRBERR        /*!< Strobe Error                          */
#define STM32H7_FLASH_ERROR_INC          STM32H7_FLASH_FLAG_INCERR         /*!< Inconsistency Error                   */
#if defined (STM32H7_FLASH_SR_OPERR)
#define STM32H7_FLASH_ERROR_OPE          STM32H7_FLASH_FLAG_OPERR          /*!< Operation Error                       */
#endif /* STM32H7_FLASH_SR_OPERR */
#define STM32H7_FLASH_ERROR_RDP          STM32H7_FLAG_RDPERR         /*!< Read Protection Error                 */
#define STM32H7_FLASH_ERROR_RDS          STM32H7_FLAG_RDSERR         /*!< Read Secured Error                    */
#define STM32H7_FLASH_ERROR_SNECC        STM32H7_FLAG_SNECCERR       /*!< ECC Single Correction Error           */
#define STM32H7_FLASH_ERROR_DBECC        STM32H7_FLAG_DBECCERR       /*!< ECC Double Detection Error            */
#define STM32H7_FLASH_ERROR_CRCRD        STM32H7_FLAG_CRCRDERR       /*!< CRC Read Error                        */

#define STM32H7_FLASH_ERROR_WRP_BANK1    STM32H7_FLASH_FLAG_WRPERR_BANK1   /*!< Write Protection Error on Bank 1      */
#define STM32H7_FLASH_ERROR_PGS_BANK1    STM32H7_FLASH_FLAG_PGSERR_BANK1   /*!< Program Sequence Error on Bank 1      */
#define STM32H7_FLASH_ERROR_STRB_BANK1   STM32H7_FLASH_FLAG_STRBERR_BANK1  /*!< Strobe Error on Bank 1                */
#define STM32H7_FLASH_ERROR_INC_BANK1    STM32H7_FLASH_FLAG_INCERR_BANK1   /*!< Inconsistency Error on Bank 1         */
#if defined (STM32H7_FLASH_SR_OPERR)
#define STM32H7_FLASH_ERROR_OPE_BANK1    STM32H7_FLASH_FLAG_OPERR_BANK1    /*!< Operation Error on Bank 1             */
#endif /* FLASH_SR_OPERR */
#define STM32H7_FLASH_ERROR_RDP_BANK1    STM32H7_FLASH_FLAG_RDPERR_BANK1   /*!< Read Protection Error on Bank 1       */
#define STM32H7_FLASH_ERROR_RDS_BANK1    STM32H7_FLASH_FLAG_RDSERR_BANK1   /*!< Read Secured Error on Bank 1          */
#define STM32H7_FLASH_ERROR_SNECC_BANK1  STM32H7_FLASH_FLAG_SNECCERR_BANK1 /*!< ECC Single Correction Error on Bank 1 */
#define STM32H7_FLASH_ERROR_DBECC_BANK1  STM32H7_FLASH_FLAG_DBECCERR_BANK1 /*!< ECC Double Detection Error on Bank 1  */
#define STM32H7_FLASH_ERROR_CRCRD_BANK1  STM32H7_FLASH_FLAG_CRCRDERR_BANK1 /*!< CRC Read Error on Bank1               */

#define STM32H7_FLASH_ERROR_WRP_BANK2    STM32H7_FLASH_FLAG_WRPERR_BANK2    /*!< Write Protection Error on Bank 2      */
#define STM32H7_FLASH_ERROR_PGS_BANK2    STM32H7_FLASH_FLAG_PGSERR_BANK2    /*!< Program Sequence Error on Bank 2      */
#define STM32H7_FLASH_ERROR_STRB_BANK2   STM32H7_FLASH_FLAG_STRBERR_BANK2   /*!< Strobe Error on Bank 2                */
#define STM32H7_FLASH_ERROR_INC_BANK2    STM32H7_FLASH_FLAG_INCERR_BANK2    /*!< Inconsistency Error on Bank 2         */
#if defined (STM32H7_FLASH_SR_OPERR)
#define STM32H7_FLASH_ERROR_OPE_BANK2    STM32H7_FLASH_FLAG_OPERR_BANK2     /*!< Operation Error on Bank 2             */
#endif /* STM32H7_FLASH_SR_OPERR */
#define STM32H7_FLASH_ERROR_RDP_BANK2    STM32H7_FLASH_FLAG_RDPERR_BANK2    /*!< Read Protection Error on Bank 2       */
#define STM32H7_FLASH_ERROR_RDS_BANK2    STM32H7_FLASH_FLAG_RDSERR_BANK2    /*!< Read Secured Error on Bank 2          */
#define STM32H7_FLASH_ERROR_SNECC_BANK2  STM32H7_FLASH_FLAG_SNECCERR_BANK2  /*!< ECC Single Correction Error on Bank 2 */
#define STM32H7_FLASH_ERROR_DBECC_BANK2  STM32H7_FLASH_FLAG_DBECCERR_BANK2  /*!< ECC Double Detection Error on Bank 2  */
#define STM32H7_FLASH_ERROR_CRCRD_BANK2  STM32H7_FLASH_FLAG_CRCRDERR_BANK2  /*!< CRC Read Error on Bank2               */

#define STM32H7_FLASH_ERROR_OB_CHANGE    STM32H7_FLASH_OPTSR_OPTCHANGEERR   /*!< Option Byte Change Error              */
/**
  * @}
  */

/** @defgroup FLASH_Type_Program FLASH Type Program
  * @{
  */
#define STM32H7_FLASH_TYPEPROGRAM_FLASHWORD  0x01U        /*!< Program a flash word at a specified address */
#if defined (STM32H7_FLASH_OPTCR_PG_OTP)
#define STM32H7_FLASH_TYPEPROGRAM_OTPWORD    0x02U        /*!< Program an OTP word at a specified address  */
#endif /* FLASH_OPTCR_PG_OTP */
/**
  * @}
  */

/** @defgroup FLASH_Flag_definition FLASH Flag definition
  * @brief Flag definition
  * @{
  */
#define STM32H7_FLASH_FLAG_BSY                     STM32H7_FLASH_SR_BSY             /*!< FLASH Busy flag */
#define STM32H7_FLASH_FLAG_WBNE                    STM32H7_FLASH_SR_WBNE            /*!< Write Buffer Not Empty flag */
#define STM32H7_FLASH_FLAG_QW                      STM32H7_FLASH_SR_QW              /*!< Wait Queue on flag */
#define STM32H7_FLASH_FLAG_CRC_BUSY                STM32H7_FLASH_SR_CRC_BUSY        /*!< CRC Busy flag */
#define STM32H7_FLASH_FLAG_EOP                     STM32H7_FLASH_SR_EOP             /*!< End Of Program on flag */
#define STM32H7_FLASH_FLAG_WRPERR                  STM32H7_FLASH_SR_WRPERR          /*!< Write Protection Error on flag */
#define STM32H7_FLASH_FLAG_PGSERR                  STM32H7_FLASH_SR_PGSERR          /*!< Program Sequence Error on flag */
#define STM32H7_FLASH_FLAG_STRBERR                 STM32H7_FLASH_SR_STRBERR         /*!< Strobe Error flag */
#define STM32H7_FLASH_FLAG_INCERR                  STM32H7_FLASH_SR_INCERR          /*!< Inconsistency Error on flag */
#if defined (STM32H7_FLASH_SR_OPERR)
#define STM32H7_FLASH_FLAG_OPERR                   STM32H7_FLASH_SR_OPERR           /*!< Operation Error on flag */
#endif /* FLASH_SR_OPERR */
#define STM32H7_FLASH_FLAG_RDPERR                  STM32H7_FLASH_SR_RDPERR          /*!< Read Protection Error on flag */
#define STM32H7_FLASH_FLAG_RDSERR                  STM32H7_FLASH_SR_RDSERR          /*!< Read Secured Error on flag */
#define STM32H7_FLASH_FLAG_SNECCERR                STM32H7_FLASH_SR_SNECCERR        /*!< Single ECC Error Correction on flag */
#define STM32H7_FLASH_FLAG_DBECCERR                STM32H7_FLASH_SR_DBECCERR        /*!< Double Detection ECC Error on flag */
#define STM32H7_FLASH_FLAG_CRCEND                  STM32H7_FLASH_SR_CRCEND          /*!< CRC End of Calculation flag */
#define STM32H7_FLASH_FLAG_CRCRDERR                STM32H7_FLASH_SR_CRCRDERR        /*!< CRC Read Error on bank flag */

#define STM32H7_FLASH_FLAG_BSY_BANK1               STM32H7_FLASH_SR_BSY             /*!< FLASH Bank 1 Busy flag */
#define STM32H7_FLASH_FLAG_WBNE_BANK1              STM32H7_FLASH_SR_WBNE            /*!< Write Buffer Not Empty on Bank 1 flag */
#define STM32H7_FLASH_FLAG_QW_BANK1                STM32H7_FLASH_SR_QW              /*!< Wait Queue on Bank 1 flag */
#define STM32H7_FLASH_FLAG_CRC_BUSY_BANK1          STM32H7_FLASH_SR_CRC_BUSY        /*!< CRC Busy on Bank 1 flag */
#define STM32H7_FLASH_FLAG_EOP_BANK1               STM32H7_FLASH_SR_EOP             /*!< End Of Program on Bank 1 flag */
#define STM32H7_FLASH_FLAG_WRPERR_BANK1            STM32H7_FLASH_SR_WRPERR          /*!< Write Protection Error on Bank 1 flag */
#define STM32H7_FLASH_FLAG_PGSERR_BANK1            STM32H7_FLASH_SR_PGSERR          /*!< Program Sequence Error on Bank 1 flag */
#define STM32H7_FLASH_FLAG_STRBERR_BANK1           STM32H7_FLASH_SR_STRBERR         /*!< Strobe Error on Bank 1 flag */
#define STM32H7_FLASH_FLAG_INCERR_BANK1            STM32H7_FLASH_SR_INCERR          /*!< Inconsistency Error on Bank 1 flag */
#if defined (STM32H7_FLASH_SR_OPERR)
#define STM32H7_FLASH_FLAG_OPERR_BANK1             STM32H7_FLASH_SR_OPERR           /*!< Operation Error on Bank 1 flag */
#endif /* FLASH_SR_OPERR */
#define STM32H7_FLASH_FLAG_RDPERR_BANK1            STM32H7_FLASH_SR_RDPERR          /*!< Read Protection Error on Bank 1 flag */
#define STM32H7_FLASH_FLAG_RDSERR_BANK1            STM32H7_FLASH_SR_RDSERR          /*!< Read Secured Error on Bank 1 flag */
#define STM32H7_FLASH_FLAG_SNECCERR_BANK1          STM32H7_FLASH_SR_SNECCERR        /*!< Single ECC Error Correction on Bank 1 flag */
#define STM32H7_FLASH_FLAG_DBECCERR_BANK1          STM32H7_FLASH_SR_DBECCERR        /*!< Double Detection ECC Error on Bank 1 flag */
#define STM32H7_FLASH_FLAG_CRCEND_BANK1            STM32H7_FLASH_SR_CRCEND          /*!< CRC End of Calculation on Bank 1 flag */
#define STM32H7_FLASH_FLAG_CRCRDERR_BANK1          STM32H7_FLASH_SR_CRCRDERR        /*!< CRC Read error on Bank 1 flag */

#if defined (STM32H7_FLASH_SR_OPERR)
#define STM32H7_FLASH_FLAG_ALL_ERRORS_BANK1       (STM32H7_FLASH_FLAG_WRPERR_BANK1   | STM32H7_FLASH_FLAG_PGSERR_BANK1   | \
                                           STM32H7_FLASH_FLAG_STRBERR_BANK1  | STM32H7_FLASH_FLAG_INCERR_BANK1   | \
                                           STM32H7_FLASH_FLAG_OPERR_BANK1    | STM32H7_FLASH_FLAG_RDPERR_BANK1   | \
                                           STM32H7_FLASH_FLAG_RDSERR_BANK1   | STM32H7_FLASH_FLAG_SNECCERR_BANK1 | \
                                           STM32H7_FLASH_FLAG_DBECCERR_BANK1 | STM32H7_FLASH_FLAG_CRCRDERR_BANK1) /*!< All Bank 1 error flags */
#else
#define FLASH_FLAG_ALL_ERRORS_BANK1       (STM32H7_FLASH_FLAG_WRPERR_BANK1   | STM32H7_FLASH_FLAG_PGSERR_BANK1   | \
                                           STM32H7_FLASH_FLAG_STRBERR_BANK1  | STM32H7_FLASH_FLAG_INCERR_BANK1   | \
                                           STM32H7_FLASH_FLAG_RDPERR_BANK1   | STM32H7_FLASH_FLAG_RDSERR_BANK1   | \
                                           STM32H7_FLASH_FLAG_SNECCERR_BANK1 | STM32H7_FLASH_FLAG_DBECCERR_BANK1 | \
                                           STM32H7_FLASH_FLAG_CRCRDERR_BANK1) /*!< All Bank 1 error flags */
#endif /* STM32H7_FLASH_SR_OPERR */

#define STM32H7_FLASH_FLAG_ALL_BANK1              (STM32H7_FLASH_FLAG_BSY_BANK1      | STM32H7_FLASH_FLAG_WBNE_BANK1     | \
                                           STM32H7_FLASH_FLAG_QW_BANK1       | STM32H7_FLASH_FLAG_CRC_BUSY_BANK1 | \
                                           STM32H7_FLASH_FLAG_EOP_BANK1      | STM32H7_FLASH_FLAG_CRCEND_BANK1   | \
                                           STM32H7_FLASH_FLAG_ALL_ERRORS_BANK1) /*!< All Bank 1 flags */

#define STM32H7_FLASH_FLAG_BSY_BANK2               (STM32H7_FLASH_SR_BSY      | 0x80000000U)        /*!< FLASH Bank 2 Busy flag */
#define STM32H7_FLASH_FLAG_WBNE_BANK2              (STM32H7_FLASH_SR_WBNE     | 0x80000000U)        /*!< Write Buffer Not Empty on Bank 2 flag */
#define STM32H7_FLASH_FLAG_QW_BANK2                (STM32H7_FLASH_SR_QW       | 0x80000000U)        /*!< Wait Queue on Bank 2 flag */
#define STM32H7_FLASH_FLAG_CRC_BUSY_BANK2          (STM32H7_FLASH_SR_CRC_BUSY | 0x80000000U)        /*!< CRC Busy on Bank 2 flag */
#define STM32H7_FLASH_FLAG_EOP_BANK2               (STM32H7_FLASH_SR_EOP      | 0x80000000U)        /*!< End Of Program on Bank 2 flag */
#define STM32H7_FLASH_FLAG_WRPERR_BANK2            (STM32H7_FLASH_SR_WRPERR   | 0x80000000U)        /*!< Write Protection Error on Bank 2 flag */
#define STM32H7_FLASH_FLAG_PGSERR_BANK2            (STM32H7_FLASH_SR_PGSERR   | 0x80000000U)        /*!< Program Sequence Error on Bank 2 flag */
#define STM32H7_FLASH_FLAG_STRBERR_BANK2           (STM32H7_FLASH_SR_STRBERR  | 0x80000000U)        /*!< Strobe Error on Bank 2 flag */
#define STM32H7_FLASH_FLAG_INCERR_BANK2            (STM32H7_FLASH_SR_INCERR   | 0x80000000U)        /*!< Inconsistency Error on Bank 2 flag */
#if defined (STM32H7_FLASH_SR_OPERR)
#define STM32H7_FLASH_FLAG_OPERR_BANK2             (STM32H7_FLASH_SR_OPERR    | 0x80000000U)        /*!< Operation Error on Bank 2 flag */
#endif /* FLASH_SR_OPERR */
#define STM32H7_FLASH_FLAG_RDPERR_BANK2            (STM32H7_FLASH_SR_RDPERR   | 0x80000000U)        /*!< Read Protection Error on Bank 2 flag */
#define STM32H7_FLASH_FLAG_RDSERR_BANK2            (STM32H7_FLASH_SR_RDSERR   | 0x80000000U)        /*!< Read Secured Error on Bank 2 flag */
#define STM32H7_FLASH_FLAG_SNECCERR_BANK2          (STM32H7_FLASH_SR_SNECCERR | 0x80000000U)        /*!< Single ECC Error Correction on Bank 2 flag */
#define STM32H7_FLASH_FLAG_DBECCERR_BANK2          (STM32H7_FLASH_SR_DBECCERR | 0x80000000U)        /*!< Double Detection ECC Error on Bank 2 flag */
#define STM32H7_FLASH_FLAG_CRCEND_BANK2            (STM32H7_FLASH_SR_CRCEND   | 0x80000000U)        /*!< CRC End of Calculation on Bank 2 flag */
#define STM32H7_FLASH_FLAG_CRCRDERR_BANK2          (STM32H7_FLASH_SR_CRCRDERR | 0x80000000U)        /*!< CRC Read error on Bank 2 flag */

#if defined (STM32H7_FLASH_SR_OPERR)
#define STM32H7_FLASH_FLAG_ALL_ERRORS_BANK2       (STM32H7_FLASH_FLAG_WRPERR_BANK2   | STM32H7_FLASH_FLAG_PGSERR_BANK2   | \
                                           STM32H7_FLASH_FLAG_STRBERR_BANK2  | STM32H7_FLASH_FLAG_INCERR_BANK2   | \
                                           STM32H7_FLASH_FLAG_OPERR_BANK2    | STM32H7_FLASH_FLAG_RDPERR_BANK2   | \
                                           STM32H7_FLASH_FLAG_RDSERR_BANK2   | STM32H7_FLASH_FLAG_SNECCERR_BANK2 | \
                                           STM32H7_FLASH_FLAG_DBECCERR_BANK2 | STM32H7_FLASH_FLAG_CRCRDERR_BANK2) /*!< All Bank 2 error flags */
#else
#define STM32H7_FLASH_FLAG_ALL_ERRORS_BANK2       (STM32H7_FLASH_FLAG_WRPERR_BANK2   | STM32H7_FLASH_FLAG_PGSERR_BANK2   | \
                                           STM32H7_FLASH_FLAG_STRBERR_BANK2  | STM32H7_FLASH_FLAG_INCERR_BANK2   | \
                                           STM32H7_FLASH_FLAG_RDPERR_BANK2   | STM32H7_FLASH_FLAG_RDSERR_BANK2   | \
                                          STM32H7_FLASH_FLAG_SNECCERR_BANK2 | STM32H7_FLASH_FLAG_DBECCERR_BANK2 | \
                                           STM32H7_FLASH_FLAG_CRCRDERR_BANK2) /*!< All Bank 2 error flags */
#endif /* FLASH_SR_OPERR */

#define STM32H7_FLASH_FLAG_ALL_BANK2              (STM32H7_FLASH_FLAG_BSY_BANK2      | STM32H7_FLASH_FLAG_WBNE_BANK2     | \
                                           STM32H7_FLASH_FLAG_QW_BANK2       | STM32H7_FLASH_FLAG_CRC_BUSY_BANK2 | \
                                           STM32H7_FLASH_FLAG_EOP_BANK2      | STM32H7_FLASH_FLAG_CRCEND_BANK2   | \
                                           STM32H7_FLASH_FLAG_ALL_ERRORS_BANK2) /*!< All Bank 2 flags */

void Stm32h7_Flash_MassErase(uint32_t VoltageRange, uint32_t Banks);
bool Stm32h7_Flash_Program(uint32_t FlashAddress, uint32_t DataAddress, uint32_t WordSize);
bool Stm32h7_Flash_Unlock(uint32_t Banks);
bool Stm32h7_Flash_Lock(uint32_t Banks);
bool Stm32h7_Flash_WaitOperation(uint32_t Timeout, uint32_t Bank);
bool Stm32h7_Flash_OB_Lock(void);
bool Stm32h7_Flash_OB_Unlock(void);
uint32_t Stm32h7_Flash_OB_GetUser(void);
void Stm32h7_Flash_OB_OptrConfig(uint32_t UserType, uint32_t UserConfig, uint32_t RDPLevel);
void Stm32h7_Flash_OB_Launch(void);
bool Stm32h7_Flash_OB_Program(uint32_t RDPLevel);

#endif /* STM32H7_FLASH_H_ */
