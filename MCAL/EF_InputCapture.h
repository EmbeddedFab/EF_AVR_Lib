/***************************************************************
 *  Source File: EF_InputCapture.h
 *
 *  Description: Input Capture driver For ATMEGA32
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

#ifndef _EF_INPUTCAPTURE_H_
#define _EF_INPUTCAPTURE_H_
/*- INCLUDES  ---------------------------------------------------*/
#include "../ServiceLayer/std_types.h"

/*- API FUNCTIONS  ----------------------------------------------*/

/*********************************************************************
* Function    : EF_void_InputCapture_Init
*
* DESCRIPTION : this function init the Input Capture using Timer1
*
* PARAMETERS  : None
*
* Return Value: None
***********************************************************************/
 extern void EF_void_InputCapture_Init(void);

 /*********************************************************************
 * Function    : EF_BOOLEAN_InputCapture_GetPulse
 *
 * DESCRIPTION : this function is used to Get the pulse of the signal
 * 				 which connected to ICP1 pin
 *
 * PARAMETERS  : Pulse_ptr: pointer to the wanted Pulse (No. of CLockCycles)
 * 				 TimeOut  : time of waiting the pulse. for example for 100 msec
 * 				 			put 100. TimeOut is Multipliers of 20 ms
 *
 * Return Value: indicate the status: FAILURE to get pulse or SUCESS
 *
 * NOTE        : 1-returned pulse is number of clock cycles ,to convert it
 * 				   to time using Time_pulse = pulse*(1/(F_CPU/prescaler))
 * 				 2-don't forget to init. Timer
 ***********************************************************************/
 extern BOOLEAN EF_BOOLEAN_InputCapture_GetPulse(U16_t* Pulse_ptr, U16_t TimeOut);

 /*********************************************************************
 * Function    : EF_BOOLEAN_InputCapture_GetPeriod
 *
 * DESCRIPTION : this function is used to Get the Period of the signal
 * 				 which connected to ICP1 pin
 *
 * PARAMETERS  : Pulse_ptr: pointer to the wanted Period (No. of CLockCycles)
 * 				 TimeOut  : time of waiting the period. for example for 2000 msec
 * 				 			put 2000. TimeOut is Multipliers of 20 ms
 *
 * Return Value: indicate the status: FAILURE to get period or SUCESS
 *
 *  * NOTE        : 1-returned pulse is number of clock cycles ,to convert it
 * 				      to time using Time_pulse = pulse*(1/(F_CPU/prescaler))
 * 				    2-don't forget to init. Timer
 *
 ***********************************************************************/
 extern BOOLEAN EF_BOOLEAN_InputCapture_GetPeriod(U16_t* Period_ptr, U16_t TimeOut);

#endif
