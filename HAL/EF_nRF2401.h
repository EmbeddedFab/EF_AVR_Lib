 /***************************************************************
 *  Source File: EF_nRF2401.h
 *
 *  Description: Simple driver for nRF2401+ module
 *
 *  History:     Version 1.0  - INIT Version
 *  Date   :     28/7/2015
 *  -------------------------------------------------------
 *  Author :     EmbeddedFab.
 *
 *  Copyright (C) <2015>  <EmbeddedFab>

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
#ifndef _EF_NRF2401_
#define _EF_NRF2401_
#include "../ServiceLayer/std_types.h"

/*********************************************************************
* Function    : EF_void_nRF_init()
*
* DESCRIPTION : initialise the SPI ,External Interrupt and nRF module
*
* PARAMETERS  : None
*
* Return Value: None
**********************************************************************/
extern void EF_void_nRF_init();

/*********************************************************************
* Function    : EF_void_nRF_TXSetup()
*
* DESCRIPTION : establish the module to transmit
*
* PARAMETERS  : None
*
* Return Value: None
**********************************************************************/
extern void EF_void_nRF_TXSetup(void);


/*********************************************************************
* Function    : EF_BOOLEAN_nRF_SendData
*
* DESCRIPTION : transmit data by the nRF module
*
* PARAMETERS  : U8_t* Data: pointer to transmitted data
* 			    U8_t DataLength: no of bytes of transmitted data
*
* Return Value:  BOOLEAN to check for Errors
**********************************************************************/
extern BOOLEAN EF_BOOLEAN_nRF_SendData(U8_t* Data, U8_t DataLength);

/*********************************************************************
* Function    : EF_BOOLEAN_nRF_RXSetup()
*
* DESCRIPTION : establish the module to receive
*
* PARAMETERS  : None
*
* Return Value: None
**********************************************************************/
extern void EF_BOOLEAN_nRF_RXSetup(void);

/*********************************************************************
* Function    : EF_BOOLEAN_nRF_GetData
*
* DESCRIPTION : receive data by the nRF module
*
* PARAMETERS  : U8_t* Data: pointer to received data
* 			    U8_t DataLength: no of bytes of received data
*
* Return Value:  BOOLEAN to check for Errors
**********************************************************************/
extern BOOLEAN EF_BOOLEAN_nRF_GetData(U8_t* Data , U8_t DataLength);

/*********************************************************************
* Function    : EF_u8_nRF_ReadRegister
*
* DESCRIPTION : read specific register located the nRF module
*
* PARAMETERS  : U8_t Reg_Add: address of wanted register
*
* Return Value: the data located in this register
**********************************************************************/
U8_t EF_u8_nRF_ReadRegister(U8_t Reg_Add);

#endif
