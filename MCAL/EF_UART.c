 /***************************************************************
 *  Source File: EF_UART.c
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
#include <avr/io.h>
#include "EF_UART.h"
#include "EF_SpechialTIMER.h"
#include "../ServiceLayer/std_types.h"
//#include "../HAL/EF_LCD.h"
#include <util/delay.h>

/**************************************************
 * Global Variables & Definitions
 *************************************************/
/* structure has the addresses of UART Registers */
const UART_reg_str uart_reg = {&UDR,&UCSRA,&UCSRB,&UCSRC,&UBRRH,&UBRRL};

/****************************************************************************
* Function    : EF_void_UART_Init
*
* DESCRIPTION : init the UART
*
* PARAMETERS  : uart_cfg : pointer to UART_cfg_str which has parameters to init the UART
*
* Return Value: None.
******************************************************************************/
void EF_void_UART_Init(UART_cfg_str *uart_cfg)
{
	/* variable helps in calculating Baud rate */
	U32_t uart_UBBR;
	*uart_reg.UCSRB_Reg = 0x00;
	*uart_reg.UCSRC_Reg = 0x00;
	/* Set the UART Baud rate */
	uart_UBBR =  F_CPU/(uart_cfg->baudrate*16)-1;
	*uart_reg.UBRRH_Reg = (uint8_t)(uart_UBBR>>8);
	*uart_reg.UBRRL_Reg = (uint8_t)uart_UBBR;
	/* Enable or disable RX,TX module & Interrupt */
	if (uart_cfg->RXInterruptEnable) *uart_reg.UCSRB_Reg |= (1 << RXCIE);
	if (uart_cfg->TXInterruptEnable) *uart_reg.UCSRB_Reg |= (1 << TXCIE);
	if (uart_cfg->ReceiverEnable   ) *uart_reg.UCSRB_Reg |= (1 << RXEN);
	if (uart_cfg->TransmitterEnable) *uart_reg.UCSRB_Reg |= (1 << TXEN);
	/* Parity and Stop bit*/
	*uart_reg.UCSRC_Reg |= (uart_cfg->parity)<<UPM0;
	*uart_reg.UCSRC_Reg |= (uart_cfg->stopBits)<<USBS;

	if (uart_cfg->numberOfDataBits == 9)
	{
		*uart_reg.UCSRB_Reg |= 0x04;
		*uart_reg.UCSRC_Reg |= (1 << URSEL) | 0x06;
	}
	else if(uart_cfg->numberOfDataBits == 5)
	{
	// do nothing
	}
	else if(uart_cfg->numberOfDataBits == 6)
	{
		*uart_reg.UCSRC_Reg |= (1 << URSEL) | 0x02;
	}
	else if(uart_cfg->numberOfDataBits == 7)
	{
		*uart_reg.UCSRC_Reg |= (1 << URSEL) | 0x04;
	}
	else
	{
		*uart_reg.UCSRC_Reg |= (1 << URSEL) | 0x06;
	}
	/* using Special Timer to able some UART Function to be unstuck */
	EF_void_TimerCreate(UART_TIMER_ID, UART_WAIT_TIME);
}
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
BOOLEAN EF_BOOLEAN_UART_GetChar(U8_t * returnedValue)
{
	BOOLEAN reciveFlag   = FALSE;

	EF_void_TimerStart(UART_TIMER_ID);

	/* make UART unstuck*/
    while( (!(*uart_reg.UCSRA_Reg & (1 << RXC))) && (!EF_BOOLEAN_TimerCheck(UART_TIMER_ID)) );
    /* extracting from the previous loop because of receiving byte or Time out*/
    EF_void_TimerStop(UART_TIMER_ID);
    EF_void_TimerReset(UART_TIMER_ID);

    reciveFlag     = TRUE;

    if(!(*uart_reg.UCSRA_Reg & (1 << RXC)))
    {
    	reciveFlag = FALSE;
    }

    *returnedValue = *uart_reg.UDR_Reg;
    return reciveFlag;
}
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
BOOLEAN EF_BOOLEAN_UART_PutChar(U8_t data)
{
	BOOLEAN sendFlag = FALSE;
	EF_void_TimerStart(UART_TIMER_ID);

	/* make UART unstuck */
    while( (!(*uart_reg.UCSRA_Reg & (1 << UDRE))) && (!EF_BOOLEAN_TimerCheck(UART_TIMER_ID)) );
    /* extracting from the previous loop because of receiving byte or Time out*/
    EF_void_TimerStop(UART_TIMER_ID);
    EF_void_TimerReset(UART_TIMER_ID);

	if(*uart_reg.UCSRA_Reg & (1 << UDRE))
	{
		/* USART Transmit Complete */
		*uart_reg.UDR_Reg = data;    /* send character */
		sendFlag = TRUE;
	}
	else
	{
		sendFlag = FALSE;
	}
	return sendFlag;
}
/****************************************************************************
* Function    : EF_u8_UART_WaitChar
*
* DESCRIPTION :  Getting the RX byte
*
* PARAMETERS  : TimeOut
*
* Return Value: U8_t : Rx byte
******************************************************************************/
U8_t EF_u8_UART_WaitChar()
{
	/* wait until receive byte */
    while((!(*uart_reg.UCSRA_Reg & (1 << RXC))) );
    return *uart_reg.UDR_Reg;
}

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
void EF_void_UART_PutChar(U8_t data)
{
	while(!(*uart_reg.UCSRA_Reg & (1 << UDRE)));  /* wait until UDR ready */
	*uart_reg.UDR_Reg = data;    				  /* send character 	  */
}

/****************************************************************************
* Function    : EF_void_UART_SendArray
*
* DESCRIPTION : sending arrays of bytes
*
*
* PARAMETERS  : array 		: pointer to TX data
*				Length		: lenght of array
*
* Return Value: None
*
******************************************************************************/
void EF_void_UART_SendArray(U8_t *array, U8_t Length)
{
    U8_t ArrayIndex;
    for(ArrayIndex = 0; ArrayIndex< Length; ArrayIndex++)
    {
    	EF_void_UART_PutChar(array[ArrayIndex]);
	}
}




void EF_void_UART_HextoASCII(U8_t *hex)
{

	U8_t temp_L = 0;                    // dummy variable that will hold LSBs of Hex number
	U8_t temp_H = 0;                    // dummy variable that will hold MSBs of Hex number

	temp_H = *hex & 0xF0;           // Obtain the upper 4 bits (MSBs) of hex number
	temp_H = temp_H >> 4;           //

	if(temp_H >9)                   // Check if the number is a letter
	{
		temp_H += 0x37;             // Convert the number to a letter in ASCII
	}
	else
	{
		temp_H += 0x30;             // Convert the number to ASCII number
	}

	EF_void_UART_PutChar(temp_H);           // Display the number

	temp_L = *hex & 0x0F;           // Obtain the lower 4 bits (LSBs) of hex number

	if(temp_L >9)                   // Check if the the number is a letter
	{
		temp_L += 0x37;             // Convert the number to a letter in ASCII
	}
	else
	{
		temp_L += 0x30;             // Convert the number to ASCII number
	}

	EF_void_UART_PutChar(temp_L);           // Display the number
}


