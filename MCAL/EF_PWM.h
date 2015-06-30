/***************************************************************
 *  Source File: EF_PWM.h
 *
 *  Description: PWM driver for ATMEGA32
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

#ifndef EF_PWM_H_
#define EF_PWM_H_

#include "EF_pwm_cfg.h"

/*************************************************
 * 	extern API functions
 *************************************************/

/*********************************************************************
* Function    : EF_void_PWM_init(U8_t u8TimerNumber);
*
* DESCRIPTION : Initializes the PWM in fast mode.
*
* PARAMETERS  : The number of 8 bit timer to set for fast mode pwm.
*               Either Timer 0 or Timer 2.
*
* Return Value: Nothing
***********************************************************************/
extern void EF_void_PWM_init(U8_t u8TimerNumber);

/****************************************************************************
* Function    : EF_void_PWM_SetDutyValue(U8_t DutyValue, U8_t u8TimerNumber)
*
* DESCRIPTION : Sets duty cycle for the  PWM
*
* PARAMETER 1 : The duty cycle value. A value between 0 and 255.
*
* PARAMETER 2 : The number of 8 bit timer. Either Timer 0 or Timer 2.
*
* Return Value: Nothing
******************************************************************************/
extern void EF_void_PWM_SetDutyValue(U8_t DutyValue, U8_t u8TimerNumber);

/****************************************************************************
* Function    : EF_void_PWM_SetDutyCycle(U8_t DutyCycle, U8_t u8TimerNumber)
*
* DESCRIPTION : Sets duty cycle for the  PWM
*
* PARAMETER 1 : The duty cycle value. A value between (0 %) and (100 %).
*
* PARAMETER 2 : The number of 8 bit timer. Either Timer 0 or Timer 2.
*
* Return Value: Nothing
******************************************************************************/
extern void EF_void_PWM_SetDutyCycle(U8_t DutyCycle, U8_t u8TimerNumber);

#endif /* PWM_EF_H_ */
