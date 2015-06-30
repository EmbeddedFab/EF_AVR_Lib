/***************************************************************
 *  Source File: EF_E2PROM_24C16.h
 *
 *  Description: This simple driver for E2PROM_24C16
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
#ifndef E2PROM_24C16_H_
#define E2PROM_24C16_H_

#include <avr/io.h>
#include <util/delay.h>
#include "../MCAL/EF_DIO.h"
#include "../MCAL/EF_I2C.h"
#include "../ServiceLayer/std_types.h"

/*********************************************************************
 * Function    : EF_void_EEPROM24C16_Init
 *
 * DESCRIPTION : This function used to initalize the I2C Module
 *
 * PARAMETERS  : None
 *
 * Return Value: Void
 ***********************************************************************/
extern void EF_void_EEPROM24C16_Init ();


/*********************************************************************
 * Function    : void EF_void_EEPROM24C16_Write_Byte (U8_t memory_location,
 *                                          U8_t slave_address,
 *                                          U8_t data);
 *
 * DESCRIPTION : This function used to Write One byte
 *
 * PARAMETERS  : U8_t memory_location,
 *               U8_t slave_address,
 *               U8_t data
 *
 * Return Value: Void
 *
 * Notes:
 * 	slave address: [1|A2|A1|A0|B2|B1|B0|R/W']
 * 	A   : represent the Device Add., set them as the pin connection except A1
 * 	     (the A1 bit must be the compliment of the A1 input pin signal)
 * 	B   : represent the block address, select one of the eight 256 x 8 memory block
 * 	R/W': put 0 when writing and put 1 when reading
 ***********************************************************************/
void EF_void_EEPROM24C16_Write_Byte (U8_t memory_location, U8_t slave_address, U8_t data);

/*********************************************************************
 * Function    : void EF_void_EEPROM_Read_Byte (U8_t memory_location,
 *                                          U8_t slave_address);
 *
 * DESCRIPTION : This function used to Write One byte
 *
 * PARAMETERS  : U8_t memory_location:
 *               U8_t slave_address:
 *
 * Return Value: Void
 *
 * Notes:
 * slave address: [1|A2|A1|A0|B2|B1|B0|R/W']
 * A   : represent the Device Add., set them as the pin connection except A1
 * 	     (the A1 bit must be the compliment of the A1 input pin signal)
 * B   : represent the block address, select one of the eight 256 x 8 memory block
 * R/W': put 0 when writing and put 1 when reading
 ***********************************************************************/
U8_t EF_void_EEPROM24C16_Read_Byte (U8_t memory_location, U8_t slave_address);

#endif /* E2PROM_24C16_H_ */
