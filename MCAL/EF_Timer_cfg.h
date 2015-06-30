 /***************************************************************
 *  Source File: EF_Timer_cfg.h
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

#ifndef TIMER_CFG_H
#define TIMER_CFG_H
#include <avr/io.h>
#include "../ServiceLayer/std_types.h"
#include "EF_SpechialTIMER.h"

/**************************************************
 * Definitions
 *************************************************/
#define TIMER_CONTROL_REG		TCCR2		/* Timer Control Register       */
#define TIMER_REG				TCNT2		/* Timer Register     		    */
#define TOIE2_MASK				0x40		/* Timer2 Interrupt Enable mask */
#define TOV2_MASK				0x40		/* Timer2 Over Flow flag  mask  */
#define TOIE_MASK      		    TOIE2_MASK	/* Interrupt Enable bit mask    */
#define TOV_MASK       		    TOV2_MASK   /* Over Flow flag bit mask      */
#define TIMER_OFV_VECTOR        TIMER2_OVF_vect

#define TIMER_CLK_STOP			0x00		/* Timer Stopped			    */
#define TIMER_CLK_DIV1			0x01		/* Timer clocked at F_CPU       */
#define TIMER_CLK_DIV8			0x02		/* Timer clocked at F_CPU/8	    */
#define TIMER_CLK_DIV64			0x04		/* Timer clocked at F_CPU/64    */
#define TIMER_CLK_DIV256		0x06		/* Timer clocked at F_CPU/256   */
#define TIMER_CLK_DIV1024		0x07		/* Timer clocked at F_CPU/1024  */

#define TIMER_PRESCALE_MASK		0x07		/* Timer Prescaler Bit-Mask	    */


/* default prescale settings for the timer,these settings are applied
 *  when calling timerInit */

#define TIMER1PRESCALE		TIMER_CLK_DIV1024	/* timer 1 prescaler default        */
#define TIMERCYCLES			CYCLES_PER_V_MLI_S	/* cycles per variable Mili seconds */
#define SYSTEMTICKTIME		20

//#define TIMER_PRESCALE		8

#if (TIMER1PRESCALE == TIMER_CLK_DIV1)
#define TIMER_PRESCALE		1
#elif (TIMER1PRESCALE == TIMER_CLK_DIV8)
#define TIMER_PRESCALE		8
#elif (TIMER1PRESCALE == TIMER_CLK_DIV64)
#define TIMER_PRESCALE		64
#elif (TIMER1PRESCALE == TIMER_CLK_DIV256)
#define TIMER_PRESCALE		256
#elif (TIMER1PRESCALE == TIMER_CLK_DIV1024)
#define TIMER_PRESCALE		1024
#endif


#define CYCLES_PER_US       ((F_CPU/ TIMER_PRESCALE )/1000000 ) 		/* system tick is 1 micro */
#define CYCLES_PER_5_MLI_S  ((F_CPU/ TIMER_PRESCALE )/200) 				/* system tick is 5 ms    */
#define CYCLES_PER_10_MLI_S ((F_CPU/ TIMER_PRESCALE )/100) 				/* system tick is 10 ms   */
#define CYCLES_PER_20_MLI_S ((F_CPU/ TIMER_PRESCALE )/50) 				/* system tick is 20 ms   */
#define CYCLES_PER_V_MLI_S  ((F_CPU/ TIMER_PRESCALE )/SYSTEMTICKTIME) 	/* system tick variable   */



#define MAX_U8_t		255			/* Max. counts of Timer 8 bit Register  */
#define MAX_U16_t		65535		/* Max. counts of Timer 16 bit Register */


#endif

