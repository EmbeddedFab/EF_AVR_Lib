/***************************************************************
 *  Source File: EF_InputCapture.c
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

/*- INCLUDES  ---------------------------------------------------*/
#include "EF_InputCapture.h"
#include <avr/io.h>
#include "EF_SpechialTIMER.h"
#include <util/delay.h>
#include "EF_DIO.h"

/*- DEFINITION  -------------------------------------------------*/
#define INPUT_CAPTURE_PIN				6
#define INPUT_CAPTURE_PORT				'd'
/* choose prescaler */
#define PRESCLR1_VALUE 		            0x01
#define PRESCLR8_VALUE 		            0x02
#define PRESCLR64_VALUE 	            0x03
#define PRESCLR256_VALUE 	            0x04
#define PRESCLR1024_VALUE 	            0x05

#define PRESCALER_VALUE					PRESCLR64_VALUE
/*
 * edge Select bit(6) in TCCR1B register
 * 1: rising edge
 * 0: falling Edge
 *  * */
#define INPUT_CAPTURE_EDGE_SELECT		6

/*
 * to use Noise cancelling, put 1 in it's bit(7) in TCCR1B register
 * it will make a delay 4 clock cycle to do it's functionality
 * * */
#define INPUT_CAPTURE_NOISE_CANCEL		7

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
 extern void EF_void_InputCapture_Init(void)
 {
	 /* make Input capture pin pulling up */
	 /*TODO : pull up or down (i/p 0r o/p)*/
//	 EF_S8_DIO_SetPin(INPUT_CAPTURE_PORT, INPUT_CAPTURE_PIN);
	 /* using Normal Mode in Timer1 */
	 TCCR1A = 0;
 }

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
 extern BOOLEAN EF_BOOLEAN_InputCapture_GetPulse(U16_t* Pulse_ptr, U16_t TimeOut)
 {
	/* to save the first edge time in it */
	U16_t Pulse_FirstEdge = 0;
	/* Status flag helps in return */
	BOOLEAN GetPulseFlag = SUCESS;

	 /* select edge (rising) and prescaler */
	TCCR1B = (1<<INPUT_CAPTURE_EDGE_SELECT)|(PRESCALER_VALUE);

	/* using Special Timer to be unstuck */
	EF_void_TimerCreate(INPUT_CAPTURE_ID, TimeOut);
	EF_void_TimerStart(INPUT_CAPTURE_ID);

	/* make Input capture unstuck  + Wait for the wanted  Edge */
	while( (!(TIFR&(1<<ICF1)))  && (!EF_BOOLEAN_TimerCheck(INPUT_CAPTURE_ID)) );

	/* extracting from the previous loop because of receiving byte or Time out*/
	if (EF_BOOLEAN_TimerCheck(INPUT_CAPTURE_ID) == TRUE)
	{
		GetPulseFlag = FAILURE;
	}

	/* stop the Input Capture timer */
	EF_void_TimerStop(INPUT_CAPTURE_ID);
	EF_void_TimerReset(INPUT_CAPTURE_ID);
	/* get the time of first edge */
	Pulse_FirstEdge = ICR1;
	/* clear flag by write one in it */
	TIFR |= 1<<ICF1;

	/*
	 * to get the pulse,get the time of second edge and subtract it from the first edge
	 * but check first for getting the first edge correctly
	 * * */
	if (GetPulseFlag == SUCESS)
	{
		 /* convert the edge (falling ) and prescaler */
		TCCR1B = (0<<INPUT_CAPTURE_EDGE_SELECT)|(PRESCALER_VALUE);
		/* using Special Timer to be unstuck */
		EF_void_TimerCreate(INPUT_CAPTURE_ID, TimeOut);
		EF_void_TimerStart(INPUT_CAPTURE_ID);

		/* make Input capture unstuck  + Wait for the wanted  Edge */
		while( (!(TIFR&(1<<ICF1))) && (!EF_BOOLEAN_TimerCheck(INPUT_CAPTURE_ID)) );

		/* extracting from the previous loop because of receiving byte or Time out*/
		if (EF_BOOLEAN_TimerCheck(INPUT_CAPTURE_ID) == TRUE)
		{
			GetPulseFlag = FAILURE;
		}

		/* stop the Input Capture timer */
		EF_void_TimerStop(INPUT_CAPTURE_ID);
		EF_void_TimerReset(INPUT_CAPTURE_ID);
		/* get the pulse by subtract the Second Edge from First Edge*/
		*Pulse_ptr = ICR1 - Pulse_FirstEdge;
		/* clear flag by write one in it */
		TIFR |= 1<<ICF1;
	}

	return GetPulseFlag;

 }

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
 extern BOOLEAN EF_BOOLEAN_InputCapture_GetPeriod(U16_t* Period_ptr, U16_t TimeOut)
 {
	 /* to save the first edge time in it */
	U16_t Period_FirstEdge = 0;
	/* Status flag helps in return */
	BOOLEAN GetPeriodFlag = SUCESS;

	 /* select edge (rising) and prescaler */
	TCCR1B = (1<<INPUT_CAPTURE_EDGE_SELECT)|(PRESCALER_VALUE);

	/* using Special Timer to be unstuck */
	EF_void_TimerCreate(INPUT_CAPTURE_ID, TimeOut);
	EF_void_TimerStart(INPUT_CAPTURE_ID);

	/* make Input capture unstuck  + Wait for the wanted  Edge */
	while( (!(TIFR&(1<<ICF1)))  && (!EF_BOOLEAN_TimerCheck(INPUT_CAPTURE_ID)) );

	/* extracting from the previous loop because of receiving byte or Time out*/
	if (EF_BOOLEAN_TimerCheck(INPUT_CAPTURE_ID) == TRUE)
	{
		GetPeriodFlag = FAILURE;
	}

	/* stop the Input Capture timer */
	EF_void_TimerStop(INPUT_CAPTURE_ID);
	EF_void_TimerReset(INPUT_CAPTURE_ID);
	/* get the time of first edge */
	Period_FirstEdge = ICR1;
	/* clear flag by write one in it */
	TIFR |= 1<<ICF1;

	/*
	 * to get the period, waiting the next rising edge and then subtract
	 * the second time from the first one to get the time period
	 * * */
	if (GetPeriodFlag == SUCESS)
	{

		/* using Special Timer to be unstuck */
		EF_void_TimerCreate(INPUT_CAPTURE_ID, TimeOut);
		EF_void_TimerStart(INPUT_CAPTURE_ID);

		/* make Input capture unstuck  + Wait for the wanted  Edge */
		while( (!(TIFR&(1<<ICF1)))  && (!EF_BOOLEAN_TimerCheck(INPUT_CAPTURE_ID)) );

		/* extracting from the previous loop because of receiving byte or Time out*/
		if (EF_BOOLEAN_TimerCheck(INPUT_CAPTURE_ID) == TRUE)
		{
			GetPeriodFlag = FAILURE;
		}

		/* stop the Input Capture timer */
		EF_void_TimerStop(INPUT_CAPTURE_ID);
		EF_void_TimerReset(INPUT_CAPTURE_ID);
		/* get the pulse by subtract the Second Edge from First Edge*/
		*Period_ptr = ICR1 - Period_FirstEdge;
		/* clear flag by write one in it */
		TIFR |= 1<<ICF1;
	}

	return GetPeriodFlag;

 }
