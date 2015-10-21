 /***************************************************************
 *  Source File: EF_UltraSonic.c
 *
 *  Description: Simple driver for UltraSonic Sensor
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
/**************************************************
 * include files
 *************************************************/
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "EF_UltraSonic.h"
#include "../MCAL/EF_TIMER.h"
#include "EF_LCD.h"
#include "../ServiceLayer/std_types.h"
#include <string.h>
#include <stdlib.h>
#include "../MCAL/EF_DIO.h"
/**************************************************
 * Define
 *************************************************/
#define F_CPU 			8000000UL
/* Trigger */
#define TRIGGER_PORT    'b'
#define TRIGGER_PIN 	 0  /* starts from 0 to 7 */
/* Echo */
#if INTERRUPT_BASED ==   0
#define ECHO_PORT 	    'd'
#define ECHO_PIN 	 	 3  /* starts from 0 to 7 */
#define ECHO		    (EF_S8_DIO_CheckPin(ECHO_PORT,ECHO_PIN ))
#endif
/* Led */
#define LED_PORT		'd'
#define LED_PIN 		 7
/**************************************************
 * global Variables
 *************************************************/
/* time of one timer clock cycle */
const float ClockCycleTime_uS = TIMER_PRESCALE/(F_CPU/1000000); //prescaler/freq .. (uS)
/* Stores the NumOfTimerTicks length */
U16_t NumOfTimerTicks = 0;
/* convert Timer ticks to time */
U16_t PulseTime_uS = 0;
/* get the final distance_cm */
U16_t PulseDistance_Cm = 0;
char string1[5] = {0};

/************************************************************************
 ********************** Global API Functions ****************************
 ************************************************************************/

/*********************************************************************
 * Function    : void EF_UltraSonic_Init();
 *
 * DESCRIPTION : This function used to Initialise UltraSonic Sensor:
 * 				 	-Timer Initialise
 * 				 	-LCD Initialise
 * 				 	-Trigger Initialise
 * 				 	-Echo Initialise
 * 				 	-Test led Initialise
 *
 * PARAMETERS  : None
 *
 * Return Value: None
 ***********************************************************************/
void EF_UltraSonic_Init()
{
	/* Timer Initialise*/
	EF_void_Timer_PORTinit();

	EF_void_LCD_init();

	/* Trigger Initialise */
	EF_B_DIO_InitPin  (TRIGGER_PORT, TRIGGER_PIN ,OUTPUT);
 	EF_S8_DIO_ClearPin(TRIGGER_PORT, TRIGGER_PIN);

 	/* Echo Initialise  */
#if INTERRUPT_BASED == 1
	GICR   |= (1<<INT1);
	MCUCR  |= (1<<ISC10); /* any logical change*/
	GIFR   |=  1<<INTF1;
	sei();
#else
	EF_B_DIO_InitPin   (ECHO_PORT, ECHO_PIN , INPUT );
	EF_S8_DIO_ClearPin (ECHO_PORT, ECHO_PIN);
#endif
	/* test led Initialise */
	EF_B_DIO_InitPin  (LED_PORT, LED_PIN , OUTPUT);
}

/**************************************************************************
 * Function    : U16_t EF_UltraSonic_GetDistance ();
 *
 * DESCRIPTION : This function used to trigger the sensor then get the distance
 * 				 between UltraSonic Sensor  and any barrier front of this Sensor
 *
 * PARAMETERS  : Void.
 *
 * Return Value: Distance_cm from any barrier front of UltraSonic Sensor
 * 				 if Interrupt based return 1 and ISR will calculate it and print in LCD
 ***************************************************************************/
U16_t EF_UltraSonic_GetDistance ()
{
	/* UltraSonic Trigger , 10uS high*/
	 EF_S8_DIO_SetPin (TRIGGER_PORT, TRIGGER_PIN);
	_delay_us(10);
	EF_S8_DIO_ClearPin(TRIGGER_PORT, TRIGGER_PIN);
#if INTERRUPT_BASED == 0

	/* polling until receive rising edge from Echo pin*/
	while (!ECHO);
	/* Echo rising edge occurred */
	if (ECHO)
	{
		/*
		 * start timer from 0, to define the timer clock cycles that took by the UltraSonic
		 * Sensor Echo from it's rising Edge to it's Falling Edge, and then multiply these
		 * cycles by the time of one timer clock cycle.
		 * So, we have the time in uS. To get Distance_cm convert it by this equation:
		 * Distance_cm = Time_uS / 58
		 * * */
		TIMER_REG = 0;
		TIFR |= TOV_MASK;				    /* make flag = 0 */
		EF_S8_DIO_SetPin (LED_PORT, LED_PIN );  /* test by led */
		GIFR |= 1 << INTF1; 				    /* make flag = 0 */
		while (ECHO);							/* waiting until Echo falling Edge*/
		NumOfTimerTicks = TIMER_REG;
		TIFR |= TOV_MASK;					 /* make flag = 0 */
		EF_S8_DIO_ClearPin (LED_PORT, LED_PIN ); /* test by led */
		PulseTime_uS = (U16_t) (ClockCycleTime_uS * NumOfTimerTicks);  /* convert Timer ticks to time */
		PulseDistance_Cm = PulseTime_uS / 58;    /* get the distance_cm */
		TIFR |= TOV_MASK;					 /* make flag = 0 */
	}
	return PulseDistance_Cm;
#else
	return 1;
#endif
}




#if INTERRUPT_BASED == 1
/* (INT1) PD3 ,  Interrupt type is any change */
ISR (INT1_vect)
{
	static volatile U8_t Rising_flag = 1;

	if (Rising_flag == 1)
	{
		/*
		 * start timer from 0, to define the timer clock cycles that took by the UltraSonic
		 * Sensor Echo from it's rising Edge to it's Falling Edge, and then multiply these
		 * cycles by the time of one timer clock cycle.
		 * So, we have the time in uS. To get Distance_cm convert it by this equation:
		 * Distance_cm = Time_uS / 58
		 ***** */
		TIMER_REG = 0;
		TIFR |= TOV_MASK;				    /* make flag = 0 */
		EF_S8_DIO_SetPin (LED_PORT, LED_PIN );  /* test by led */
		Rising_flag = 0;					    /* to enter the "else" in the second entry to this ISR
												  in the falling Edge*/
		GIFR |= 1 << INTF1; 				    /* make flag = 0 */
	}
	else
	{
		NumOfTimerTicks = TIMER_REG;
		TIFR |= TOV_MASK;					 /* make flag = 0 */
		EF_S8_DIO_ClearPin (LED_PORT, LED_PIN ); /* test by led */
		PulseTime_uS = (U16_t) (ClockCycleTime_uS * NumOfTimerTicks);  /* convert Timer ticks to time */
		PulseDistance_Cm = PulseTime_uS / 58;    /* get the distance_cm */

		/* print in LCD */
		EF_void_LCD_Clear_Screen();
		itoa(PulseDistance_Cm, string1, 10);
		EF_void_LCD_goto(1, 2);
		EF_void_LCD_print( (U8_t *)string1);

		Rising_flag = 1;						 /* to enter the "if" condition, if trigger to UltraSonic is happened */
		GIFR |= 1 << INTF1;						 /* make flag = 0 */
	}
}

#endif

