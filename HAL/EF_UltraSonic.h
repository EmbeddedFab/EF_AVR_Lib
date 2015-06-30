 /***************************************************************
 *  Source File: EF_UltraSonic.h
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
#ifndef _ULTRASONIC__
#define _ULTRASONIC__


/**************************************************
 * include files
 *************************************************/
#include "../ServiceLayer/std_types.h"

/**************************************************
 * Define
 *************************************************/
/*
 * to define the way of getting the Echo time by polling or Interrupt
 * if Interrupt put INTERRUPT_BASED equals 1
 * * */
#define INTERRUPT_BASED  1

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
void EF_UltraSonic_Init(void);

/**************************************************************************
 * Function    : U16_t EF_UltraSonic_GetDistance ();
 *
 * DESCRIPTION : This function used to trigger the sensor then get the distance between UltraSonic Sensor
 * 				 and any barrier front of this Sensor
 *
 * PARAMETERS  : Void.
 *
 * Return Value: Distance_cm from any barrier front of UltraSonic Sensor
 * 				 if Interrupt based return 1 and ISR will calculate it and print in LCD
 ***************************************************************************/
U16_t EF_UltraSonic_GetDistance (void);

#endif
