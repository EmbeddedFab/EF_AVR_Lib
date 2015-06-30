/***************************************************************
 *  Source File: EF_EEPROM.c
 *
 *  Description: Internal EEPROM (1Kbytes) driver For ATMEGA32
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
/**************************************************************
 * Notes: This driver can use with EEPROM module only.
 *
 **************************************************************/

#include<avr\io.h>
#include<util\delay.h>
#include "EF_EEPROM.h"

/*The below value should be set depending on the controller
by referring the respective data sheet*/
#define MAX_EEPROM_SIZE           1024


/*********************************************************************
 * Function    : void EF_void_EEPROM_WriteByte(U16_t eeprom_Address,
 *                                        U8_t eeprom_Data);
 * DESCRIPTION : This function is used to write the data at specified
 *               EEPROM_address...
 * PARAMETERS  :
 *               U16_t eeprom_Address: Location address for data.
 *               U8_t eeprom_Data    : Data which user need to save.
 *
 * Return Value: void.
 * Note        : void.
 ***********************************************************************/
void EF_void_EEPROM_WriteByte(U16_t eeprom_Address, U8_t eeprom_Data)
{
	/* Wait for completion of previous write, EEWE will be
	   cleared by hardware once EEprom write is completed */
	while(EECR & (1<<EEWE));

	EEAR = eeprom_Address;  /*Load the eeprom adddress and data   */
	EEDR = eeprom_Data;
	EECR |= (1<<EEMWE);     /* EEMWE: master write Enable 		  */
	EECR |= (1<<EEWE);      /* Start EEprom write by setting EEWE */
}

/*********************************************************************
 * Function    : U8_t EF_EPROM_ReadByte(U16_t eeprom_Address);
 * DESCRIPTION : This function is used to read the data from
 *               specified EEPROM_address.
 * PARAMETERS  :
 *               U16_t eeprom_Address: Location address for data.
 *
 * Return Value: U8_t                : Data read from Address.
 * Note        : void.
 ***********************************************************************/
U8_t EF_u8_EPROM_ReadByte(U16_t eeprom_Address)
{
	while(EECR & (1<<EEWE));  /* Wait for completion of previous write if any. 		   */

	EEAR = eeprom_Address;    /*Load the address from where the data needs to be read. */
	EECR |=(1<<EERE);         /* start eeprom read by setting EERE :read enable        */
	/* TODO: remove delay */
	_delay_ms(1);
	return EEDR;              /* Return data from data register						   */
}

/*********************************************************************
 * Function    : void EF_void_EEPROM_WriteNBytes(U16_t EepromAddr,
 *                                          U8_t *RamAddr,
 *                                          S8_t NoOfBytes);
 * DESCRIPTION : This function is used to write N-bytes of data
 *               at specified EEPROM_address.
 * PARAMETERS  :
 *               U16_t eeprom_Address: Location address for data.
 *               U8_t *RamAddr      :
 *               S8_t NoOfBytes     :
 *
 * Return Value: void.
 * Note        : void.
 ***********************************************************************/
void EF_void_EEPROM_WriteNBytes(U16_t eeprom_Address, U8_t *RamAddr, S8_t NoOfBytes)
{
	if(NULL != *RamAddr)
	{
		while(NoOfBytes !=  0)
		{
			EF_void_EEPROM_WriteByte(eeprom_Address,
					            *RamAddr);  /* Write a byte from RAM to EEPROM       	   */
			eeprom_Address++;               /* Increment the EEprom Address 			   */
			RamAddr++;                      /* Increment the RAM Address 				   */
			NoOfBytes--;                    /* Decrement NoOfBytes after writing each Byte */
		}
	}
}

/*********************************************************************
 * Function    : void EEPROM_ReadNBytes(U16_t EepromAddr,
 *                                      U8_t *RamAddr,
 *                                      S8_t NoOfBytes);
 * DESCRIPTION : This function is used to Read N-bytes of data
 *               from specified EEPROM_address.
 * PARAMETERS  :
 *               U16_t eeprom_Address: Location address for data.
 *               U8_t *RamAddr      :
 *               S8_t NoOfBytes     :
 *
 * Return Value: void.
 * Note        : void.
 ***********************************************************************/
void EF_void_EEPROM_ReadNBytes(U16_t EepromAddr, U8_t *RamAddr, S8_t NoOfBytes)
{
	if(NULL != *RamAddr)
	{
		while(NoOfBytes !=  0)
		{
			*RamAddr = EF_u8_EPROM_ReadByte(EepromAddr); /* Read a byte from EEPROM to RAM 			 */
			EepromAddr++;                             /* Increment the EEprom Address				 */
			RamAddr++;                                /* Increment the RAM Address 				   	 */
			NoOfBytes--;                              /* Decrement NoOfBytes after Reading each Byte */
		}
	}
}
/*********************************************************************
 * Function    : void EEPROM_Erase(void);
 * DESCRIPTION : This function is used to erase the entire EEprom memory.
 * PARAMETERS  : void.
 *
 * Return Value: void.
 * Note        : void.
 ***********************************************************************/
void EF_void_EEPROM_Erase(void)
{
	U16_t eeprom_address;
	for(eeprom_address=0; eeprom_address < MAX_EEPROM_SIZE; eeprom_address++)
	{
		EF_void_EEPROM_WriteByte(eeprom_address,0xff);     /* Write Each memory location with OxFF */
	}
}
