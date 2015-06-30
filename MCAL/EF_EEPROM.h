/***************************************************************
 *  Source File: EF_EEPROM.h
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

#ifndef EF_EEPROM_H_
#define EF_EEPROM_H_

#include "../ServiceLayer/std_types.h"
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
extern void EF_void_EEPROM_WriteByte(U16_t eeprom_Address, U8_t eeprom_Data);

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
extern void EF_void_EEPROM_WriteNBytes(U16_t eeprom_Address, U8_t *RamAddr, S8_t NoOfBytes);

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
extern U8_t EF_u8_EPROM_ReadByte(U16_t eeprom_Address);

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
extern void EF_void_EEPROM_ReadNBytes(U16_t EepromAddr, U8_t *RamAddr, S8_t NoOfBytes);

/*********************************************************************
 * Function    : void EEPROM_Erase(void);
 * DESCRIPTION : This function is used to erase the entire EEprom memory.
 * PARAMETERS  : void.
 *
 * Return Value: void.
 * Note        : void.
 ***********************************************************************/
extern void EF_void_EEPROM_Erase(void);

#endif /* EF_EEPROM_H_ */
