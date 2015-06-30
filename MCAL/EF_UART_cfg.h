 /***************************************************************
 *  Source File: EF_UART_cfg.h
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

#ifndef UART_CFG_H
#define UART_CFG_H
 
#include <avr/interrupt.h>
#include "../ServiceLayer/std_types.h"

/**************************************************
 * Predefine variables
 **************************************************/
#define NO_PARITY     0
#define EVEN_PARITY   2
#define ODD_PARITY	  3

#define ONE_STOP_BIT  0
#define TWO_STOP_BITS 1


/**************************************************
 * configuration types
 *************************************************/
/* structure is passed in the void_UART_init argument*/
typedef struct
{
	U32_t baudrate;			   /* put here the normal Baud rate as 9600      */
	U8_t numberOfDataBits;     /* take number from 5 to 9					 */
	U8_t stopBits;			   /* take one of #defined above as ONE_STOP_BIT */
	U8_t parity;			   /* take one of #defined above as NO_PARITY    */
	BOOLEAN RXInterruptEnable; /* take 1 if enable and 0 if disable			 */
	BOOLEAN TXInterruptEnable; /* take 1 if enable and 0 if disable			 */
	BOOLEAN ReceiverEnable;    /* take 1 if enable and 0 if disable			 */
	BOOLEAN TransmitterEnable; /* take 1 if enable and 0 if disable			 */
}UART_cfg_str;


/**************************************************
 * register types
 *************************************************/

/* using this structure to map the UART registers */
typedef struct
{
	volatile U8_t * UDR_Reg   ;
	volatile U8_t * UCSRA_Reg ;
	volatile U8_t * UCSRB_Reg ;
	volatile U8_t * UCSRC_Reg ;
	volatile U8_t * UBRRH_Reg ;
	volatile U8_t * UBRRL_Reg ;
}UART_reg_str;

#endif
