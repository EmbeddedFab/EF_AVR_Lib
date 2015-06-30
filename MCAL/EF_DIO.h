/***************************************************************
 *  Source File: EF_DIO.h
 *
 *  Description: DIO driver For ATMEGA32
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

#include "../ServiceLayer/std_types.h"

/**************************************************
 * Define
 *************************************************/
#define INPUT   0
#define OUTPUT  1

/*********************************************************************
* Function    : DIO_InitPort(U8_t PortName, U8_t Value);
*
* DESCRIPTION : This function used to Initialize port.
*
* PARAMETERS  :  
*            
*             U8_t PortName: User write which port need to access.
*                            like 'A' or 'a'.
*             U8_t Value   : User write the value in range (0 -> 255).
*
* Return Value: The function will return -1 if user enter wrong inputs.
*               or return 1 if OK.  
* Note        : To configure PIN as output you should set the bit mask "1" 
*               and to set PIN input you should set bit mask "0"
***********************************************************************/
S8_t  EF_S8_DIO_InitPort (U8_t PortName, U8_t Value);

/*********************************************************************
* Function    : DIO_WritePort(U8_t PortName, U8_t Value);
*
* DESCRIPTION : This function used to write value to any port.
*
* PARAMETERS  :  
*            
*             U8_t PortName: User write which port need to access.
*                            like 'A' or 'a'.
*             U8_t Value   : User write the value in range (0 -> 255).
*
* Return Value: The function will return -1 if user enter wrong inputs.
*               or return 1 if OK.  
***********************************************************************/
S8_t  EF_S8_DIO_WritePort(U8_t PortName, U8_t Value);

/*********************************************************************
* Function    : S16_t DIO_ReadPort (U8_t PortName);
*
* DESCRIPTION : This function used to read value from any port.
*
* PARAMETERS  :  
*            
*             U8_t PortName: User write which port need to access.
*                            like 'A' or 'a'.
*
* Return Value: The function will return -1 if user enter wrong inputs.
*               or the value.
***********************************************************************/
S16_t EF_S16_DIO_ReadPort (U8_t PortName);

/*********************************************************************
* Function    : EF_B_DIO_InitPin (U8_t PortName, U8_t PinNumber,U8_t Direction)
*
* DESCRIPTION : This function used to Initialize port.
*
* PARAMETERS  :
*
*             U8_t PortName: User write which port need to access.
*                            like 'A' or 'a'.
*             U8_t PinNumber: Pin Number between 0  and 7
*
*             U8_t Direction: Either INPUT or OUTPUT
*
* Return Value: The function will return -1 if user enter wrong inputs.
*               or return 1 if OK.
* Note        : Nothing
***********************************************************************/
BOOLEAN  EF_B_DIO_InitPin (U8_t PortName, U8_t PinNumber,U8_t Direction);

/*********************************************************************
* Function    : S8_t DIO_SetPin (U8_t PortName, U8_t PinNumber);
*
* DESCRIPTION : This function used to set pin any port.
*
* PARAMETERS  :  
*            
*             U8_t PortName: User write which port need to access.
*                            like 'A' or 'a'.
*             U8_t PinNumber: User write the value in range (0 -> 7).
*
* Return Value: The function will return -1 if user enter wrong inputs.
*               or return 1 if OK.  
* Note        : This function set output PIN.
***********************************************************************/
S8_t  EF_S8_DIO_SetPin   (U8_t PortName, U8_t PinNumber);

/*********************************************************************
* Function    : S8_t DIO_ClearPin (U8_t PortName, U8_t PinNumber);
*
* DESCRIPTION : This function used to clear Pin in port.
*
* PARAMETERS  :  
*            
*             U8_t PortName: User write which port need to access.
*                            like 'A' or 'a'.
*             U8_t PinNumber: User write the value in range (0 -> 7).
*
* Return Value: The function will return -1 if user enter wrong inputs.
*               or return 1 if OK.  
***********************************************************************/
S8_t  EF_S8_DIO_ClearPin (U8_t PortName, U8_t PinNumber);

/*********************************************************************
* Function    : S8_t  DIO_SetPort  (U8_t PortName);
*
* DESCRIPTION : This function used to set port.
*
* PARAMETERS  :  
*            
*             U8_t PortName: User write which port need to access.
*                            like 'A' or 'a'.
*             U8_t Value   : User write the value in range (0 -> 255).
*
* Return Value: The function will return -1 if user enter wrong inputs.
*               or return 1 if OK.  
***********************************************************************/
S8_t  EF_S8_DIO_SetPort  (U8_t PortName);

/*********************************************************************
* Function    : S8_t  DIO_ClearPort(U8_t PortName);
*
* DESCRIPTION : This function used to clear port.
*
* PARAMETERS  :  
*            
*             U8_t PortName: User write which port need to access.
*                            like 'A' or 'a'.
*
* Return Value: The function will return -1 if user enter wrong inputs.
*               or return 1 if OK.  
***********************************************************************/
S8_t  EF_S8_DIO_ClearPort(U8_t PortName);

/*********************************************************************
* Function    : S8_t  DIO_TogglePin(U8_t PortName, U8_t PinNumber);
*
* DESCRIPTION : This function used to toggle any PIN in any PORT.
*
* PARAMETERS  :  
*            
*             U8_t PortName: User write which port need to access.
*                            like 'A' or 'a'.
*             U8_t PinNumber: User write the value in range (0 -> 7).
*
* Return Value: The function will return -1 if user enter wrong inputs.
*               or return 1 if OK.  
***********************************************************************/
S8_t  EF_S8_DIO_TogglePin(U8_t PortName, U8_t PinNumber);

/*********************************************************************
* Function    : S8_t  DIO_CheckPin (U8_t PortName, U8_t PinNumber);
*
* DESCRIPTION : This function used to Check pin status .
*
* PARAMETERS  :  
*            
*             U8_t PortName: User write which port need to access.
*                            like 'A' or 'a'.
*             U8_t PinNumber: User write the value in range (0 -> 7).
*
* Return Value: The function will return -1 if user enter wrong inputs.
*               or return 1 if Pin ON and 0 If pin OFF.  
***********************************************************************/
S8_t  EF_S8_DIO_CheckPin (U8_t PortName, U8_t PinNumber);

