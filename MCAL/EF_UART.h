 /***************************************************************
 *  Source File: EF_UART.h
 *
 *  Description: UART driver for ATMEGA32
 *
 *  History:     Version 1.0  - INIT Version
 *  Date   :     25/08/2014
 *  -------------------------------------------------------
 *  Author :     EmbeddedFab.
 *
 *  Copyright (C) <2014>  <EmbeddedFab>
 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>
 
 **************************************************************/
#ifndef UART_H
#define UART_H

#include "EF_UART_cfg.h"
/****************************************************************************
* Function    : EF_void_UART_Init
*
* DESCRIPTION : init the UART
*
* PARAMETERS  : uart_cfg : pointer to UART_cfg_str which has parameters to init the UART
*
* Return Value: None.
******************************************************************************/
extern void EF_void_UART_Init(UART_cfg_str *uart_cfg);

/****************************************************************************
* Function    : EF_BOOLEAN_UART_PutChar
*
* DESCRIPTION : Sending the TX byte ,used timer to be unStuck
*
* PARAMETERS  : data : TX byte
*
* Return Value: return True if Byte is transmitted or false
*
* NOTE		  : special timer must be initialized and enable Global interrupt
* 			    before using this function
******************************************************************************/
extern BOOLEAN EF_BOOLEAN_UART_PutChar(U8_t data);

/****************************************************************************
* Function    : EF_BOOLEAN_UART_GetChar
*
* DESCRIPTION : Getting the RX byte ,used timer to be unStuck
*
* PARAMETERS  : returnedValue: pointer to the Rx byte
*
* Return Value: return True if Byte is received or false
*
* NOTE		  : special timer must be initialized and enable Global interrupt
* 			    before using this function
******************************************************************************/
extern BOOLEAN EF_BOOLEAN_UART_GetChar(U8_t * returnedValue);


extern BOOLEAN EF_BOOLEAN_UART_CheckForRxData( );
/****************************************************************************
* Function    : EF_u8_UART_WaitChar
*
* DESCRIPTION :  Getting the RX byte
*
* PARAMETERS  : None
*
* Return Value: U8_t : Rx byte
******************************************************************************/
extern U8_t EF_u8_UART_WaitChar();
/****************************************************************************
* Function    : EF_void_UART_PutChar
*
* DESCRIPTION : Sending the TX byte
*
* PARAMETERS  : Tx_byte
*
* Return Value: None
*
******************************************************************************/
extern void EF_void_UART_PutChar(U8_t data);
/****************************************************************************
* Function    : EF_void_UART_SendArray
*
* DESCRIPTION : sending arrays of bytes
*
*
* PARAMETERS  : array 	: pointer to TX data
*				Length	: length of array
*
* Return Value: None
*
******************************************************************************/
void EF_void_UART_SendArray(U8_t *array, U8_t Length);


void EF_void_UART_HextoASCII(U8_t *hex);

#endif
