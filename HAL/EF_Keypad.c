/***************************************************************
 *  Source File: EF_KeyPad.c
 *
 *  Description: This simple driver for keypad 3X3
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
#include "EF_KeyPad.h"
#include "../MCAL/EF_DIO.h"

/*************************************************************
 * Global Definitions
**************************************************************/

#define KEYBAD_PORT         'A'
#define FIRST_ROW_PIN        4
#define SECOND_ROW_PIN       3
#define THIRD_ROW_PIN        2
#define FIRST_COLUMN_PIN     7
#define SECOND_COLUMN_PIN    6
#define THIRD_COLUMN_PIN     5
#define ENTER_KEY            2

/*************************************************************
 * API Functions
**************************************************************/
/*********************************************************************
* Function    : void EF_void_KeyPad_Init (void);
*
* DESCRIPTION : This function used to Initialize keypad
*
* PARAMETERS  : Void.
*            
*
* Return Value: Void.
***********************************************************************/
void EF_void_KeyPad_Init (void)
{
    /* make 3 pins output (COLOUMNS) and anther 3 inputs (ROWS) with pull-up
     * normally, input is high. */
	/* TODO : don't use this function as it effects on all pins */
	EF_S8_DIO_InitPort (KEYBAD_PORT, 0xE0);           /* Init Port A "Keypad"*/
	EF_S8_DIO_SetPin   (KEYBAD_PORT, FIRST_ROW_PIN);  /* Enable Internal PullUp Resistor */
	EF_S8_DIO_SetPin   (KEYBAD_PORT, SECOND_ROW_PIN);
	EF_S8_DIO_SetPin   (KEYBAD_PORT, THIRD_ROW_PIN);
}
/*********************************************************************
* Function    : U8_t EF_u8_KeyPad_Scan (void);
*
* DESCRIPTION : This function used to scan the input if it existed
*
* PARAMETERS  : void
*            
*
* Return Value: U8_t : Return number from (1 to 9) if user press any
                       key or 0 if user not press.
***********************************************************************/
U8_t EF_u8_KeyPad_Scan (void)
{
    U8_t KeyPressed = 0;
    /* when SW is pressed , it will be a short circuit between input and output,
     * so input will be low. So Scanning with making output low , will tell
     * the Scan function which SW is pressed. keypad mapping :
     * 	[1|2|3]
     * 	[4|5|6]
     * 	[7|8|9]
     * */
    /* Scan First Column */
    EF_S8_DIO_ClearPin(KEYBAD_PORT, FIRST_COLUMN_PIN);
	EF_S8_DIO_SetPin  (KEYBAD_PORT, SECOND_COLUMN_PIN);
	EF_S8_DIO_SetPin  (KEYBAD_PORT, THIRD_COLUMN_PIN);

    if(!(EF_S8_DIO_CheckPin(KEYBAD_PORT, FIRST_ROW_PIN)))
    {
        KeyPressed = 1;
    }
    else if(!(EF_S8_DIO_CheckPin(KEYBAD_PORT, SECOND_ROW_PIN)))
    {
        KeyPressed = 4;
    }
    else if(!(EF_S8_DIO_CheckPin(KEYBAD_PORT, THIRD_ROW_PIN)))
    {
        KeyPressed = 7;	
    }
/* Scan Second Column */

    EF_S8_DIO_ClearPin(KEYBAD_PORT, SECOND_COLUMN_PIN);
	EF_S8_DIO_SetPin  (KEYBAD_PORT, FIRST_COLUMN_PIN);
	EF_S8_DIO_SetPin  (KEYBAD_PORT, THIRD_COLUMN_PIN);

    if(!(EF_S8_DIO_CheckPin(KEYBAD_PORT, FIRST_ROW_PIN)))
    {
        KeyPressed = 2;
    }
    else if(!(EF_S8_DIO_CheckPin(KEYBAD_PORT, SECOND_ROW_PIN)))
    {
        KeyPressed = 5;
    }
    else if(!(EF_S8_DIO_CheckPin(KEYBAD_PORT, THIRD_ROW_PIN)))
    {
        KeyPressed = 8;	
    }

/* Scan Third Column */

    EF_S8_DIO_ClearPin(KEYBAD_PORT, THIRD_COLUMN_PIN);
	EF_S8_DIO_SetPin  (KEYBAD_PORT, FIRST_COLUMN_PIN);
	EF_S8_DIO_SetPin  (KEYBAD_PORT, SECOND_COLUMN_PIN);
	
    if(!(EF_S8_DIO_CheckPin(KEYBAD_PORT, FIRST_ROW_PIN)))
    {
        KeyPressed = 3;
    }
    else if(!(EF_S8_DIO_CheckPin(KEYBAD_PORT, SECOND_ROW_PIN)))
    {
        KeyPressed = 6;
    }
    else if(!(EF_S8_DIO_CheckPin(KEYBAD_PORT, THIRD_ROW_PIN)))
    {
        KeyPressed = 9;	
    }
	return (KeyPressed);
}



