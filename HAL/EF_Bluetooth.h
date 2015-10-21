/***************************************************************
 *  Source File: EF_Bluetooth.h
 *
 *  Description: Bluetooth wifi driver
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
#ifndef _EF_Bluetooth_H_
#define _EF_Bluetooth_H_

#include "../ServiceLayer/std_types.h"
#include "EF_Bluetooth_cfg.h"
#include "EF_LCD.h"

/* ------------------------- API Functions ----------------------*/

/*********************************************************************
* Function    : EF_BOOLEAN_Bluetooth_ModuleInit ( BOOLEAN bMaster, U8_t* Password ,
*                                               U8_t* ModuleName , U8_t ModuleName_Length);
*
* DESCRIPTION : This function used to initialise UART, Enable AT Command Pin (make it 3.3v to at command
*               and 0 for Communication Mode) , Timer and Send some Init Commands
*
* PARAMETERS  : bMaster   : 1 to make Bluetooth Module to master and 0 to be slave
*               Password  : Set Bluetooth Module Password (4 Number characters (ASCII) like: "1234" )
*               ModuleName: Set Bluetooth Module Name
*
* Return Value: BOOLEAN to check for Errors
**********************************************************************/
BOOLEAN EF_BOOLEAN_Bluetooth_ModuleInit ( BOOLEAN bMaster, U8_t* Password , U8_t* ModuleName , U8_t ModuleName_Length);

/*********************************************************************
* Function    : EF_BOOLEAN_Bluetooth_MasterScan (U8_t ScanDevicesNumber ,
*                                               U8_t TimeOut, U8_t* ReturnedAddresses );
*
* DESCRIPTION : This function used to query/search for the nearby discoverable devices, print and
*               return the MAC Add. which is found.
*
* PARAMETERS  : ScanDevicesNumber: max number of devices to stop searching when reaching it.
*               TimeOut          : max time to stop searching when reaching it.
*               ReturnedAddresses: MAC Add. of existing devices, (you should give it pointer to
*                                  Large array Like 50 character)
*
* Return Value: BOOLEAN to check for Errors
**********************************************************************/
BOOLEAN EF_BOOLEAN_Bluetooth_MasterScan (U8_t ScanDevicesNumber , U8_t TimeOut, U8_t* ReturnedAddresses );

/*********************************************************************
* Function    : EF_BOOLEAN_Bluetooth_MasterConnect (U8_t SlaveMAC_Add ,
*                                                  U8_t TimeOut,
*                                                  U8_t* ReturnSlaveName );
*
* DESCRIPTION : This function used to pair , connect to definied slave device with
*               it's MAC ADD , print and return Slave Device name.
*
* PARAMETERS  : SlaveMAC_Add   : 14 characters (12 of them is the MAC ADD of the wanted Slave
*                                device. give it as : 4char,2char,6char == "1CAF,05,D69CE9" )
*               TimeOut        : max time to enable Slave to enter Password and pairing
*               ReturnSlaveName:  return Slave Device name.
*
* Return Value: BOOLEAN to check for Errors
**********************************************************************/
BOOLEAN EF_BOOLEAN_Bluetooth_MasterConnect (U8_t * SlaveMAC_Add, U8_t TimeOut, U8_t* ReturnSlaveName );

/*********************************************************************
* Function    : EF_BOOLEAN_Bluetooth_DisConnect ();
*
* DESCRIPTION : This function used to disconnect Connection whether Module was Master or Slave
*
* PARAMETERS  : None.
*
* Return Value: BOOLEAN to check for Errors
**********************************************************************/
BOOLEAN EF_BOOLEAN_Bluetooth_DisConnect ();

/*********************************************************************
* Function    : EF_BOOLEAN_Bluetooth_GetModuleStatus ( U8_t* Status );
*
* DESCRIPTION : This function used to print and return the Module Status
*               returned +STATE:<stat>\r\nOK\r\n  , <stat> are  9 status
*
* PARAMETERS  : Status: pointer to the returned status
*
* Return Value: return 1 if INITIALIZED : Slave or Master after power up
*               return 2 if READY
*               return 3 if PAIRABLE    : Slave  after AT+INIT
*               return 4 if PAIRED      : Master after pairing or binding
*               return 5 if INQUIRING   : Master after quering
*               return 6 if CONNECTING  : Master after pairing when Communication Enable (0 v to pin)
*               return 7 if CONNECTED   : Slave or Master after linking
*               return 8 if DISCONNECTED: Master or Slave
*               return 9 if UNKNOWN
*               return 0 if error
**********************************************************************/
U8_t EF_u8_Bluetooth_GetModuleStatus ( U8_t* Status );


/*********************************************************************
* Function    : EF_BOOLEAN_Bluetooth_SendData (U8_t* Data, U8_t DataLength );
*
* DESCRIPTION : This function used to
*
* PARAMETERS  : Data : pointer to send data
*               DataLength : number of characters
*
* Return Value: return FALSE if the status is not connected
*               return TRUE if ok
**********************************************************************/
BOOLEAN EF_BOOLEAN_Bluetooth_SendData (U8_t* Data, U8_t DataLength );

/*********************************************************************
* Function    : EF_BOOLEAN_Bluetooth_GetDataIfExist ( U8_t* RxData_ptr );
*
* DESCRIPTION : This function used to
*
* PARAMETERS  : RxData_ptr: pointer to returned data.
*
* Return Value: return FALSE if the status is not connected
*               return TRUE if ok
**********************************************************************/
BOOLEAN EF_BOOLEAN_Bluetooth_GetDataIfExist ( U8_t* RxData_ptr );



#endif
