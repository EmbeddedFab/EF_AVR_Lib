 /***************************************************************
 *  Source File: EF_SPI.c
 *
 *  Description: SPI driver for ATMEGA32
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
#include "EF_SPI.h"
#include <util/delay.h>
#include <avr/io.h>
#include "EF_SPI_cfg.h"
#include "EF_SpechialTIMER.h"


/*********************************************************************
* Function    : void EF_SpiInit(U8_t DeviceType);
*
* DESCRIPTION : This function used to Initialize SPI Module.
*
* PARAMETERS  : U8_t DeviceType : 1 ----> Master
*                                 0 ----> Slave
* Return Value: void
**********************************************************************/
void EF_SpiInit(U8_t DeviceType)
{	
    SPCR = (1 << SPE)|(1 << DORD)|(1 << SPR1)|(1 << SPR0);
	if(MASTER_TYPE == DeviceType)
	{
        SPCR |= (1 << MSTR);   /* Set the device as Master */
        /* make (MOSI) PB5 & (SCK)PB7 & (SS)PB4 : output */
        SPI_DDR |= (1<<MOSI_BIT) | (1<<SS_BIT) | (1<<SCK_BIT);
	}
	else
	{
		SPI_DDR |= (1<<MISO_BIT);		   /* Slave , make (MISO) PB6 : output */
	}

	/* using Special Timer to able some UART Function to be unstuck */
	EF_void_TimerCreate(SPI_TIMER_ID, SPI_TIMEOUT);

}


/*********************************************************************
* Function    : U8_t EF_SpiTransfer(U8_t data);
*
* DESCRIPTION : This function used to Transfer Data through SPI bus
*
* PARAMETERS  : U8_t data  Data user need to transmit.
*
*
* Return Value: Data User received
**********************************************************************/
U8_t EF_SpiTransfer(U8_t data)
{
	SPDR = data;
	while (!( SPSR & (1<<SPIF)))   /* Wait for empty transmit buffer */
	{};
	return SPDR;                     /* Put data into buffer, sends the data */
}

/*********************************************************************
* Function    : U8_t EF_BOOLEAN_SpiTransfer(U8_t * returnedValue);
*
* DESCRIPTION : This function used to Transfer Data through SPI bus without Stuck .
*
* PARAMETERS  : U8_t data  transfered data .
*
*
* Return Value: return True if Byte is received or false
**********************************************************************/
BOOLEAN EF_BOOLEAN_SpiTransfer(U8_t * returnedValue)
{
	/* TODO : test this function */
	BOOLEAN TransferFlag   = FALSE;
	/* using Special Timer */
	EF_void_TimerStart(SPI_TIMER_ID);

	SPDR = *returnedValue;
	/* make SPI unstuck  + Wait for empty transmit buffer */
    while( (!( SPSR & (1<<SPIF))) && (!EF_BOOLEAN_TimerCheck(SPI_TIMER_ID)) );
    /* extracting from the previous loop because of receiving byte or Time out*/
    EF_void_TimerStop(SPI_TIMER_ID);
    EF_void_TimerReset(SPI_TIMER_ID);


    if  (!( SPSR & (1<<SPIF)))
    {
    	/* TODO : make 0,1 #defined */
    	TransferFlag = 0;
    }
    else
    {
    	TransferFlag = 1;
    }
	*returnedValue = SPDR;

	return TransferFlag;

}
