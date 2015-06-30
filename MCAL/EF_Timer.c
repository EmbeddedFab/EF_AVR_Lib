 /***************************************************************
 *  Source File: EF_Timer.c
 *
 *  Description: Timer driver for ATMEGA32
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

#include <avr/io.h>
#include <avr/interrupt.h>
#include "EF_Timer.h"
#include "../ServiceLayer/std_types.h"

/**************************************************
 * Global variables
 *************************************************/
static void (*voidFuncPtr)(void);


/****************************************************************************
* Function    : EF_void_Timer_PORTinit
*
* DESCRIPTION : this function initializes the timer module with the configuration data
*
* PARAMETERS  : None
*
* Return Value: None
******************************************************************************/
void EF_void_Timer_PORTinit(void)
{
	
	/* initialize timer 1 , set the prescaler */
	TIMER_CONTROL_REG = (TIMER_CONTROL_REG & ~ TIMER_PRESCALE_MASK) |TIMER1PRESCALE;
	/* reset TCNT1 */
	TIMER_REG = MAX_U8_t - (U8_t)TIMERCYCLES ;
	/* enable TCNT1 overflow */
	TIMSK |= TOIE_MASK;
	/* TOV1 can be cleared by writing a logic one to its bit location */
	TIFR  |= TOV_MASK;
	/* Enable Global interrupt */
	sei();
}


/****************************************************************************
* Function    : EF_void_Timer_SetISRCallBack
*
* DESCRIPTION : this call back function in the timer module.
* 				Attach a user function to a timer interrupt
*
* PARAMETERS  : void (*userFunc)(void)  : pointer to the call back function which
* 				will call in the Timer ISR if timer interrupt is happened
*
* Return Value: None
******************************************************************************/
void EF_void_Timer_SetISRCallBack( void (*userFunc)(void) )
{
		/* the supplied user's function */
		voidFuncPtr = userFunc;
}


/****************************************************************************
* Function    : void_ucPORT_removeTimerISRCallBack
*
* DESCRIPTION : Detach a user function from a timer interrupt
*
* PARAMETERS  : None
*
* Return Value: None
******************************************************************************/
void EF_void_Timer_RemoveTimerISRCallBack(void)
{

		/* set the interrupt function to run nothing */
		voidFuncPtr = NULL;

}


/****************************************************************************
* Function    : ISR(TIMER1_OVF_vect)
*
* DESCRIPTION : Interrupt handler for tcnt1 overflow interrupt
*
* PARAMETERS  : None
*
* Return Value: None
******************************************************************************/

ISR(TIMER_OFV_VECTOR)

{
	/* if a user function is defined, execute it */
	if(voidFuncPtr != NULL)
		voidFuncPtr();

	/* reset TCNT1 */
	TIMER_REG = MAX_U8_t - (U8_t)TIMERCYCLES ;
	/* TOV1 can be cleared by writing a logic one to its bit location */
	TIFR  |= TOV_MASK;

	
}


