/***************************************************************
 *  Source File: EF_ESP8266.h
 *
 *  Description: ESP8266 wifi driver
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
#ifndef _EF_ESP8266_H_
#define _EF_ESP8266_H_

#include "../ServiceLayer/std_types.h"
#include "EF_LCD.h"
#include "EF_ESP8266_cfg.h"


/* ------------------------- API Functions ----------------------*/

/*********************************************************************
* Function    : EF_BOOLEAN_ESP8266_InitModule (void);
*
* DESCRIPTION : This function used to initialise ESP8266 Wifi, init UART
*               and Timer , Send some Init Commands
*
* PARAMETERS  : none.
*
* Return Value: BOOLEAN to check for Errors
**********************************************************************/
BOOLEAN EF_BOOLEAN_ESP8266_InitModule (void);


/*********************************************************************
* Function    : EF_BOOLEAN_ESP8266_JoinAP
*
* DESCRIPTION : This function used to connect to Given Access Point
*
* PARAMETERS  : AcessPoint: pointer to Acess Point name
*               AcessPoint_Length: length of AcessPoint array
*               Password: pointer to Password array
*               Password_Length: password array length
*
* Return Value: return TRUE if ok or FALSE if not Expected Answer
**********************************************************************/
BOOLEAN EF_BOOLEAN_ESP8266_JoinAP (U8_t* AcessPoint, U8_t AcessPoint_Length, U8_t* Password, U8_t Password_Length);


/*********************************************************************
* Function    : EF_void_ESP8266_SetIp
*
* DESCRIPTION : This function used to Set Ip for Wifi Module
*
* PARAMETERS  : IP_ptr: pointer to IP
*               IP_Length: IP Length
*
* Return Value: BOOLEAN to check for Errors
**********************************************************************/
BOOLEAN EF_BOOLEAN_ESP8266_SetIp (U8_t* IP_ptr, U8_t IP_Length);


/*********************************************************************
* Function    : EF_BOOLEAN_ESP8266_OpenServer
*
* DESCRIPTION : This function used to make Wifi Module Opening Server
*
* PARAMETERS  : none.
*
* Return Value: BOOLEAN to check for Errors
**********************************************************************/
BOOLEAN EF_BOOLEAN_ESP8266_OpenServer (void );

/*********************************************************************
* Function    : EF_BOOLEAN_ESP8266_CreateAcessPoint
*
* DESCRIPTION : This function used to Set Ip for Wifi Module
*
* PARAMETERS  : AP_ptr    : pointer to AP name which is wanted to create
*               AP_Length : AP Length
*
* Return Value: BOOLEAN to check for Errors
**********************************************************************/
BOOLEAN EF_BOOLEAN_ESP8266_CreateAcessPoint (U8_t* AP_ptr, U8_t AP_Length);

/*********************************************************************
* Function    : EF_BOOLEAN_ESP8266_SendToServer
*
* DESCRIPTION : Send data to Server , (to Server Ip), and if errors repeat all
*               steps for number of attemps = MAX_ATTEMPS
*
* PARAMETERS  : IP_ptr : Tx IP
*               IP_Length : length of Ip array
*               Data_ptr : pointer to Data wanted to send
*               DataLength : Data Length
*
* Return Value: BOOLEAN to check for Errors
**********************************************************************/
BOOLEAN EF_BOOLEAN_ESP8266_SendToServer (U8_t* IP_ptr, U8_t IP_Length, U8_t* Data_ptr, U8_t DataLength );


/*********************************************************************
* Function    : EF_BOOLEAN_ESP8266_GetRxData
*
* DESCRIPTION : This function used to parcing to get the rx Data
*               and to print any data and to reinit if reset
*
* PARAMETERS  : UartBase
*               RxData_ptr: pointer to received data
*
* Return Value: 0 if byte is not received
*               1 if normal byte received
*               2 if data completed
*               3 if hardware reset  occured
**********************************************************************/
BOOLEAN EF_BOOLEAN_ESP8266_GetRxData (U32_t UartBase , U8_t* RxData_ptr);

#endif
