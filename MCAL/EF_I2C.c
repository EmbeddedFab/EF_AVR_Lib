 /***************************************************************
 *  Source File: EF_I2C.c
 *
 *  Description: I2C driver for ATMEGA32
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


#include "EF_I2C.h"


/*********************************************************************
 * Function    : void EF_void_I2C_Write(U8_t data);
 *
 * DESCRIPTION : This function used to Write One byte one I2C data bus
 *
 * PARAMETERS  : U8_t data : Data user want to send
 *
 * Return Value: Void
 *
 * Notes:
 ***********************************************************************/
void EF_void_I2C_Write(U8_t data)
{
	/* put the data in TW data register */
	TWDR = data;
	/* to start sending Byte */
	TWCR = (1<<TWINT) | (1<<TWEN);
	/* Polling until the byte is transmitted completely */
	while((TWCR & (1 << TWINT)) == 0 );

}

/*********************************************************************
 * Function    : void EF_void_I2C_Start(void);
 *
 * DESCRIPTION : This function used to Send Start Bit data bus
 *
 * PARAMETERS  : Void
 *
 * Return Value: Void
 *
 * Notes:
 ***********************************************************************/
void EF_void_I2C_Start(void)
{
	/* clear Interrupt Flag, start condition bit and Enable Module */
	TWCR = (1<< TWINT) | (1<<TWSTA) | (1<<TWEN);
	/* Polling until the start condition is transmitted completely */
	while((TWCR & (1 << TWINT)) == 0);
}

/*********************************************************************
 * Function    : void EF_void_I2C_Stop(void);
 *
 * DESCRIPTION : This function used to Send Stop Bit data bus
 *
 * PARAMETERS  : Void
 *
 * Return Value: Void
 *
 * Notes:
 ***********************************************************************/
void EF_void_I2C_Stop(void)
{
	TWCR = (1<< TWINT) | (1<< TWEN) | (1<< TWSTO) ;
}

/*********************************************************************
 * Function    : void EF_void_I2C_Init(void);
 *
 * DESCRIPTION : This function used to Send Init I2C Module
 *
 * PARAMETERS  : Void
 *
 * Return Value: Void
 *
 * Notes:
 ***********************************************************************/
void EF_void_I2C_Init(void)
{
	TWSR = 0x00;				/* make prescaler =1	   */
	TWBR = 0x47;                /* Baud rate configuration */
	TWCR = (1<< TWEN);          /* Enable I2C Module       */
}

/*********************************************************************
 * Function    : U8_t EF_U8_I2C_Read_Byte (BOOLEAN ACK);
 *
 * DESCRIPTION : This function used to Read One byte one I2C data bus
 *
 * PARAMETERS  : BOOLEAN ACK : User want to use ACK or Not
 *
 * Return Value: Void
 *
 * Notes: ACK Parameter should be 1 0r 0
 ***********************************************************************/
U8_t EF_U8_I2C_Read_Byte (BOOLEAN ACK)
{
	/* to start receiving Byte */
	TWCR = (1<< TWINT) | (1<< TWEN) | (ACK<< TWEA) ;
	/* Polling until the byte is received completely */
	while((TWCR & (1 << TWINT)) == 0);
	return TWDR;

}



