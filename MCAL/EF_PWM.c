 /***************************************************************
 *  Source File: EF_PWM.c
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

#include "EF_pwm_cfg.h"


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
void EF_void_PWM_init(U8_t u8TimerNumber)
{
	switch(u8TimerNumber)
	{
        #ifdef ENABLE_TIMER_2
	    case TIMER_2:
	    	 /* OC2 PIN TO BE OUTPUT */
	    	TIMER2_DDR |= (1<<OC2_OUTPUT_SETTER);
	    	 /*Disable Timer 2*/
	    	 TCCR2 = DISABLE_MODULE_VALUE;
	    	 /* Setting pre scaler */
	    	 TCCR2 = PRESCLR1_VALUE|(FAST_PWM)|(NON_INVERT);
	    	 /* Initializing the timer with zero */
	    	 TCNT2 = ZER0_INITIALIZER;
	         break;
#else
	    case TIMER_0:
	    	/* OC0 PIN TO BE OUTPUT */
	    	TIMER0_DDR |= (1<<OC0_OUTPUT_SETTER);
	    	/* Disable Timer 0 */
	    	TCCR0 = DISABLE_MODULE_VALUE;
	    	/* Setting pre scaler */
	    	TCCR0 = (PRESCLR64_VALUE)|(FAST_PWM)|(NON_INVERT);
	    	/* Initializing the timer with zero */
	    	TCNT0 = ZER0_INITIALIZER;
	    	break;

#endif
	    default:
	    	 break;
	}
	return;
}

/****************************************************************************
* Function    : EF_void_PWM_SetDutyValue(U8_t DutyValue, U8_t u8TimerNumber)
*
* DESCRIPTION : Sets duty cycle for the  PWM
*
* PARAMETER 1 : The duty cycle value. A value between 0 (0 %) and 255 (100 %).
*
* PARAMETER 2 : The number of 8 bit timer. Either Timer 0 or Timer 2.
*
* Return Value: Nothing
******************************************************************************/
void EF_void_PWM_SetDutyValue(U8_t DutyValue, U8_t u8TimerNumber)
{

	  switch(u8TimerNumber)
	  {

#ifdef ENABLE_TIMER_2
	  case TIMER_2:
		  /*Output Compare Register � OCR2
		   * contains an 8-bit value that is continuously compared
		   * with the counter value (TCNT2). A match can be used to
		   * generate an output compare interrupt
		   * */
		  OCR2 = DutyValue;
		  break;
#else
	  case TIMER_0:
		  /*Output Compare Register � OCR0
		   * contains an 8-bit value that is continuously compared
		   * with the counter value (TCNT0). A match can be used to
		   * generate an output compare interrupt
		   * */
		  OCR0 = DutyValue;
		  break;
      #endif
	  default:
		   break;
	  }
	  return;
}

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
void EF_void_PWM_SetDutyCycle(U8_t DutyCycle, U8_t u8TimerNumber)
{
	  U8_t PWM_actual_Duty ;           //duty cycle is represented by a number from 0->255


	  PWM_actual_Duty = (DutyCycle * TIMER_MAX_VALUE) / DUTY_CYCLE_MAX_VALUE;
	  /*Output Compare Register OCRx
	   * contains an 8-bit value that is continuously compared
	   * with the counter value (TCNTx). A match can be used to
	   * generate an output compare interrupt
	   **/
	  switch(u8TimerNumber)
	  {

#ifdef ENABLE_TIMER_2
	  case TIMER_2:
		  OCR2 = PWM_actual_Duty;
		  break;
#else
	  case TIMER_0:
		  OCR0 = PWM_actual_Duty;
		  break;
#endif
	  default:
		  break;
	  }
}


