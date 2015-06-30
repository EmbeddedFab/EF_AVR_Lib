 /***************************************************************
 *  Source File: EF_SkyLabGPS.h
 *
 *  Description: Simple driver for SkyLab GPs module
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
#ifndef  _SKYLAB__
#define  _SKYLAB__

#include "../ServiceLayer/std_types.h"
/**************************************************
 * Definitions
 *************************************************/
/* return value from EF_B_SkyLabGPS_GetPosition function */
enum
{
	DATA_NOT_VAILD,
	DATA_VAILD

}ReturnFromGetPosition;

/*********************************************************************
 * API Functions
 *********************************************************************/
/*********************************************************************
* Function    : EF_B_SkyLabGPS_Init()
*
* DESCRIPTION : initialise the uart and LCD/UartUtilts
*
* PARAMETERS  : None
*
* Return Value: BOOLEAN to check for Errors
**********************************************************************/
BOOLEAN EF_B_SkyLabGPS_Init();

/*********************************************************************
* Function    : EF_B_SkyLabGPS_GetPosition( double* D_LatitudePtr, double* D_LongitudePtr)
*
* DESCRIPTION : receive the frames which is sent by GPS module every 1sec,
* 				and parse it to get the latitude and longitude after converting
* 				them to double if data is valid
*
* PARAMETERS  : D_LatitudePtr  : pointer to return the latitude  "double" in it
* 				D_LongitudePtr : pointer to return the longitude "double" in it
*
* Return Value: returns DATA_NOT_VAILD or DATA_VAILD
**********************************************************************/
BOOLEAN EF_B_SkyLabGPS_GetPosition( double* D_LatitudePtr, double* D_LongitudePtr);

/*********************************************************************
* Function    : EF_B_SkyLabGPS_PrintPosition( double D_Latitude, double D_longitude)
*
* DESCRIPTION : display the latitude and longitude in LCD/UART_Utilts
*
* PARAMETERS  : D_Latitude  : latitude  in double
* 				D_longitude : longitude in double
*
* Return Value: BOOLEAN to check for Errors
**********************************************************************/
BOOLEAN EF_B_SkyLabGPS_PrintPosition( double D_Latitude, double D_Longitude);

#endif
