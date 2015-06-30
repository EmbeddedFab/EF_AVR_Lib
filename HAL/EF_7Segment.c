/***************************************************************
 *  Source File: EF_7Segment.c
 *
 *  Description: This simple driver for 7-Segment
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

#include "EF_7Segment.h"

#include <avr/io.h>
#include "../MCAL/EF_DIO.h"
#include <util/delay.h>


/*********************************************************************
* Function    : void Segment_Init (void);
*
* DESCRIPTION : This function used to Initialize 7-Segment
*
* PARAMETERS  : void
*
*
* Return Value: Void
***********************************************************************/
void EF_void_Segment_Init (void)
{
	/* used 4 bit for data instead of 7 */
	EF_S8_DIO_InitPort (SEGMENT_DATA_PORT   , 0x0F);
	/* used 4 bit for control the four 7Segemnts */
	EF_S8_DIO_InitPort (SEGMENT_CONTROL_PORT, 0xF0);
}


/*********************************************************************
* Function    : Segment_Display(U8_t SegmentNumber);
*
* DESCRIPTION : This function used to display 7-Segment
*
* PARAMETERS  : U8_t SegmentNumber : Number in range 0 to 9999
*
*
* Return Value: Void
***********************************************************************/
void EF_void_Segment_Display(U16_t SegmentNumber)
{
	U8_t UnitsDigit = 0;
	U8_t TensDigit  = 0;
	U8_t hundreds   = 0;
	U8_t Thousands  = 0;
	U16_t temp;

	/* there are 4 7Segments used as:
	 * [Thousands|hundreds|TensDigit|UnitsDigit] */
	Thousands  = (SegmentNumber / 1000);
	temp       = (SegmentNumber % 1000);
	hundreds   = (U8_t)(temp   / 100);
	temp       =       (temp   % 100);
	TensDigit  = (U8_t)(temp   / 10);
	UnitsDigit = (U8_t)(temp   % 10);

	/* 7segemnts here are common cathode, to turn on any segment make it High
	 * to economize using pins ,multiplexing is used by control pin to each
	 * 7segment and BCD to 7Segment decoder is used to save 3 pins
	 * So All used pins : 4 for Data and 4 for Multiplexing Control
	 *  * */
	if(SegmentNumber >= 1000)
	{
		EF_S8_DIO_ClearPin (SEGMENT_CONTROL_PORT , FOURTH_SEGMENT_ENABLE);
		EF_S8_DIO_SetPin   (SEGMENT_CONTROL_PORT , THIRD_SEGMENT_ENABLE);
		EF_S8_DIO_SetPin   (SEGMENT_CONTROL_PORT , FIRST_SEGMENT_ENABLE);
		EF_S8_DIO_SetPin   (SEGMENT_CONTROL_PORT , SECOND_SEGMENT_ENABLE);

		EF_S8_DIO_WritePort(SEGMENT_DATA_PORT , Thousands);
		_delay_ms (5);
	}
	if(SegmentNumber >= 100)
	{
		EF_S8_DIO_ClearPin (SEGMENT_CONTROL_PORT , THIRD_SEGMENT_ENABLE);
		EF_S8_DIO_SetPin   (SEGMENT_CONTROL_PORT , FOURTH_SEGMENT_ENABLE);
		EF_S8_DIO_SetPin   (SEGMENT_CONTROL_PORT , FIRST_SEGMENT_ENABLE);
		EF_S8_DIO_SetPin   (SEGMENT_CONTROL_PORT , SECOND_SEGMENT_ENABLE);

		EF_S8_DIO_WritePort(SEGMENT_DATA_PORT , hundreds);
		_delay_ms (5);
	}
	if(SegmentNumber >= 10)
	{
		EF_S8_DIO_ClearPin (SEGMENT_CONTROL_PORT , SECOND_SEGMENT_ENABLE);
		EF_S8_DIO_SetPin   (SEGMENT_CONTROL_PORT , FIRST_SEGMENT_ENABLE);
		EF_S8_DIO_SetPin   (SEGMENT_CONTROL_PORT , FOURTH_SEGMENT_ENABLE);
		EF_S8_DIO_SetPin   (SEGMENT_CONTROL_PORT , THIRD_SEGMENT_ENABLE);

		EF_S8_DIO_WritePort(SEGMENT_DATA_PORT , TensDigit);
		_delay_ms (5);
	}
	EF_S8_DIO_SetPin   (SEGMENT_CONTROL_PORT , SECOND_SEGMENT_ENABLE);
	EF_S8_DIO_SetPin   (SEGMENT_CONTROL_PORT , THIRD_SEGMENT_ENABLE);
	EF_S8_DIO_SetPin   (SEGMENT_CONTROL_PORT , FOURTH_SEGMENT_ENABLE);
	EF_S8_DIO_ClearPin (SEGMENT_CONTROL_PORT , FIRST_SEGMENT_ENABLE);

	EF_S8_DIO_WritePort(SEGMENT_DATA_PORT , UnitsDigit);
	_delay_ms (5);

}
