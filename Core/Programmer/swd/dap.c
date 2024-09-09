/*******************************************************************************
 * @file dap.c
 * @brief Low level SWD interface functions.
 * @author Silicon Labs
 * @version 1.03
 *******************************************************************************
 * @section License
 * <b>(C) Copyright 2014 Silicon Labs, http://www.silabs.com</b>
 *******************************************************************************
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 *
 * DISCLAIMER OF WARRANTY/LIMITATION OF REMEDIES: Silicon Labs has no
 * obligation to support this Software. Silicon Labs is providing the
 * Software "AS IS", with no express or implied warranties of any kind,
 * including, but not limited to, any implied warranties of merchantability
 * or fitness for any particular purpose or warranties against infringement
 * of any proprietary rights of a third party.
 *
 * Silicon Labs will not be liable for any consequential, incidental, or
 * special damages, or any other relief, or for any claim by any third party,
 * arising from your use of this Software.
 *
 ******************************************************************************/
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <setjmp.h>
#include "dap.h"
#include "errors.h"
#include "main.h"


/* These codes are only needed for SWD protocol version 2 with multidrop */
static uint32_t selectionAlertSequence[] = { 0x6209F392, 0x86852D95, 0xE3DDAFE9, 0x19BC0EA2};
static uint32_t activationCode = 0x1A;


/**********************************************************
 * Reads from an AP or DP register.
 * 
 * @param ap
 *   If this parameter is true, read from AP register. 
 *   If false read from DP register. 
 *
 * @param reg
 *   The register number [0-3] to read from
 * 
 * @param data[out]
 *   The register value is written to this parameter
 **********************************************************/    
static uint32_t readReg(bool ap, int reg, uint32_t *data)
{
  int i;
  uint32_t cb = 0;
  uint32_t parity;
  uint32_t b;
  uint32_t ack = 0;
  uint32_t ret = SWD_ERROR_OK;
   
  /* Initalize output variable */
  *data = 0;
  
  /* Convert to int */
  int _ap = (int)ap;
  int _read = (int)1;
  
  int A2 = reg & 0x1;
  int A3 = (reg >> 1) & 0x1;
  
  /* Calulate parity */
  parity = (_ap + _read + A2 + A3) & 0x1;
  
  
  SWDIO_SET_OUTPUT();
  
  /* Send request */
  SWD_WRITE_BIT(1);
  SWD_WRITE_BIT(_ap);
  SWD_WRITE_BIT(_read);
  SWD_WRITE_BIT(A2);
  SWD_WRITE_BIT(A3);
  SWD_WRITE_BIT(parity);
  SWD_WRITE_BIT(0);
  SWD_WRITE_BIT(1);
  
  /* Turnaround */
  SWDIO_SET_INPUT();
  SWCLK_CYCLE();
  
  /* Read ACK */
  for ( i=0; i<3; i++ ) {
	  SWD_READ_BIT(b);
    ack |= b << i;
  }
  
  /* Verify that ACK is OK */
  if ( ack == ACK_OK ) {
  
    for ( i=0; i<32; i++ ) 
    {
      /* Read bit */
    	SWD_READ_BIT(b);
      *data |= b << i;
      
      /* Keep track of expected parity */
      if ( b ) cb = !cb;
    }
    
    /* Read parity bit */
    SWD_READ_BIT(parity);
    
    /* Verify parity */
    if ( cb == parity ) 
    {
      ret = SWD_ERROR_OK;
    } else {
      ret = SWD_ERROR_PARITY;
    }
    
  } 
  else if ( ack == ACK_WAIT ) 
  {
    ret = SWD_ERROR_WAIT;
  } 
  else if ( ack == ACK_FAULT ) 
  {
    ret = SWD_ERROR_FAULT;
  } 
  else 
  {
    /* Line not driven. Protocol error */
    ret = SWD_ERROR_PROTOCOL;
  }
  
  /* Turnaround */
  SWCLK_CYCLE();
  
  /* 8-cycle idle period. Make sure transaction
   * is clocked through DAP. */
  SWDIO_SET_OUTPUT();
  for ( i=0; i<8; i++ ) {
    SWD_WRITE_BIT(0);
  }
  
  return ret;
}


/**********************************************************
 * Writes to a DP or AP register.
 * 
 * @param ap
 *   If this parameter is true, write to AP register. 
 *   If false write to DP register. 
 *
 * @param reg
 *   The register number [0-3] to write to
 * 
 * @param data
 *   The value to write to the register
 **********************************************************/
static uint32_t writeReg(bool ap, int reg, uint32_t data, bool ignoreAck)
{
  uint32_t ack = 0;
  int i;
  uint32_t parity = 0;
  uint32_t b;
  uint32_t ret = SWD_ERROR_OK;
  
  /* Convert to int */
  int _ap = (int)ap;
  int _read = (int)0;
  
  /* Calulate address bits */
  int A2 = reg & 0x1;
  int A3 = (reg >> 1) & 0x1;
  
  /* Calculate parity */
  parity = (_ap + _read + A2 + A3) & 0x1;
  
  SWDIO_SET_OUTPUT();
  
  /* Write request */
  SWD_WRITE_BIT(1);
  SWD_WRITE_BIT(_ap);
  SWD_WRITE_BIT(_read);
  SWD_WRITE_BIT(A2);
  SWD_WRITE_BIT(A3);
  SWD_WRITE_BIT(parity);
  SWD_WRITE_BIT(0);
  SWD_WRITE_BIT(1);
  
  SWDIO_SET_INPUT(); 
  
  /* Turnaround */
  SWCLK_CYCLE();
  
  /* Read acknowledge */
  for ( i=0; i<3; i++ ) {
	  SWD_READ_BIT(b);
    ack |= b << i;
  }
  
  if ( ack == ACK_OK || ignoreAck ) 
  {
    /* Turnaround */
    SWCLK_CYCLE();
  
    SWDIO_SET_OUTPUT();
    
    /* Write data */
    parity = 0;
    for ( i=0; i<32; i++ ) {
      b = (data >> i) & 0x1;
      SWD_WRITE_BIT(b);
      if (b) parity = !parity;
    }
    
    /* Write parity bit */
    SWD_WRITE_BIT(parity);
  
  } 
  else if ( ack == ACK_WAIT ) 
  {
    ret = SWD_ERROR_WAIT;
  } 
  else if ( ack == ACK_FAULT ) 
  {
    ret = SWD_ERROR_FAULT;
  } 
  else {
    /* Line not driven. Protocol error */
    ret = SWD_ERROR_PROTOCOL;
  }
    
  
  /* 8-cycle idle period. Make sure transaction
   * is clocked through DAP. */
  SWDIO_SET_OUTPUT();
  for ( i=0; i<8; i++ ) {
	  SWD_WRITE_BIT(0);
  }
  
  return ret;
  
}


/**********************************************************
 * Sends the JTAG-to-SWD sequence. This must be performed
 * at the very beginning of every debug session and
 * again in case of a protocol error. 
 **********************************************************/
void JTAG_to_SWD_Sequence(void)
{
  int i;
  int b;
  
  SWDIO_SET_OUTPUT();
  
  /* First reset line with > 50 cycles with SWDIO high */
  SWDIO_OUT(1);
  for ( i=0; i<60; i++ ) {
    SWCLK_CYCLE();
  }
  
  /* Transmit 16-bit JTAG-to-SWD sequence */
  for ( i=0; i<16; i++ ) {
    b = (JTAG2SWD >> i) & 0x1;
    SWD_WRITE_BIT(b);
  }
  
  /* Do another reset to make sure SW-DP is in reset state */
  SWDIO_OUT(1);
  for ( i=0; i<60; i++ ) {
    SWCLK_CYCLE();
  }
  
  /* Insert a 16 cycle idle period */
  SWDIO_OUT(0);
  for ( i=0; i<16; i++ ) {
    SWCLK_CYCLE();
  }
}




/**********************************************************
 * This function is used to take the target DAP from 
 * 'dormant' state to 'reset' state. It is only used
 * on DAPs that implement SWD protocol version 2 (or
 * later) that implements multidrop. 
 **********************************************************/
void exitDormantState(void)
{
  int i,j,b;
  
  SWDIO_SET_OUTPUT();
  
  /* At least 8 cycles with SWDIO high */
  for ( i=0; i<10; i++ ) {
    SWD_WRITE_BIT(1);
  }
  
  /* 128-bt selection alert sequence */
  for ( i=0; i<4; i++ )
  {
    for ( j=0; j<32; j++ )
    {
      b = (selectionAlertSequence[i] >> j) & 0x1;
      SWD_WRITE_BIT(b);
    }
  }
  
  /* Four cycles with SWDIO low */
  for ( i=0; i<4; i++ )
  {
	  SWD_WRITE_BIT(0);
  }
  
  /* Serial Wire Activation code */
  for ( i=0; i<8; i++ )
  {
    b = (activationCode >> i) & 0x1;
    SWD_WRITE_BIT(b);
  }
  
  /* Line reset */
  for ( i=0; i<60; i++ ) {
	  SWD_WRITE_BIT(1);
  }
}
  

/**********************************************************
 * Writes to one of the four AP registers in the currently
 * selected AP bank.
 * 
 * @param reg[in]
 *    The register number [0-3] to write to
 * 
 * @param data[in]
 *    Value to write to the register
 * 
 **********************************************************/
void writeAP(int reg, uint32_t data)
{
  uint32_t swdStatus;
  uint32_t retry = SWD_RETRY_COUNT;

  do {
    swdStatus = writeReg(true, reg, data, false);
    retry--;
  } while ( swdStatus == SWD_ERROR_WAIT && retry > 0 );  
  
  if ( swdStatus != SWD_ERROR_OK ) 
  {
    RAISE(swdStatus);
  }
}


/**********************************************************
 * Writes to one of the four DP registers.
 * 
 * @param reg[in]
 *    The register number [0-3] to write to
 * 
 * @param data[in]
 *    Value to write to the register
 * 
 **********************************************************/
void writeDP(int reg, uint32_t data)
{
  uint32_t swdStatus;
  uint32_t retry = SWD_RETRY_COUNT;
  
  do {
    swdStatus = writeReg(false, reg, data, false);
    retry--;
  } while ( swdStatus == SWD_ERROR_WAIT && retry > 0 ); 
  
  if ( swdStatus != SWD_ERROR_OK ) 
  {
    RAISE(swdStatus);
  }
}


/**********************************************************
 * Writes to one of the four DP registers and ignores
 * the ACK response. This is needed when writing
 * to the TARGETSEL register.
 * 
 * @param reg[in]
 *    The register number [0-3] to write to
 * 
 * @param data[in]
 *    Value to write to the register
 * 
 **********************************************************/
void writeDpIgnoreAck(int reg, uint32_t data)
{
  uint32_t swdStatus;
  uint32_t retry = SWD_RETRY_COUNT;
  
  do {
    swdStatus = writeReg(false, reg, data, true);
    retry--;
  } while ( swdStatus == SWD_ERROR_WAIT && retry > 0 ); 
  
  if ( swdStatus != SWD_ERROR_OK ) 
  {
    RAISE(swdStatus);
  }
}




/**********************************************************
 * Reads one of the four AP registers in the currently
 * selected AP bank.
 * 
 * @param reg[in]
 *    The register number [0-3] to read
 * 
 * @param data[out]
 *    Value of register is written to this parameter
 * 
 **********************************************************/
void readAP(int reg, uint32_t *data)
{
  uint32_t swdStatus;
  uint32_t retry = SWD_RETRY_COUNT;
  
  do {
    swdStatus = readReg(true, reg, data);
    retry--;
  } while ( swdStatus == SWD_ERROR_WAIT && retry > 0 ); 
  
  if ( swdStatus != SWD_ERROR_OK ) 
  {
    RAISE(swdStatus);
  }
}


/**********************************************************
 * Reads one of the four DP registers.
 * 
 * @param reg[in]
 *    The register number [0-3] to read
 * 
 * @param data[out]
 *    Value of register is written to this parameter
 * 
 **********************************************************/
void readDP(int reg, uint32_t *data)
{
  uint32_t swdStatus;
  uint32_t retry = SWD_RETRY_COUNT;
  
  do {
    swdStatus = readReg(false, reg, data);
    retry--;
  } while ( swdStatus == SWD_ERROR_WAIT && retry > 0 ); 
  
  if ( swdStatus != SWD_ERROR_OK ) {
    RAISE(swdStatus);
  }
}


/**********************************************************
 * Initalized the SW-DP. This function performs first
 * sends the JTAG-to-SWD sequence and then reads
 * the IDCODE register. 
 * 
 * @returns
 *    The value of the IDCODE register
 **********************************************************/
uint32_t test = 0;
uint32_t initDp(void)
{
  uint32_t dpId = 0;
  
  /* If connecting to a Multidrop capable SW-DP (SWD version 2), this 
   * function must be run first. */
  //exitDormantState();
  
  /* Send the JTAG-to-SWD switching sequence */
  JTAG_to_SWD_Sequence();
  
  /* Read IDCODE to get the DAP out of reset state */
  readDP(DP_IDCODE, &dpId);

  /* Debug power up request */
  //khchoi: Sticky error clear & CTRL/STAT Register Clear.(Black Magic Probe)
  writeDP(DP_ABORT, (1 << 2));
  //writeDP(DP_ABORT, (1 << 3)); //wderrclr
  readDP(DP_CTRL, &test);
  writeDP(DP_CTRL, 0);
  readDP(DP_CTRL, &test);
  writeDP(DP_CTRL, DP_CTRL_CSYSPWRUPREQ | DP_CTRL_CDBGPWRUPREQ);

  /* Wait until we receive powerup ACK */
  int retry = PWRUP_TIMEOUT + 200;
  uint32_t status;
  while ( retry > 0 )
  {
    readDP(DP_CTRL, &status);
    if ( (status & (DP_CTRL_CDBGPWRUPACK | DP_CTRL_CSYSPWRUPACK)) 
               == (DP_CTRL_CDBGPWRUPACK | DP_CTRL_CSYSPWRUPACK) )
    {
      break;
    }
    
    retry--;
  }
  
  /* Throw error if we failed to power up the debug interface */
  if ( (status & (DP_CTRL_CDBGPWRUPACK | DP_CTRL_CSYSPWRUPACK)) 
              != (DP_CTRL_CDBGPWRUPACK | DP_CTRL_CSYSPWRUPACK) )
  {
    RAISE(SWD_ERROR_DEBUG_POWER);
  }

  /* Select first AP bank */
  writeDP(DP_SELECT, 0x00);

  return dpId;
}

/**********************************************************
 * Reads the ID of AP #0. This will be either
 *   - EFM32_AHBAP_ID  if device is unlocked
 *   - EFM32_AAP_ID    if device is locked
 * 
 * Note: on Zero Gecko it will always be EFM32_AHBAP_ID
 * and the AAP must be access as a memory mapped peripheral
 * 
 * @returns
 *    The value of IDR register (address 0xFC) for AP #0
 **********************************************************/
uint32_t readApId(void)
{
  uint32_t apId;
  
  /* Select last AP bank */
  writeDP(DP_SELECT, 0xf0);
  
  /* Dummy read AP ID */
  readAP(AP_IDR, &apId);
  
  /* Read AP ID */
  readDP(DP_RDBUFF, &apId);
  
  /* Select first AP bank again */
  writeDP(DP_SELECT, 0x00);
  
  return apId;
}

/**********************************************************
 * Sends the AAP Window Expansion Sequence. This sequence
 * should be sent on SWDIO/SWCLK when reset is held low.
 * This will cause the AAP window to be 255 times longer
 * when reset is released. 
 **********************************************************/
void aapExtensionSequence(void)
{
  int i;
  
  SWCLK_SET();
  
  for ( i=0; i<4; i++ ) {
    SWDIO_CYCLE();
  }
  
  SWCLK_CLR();
  
  for ( i=0; i<4; i++ ) {
    SWDIO_CYCLE();
  }
  
  SWCLK_CLR();
}


/**********************************************************
 * Initalize the AHB-AP. The transfer size must be set
 * to 32-bit before trying to access any internal
 * memory. 
 **********************************************************/
void initAhbAp(void)
{    
  /* Set transfer size to 32 bit */  
  writeAP(AP_CSW, AP_CSW_DEFAULT);  
}




void swdio_in_mode(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin = SWD_IO_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(SWD_IO_GPIO_Port, &GPIO_InitStruct);
}

void swdio_out_mode(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin = SWD_IO_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(SWD_IO_GPIO_Port, &GPIO_InitStruct);
}

