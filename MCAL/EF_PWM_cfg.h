/***************************************************************
 *  Source File: EF_PWM_cfg.h
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
#ifndef EF_PWM_CFG_H_
#define EF_PWM_CFG_H_

#include <avr/io.h>
#include "../ServiceLayer/std_types.h"

/* TCCR0. (0111 1011) 0x
 * Bit 7    FOC0: writing a logical one to the FOC0 bit, an immediate compare match is forced on the Waveform Generation unit.
 * Bit 6,3  WGM01:0:  1 1 Fast PWM
 * Bit 5:4  COM01:0:  1 1 Set OC0 on compare match, clear OC0 at TOP
 * Bit 2:0  CS02:0: Clock Select  0 1 1 clkI/O/64 (From prescaler )
 * */
#define FAST_PWM						(1<<3)|(1<<6)
#define NON_INVERT						(1<<5)
#define PRESCLR1_VALUE 		            0x01
#define PRESCLR8_VALUE 		            0x02
#define PRESCLR64_VALUE 	            0x03
#define PRESCLR256_VALUE 	            0x04
#define PRESCLR1024_VALUE 	            0x05
#define DISABLE_MODULE_VALUE 		    0x00
/* Timer DDR */
#define TIMER0_DDR						DDRB
#define TIMER2_DDR						DDRD
/* to put in the EF_void_PWM_init function */
#define TIMER_0                         0
#define TIMER_2                         2
/* Out compare pin */
#define OC0_OUTPUT_SETTER               3
#define OC2_OUTPUT_SETTER               7

#define ZER0_INITIALIZER                0x00
/* Max counts of timer0 is 2^8=255 */
#define TIMER_MAX_VALUE                 255
/* max Percentage Value (100%) */
#define DUTY_CYCLE_MAX_VALUE            100

/* #define ENABLE_TIMER_2 */

#endif /* EF_PWM_CFG_H_ */
