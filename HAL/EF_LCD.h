/***************************************************************
 *  Source File: EF_LCD.h
 *
 *  Description: This simple driver for LCD 2X16 4bit mode
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
/**************************************************************
 * Notes: This driver can use with 4 bit mode only.
 *
 **************************************************************/

#ifndef EF_LCD_H_
#define EF_LCD_H_

//#define F_CPU 8000000UL
#include "../ServiceLayer/std_types.h"

/*********************************************************************
* Function    : void initLCD(void);
*
* DESCRIPTION : This function used to Initialize LCD Ports and send
* 				suitable default CMD before start display.
*
* PARAMETERS  : Void.
*
* Return Value: Void.
***********************************************************************/
void EF_void_LCD_init();


/*********************************************************************
* Function    : void EF_EF_void_LCD_send_command(U8_t cmnd);
*
* DESCRIPTION : This function used to send CMD to LCD like move or control
*               display mode.
*
* PARAMETERS  : U8_t cmnd:  Command User want to send.
*
* Return Value: Void.
***********************************************************************/
void EF_void_LCD_send_command(U8_t cmnd);


/*********************************************************************
* Function    : void EF_void_LCD_send_data(U8_t data);
*
* DESCRIPTION : This function used to send data to LCD .
*
* PARAMETERS  : U8_t Data:  Data User want to send.
*
* Return Value: Void.
***********************************************************************/
void EF_void_LCD_send_data(U8_t data);


/***********************************************************************
 * Function    : void EF_void_LCD_goto(U8_t y, U8_t x);
 *
 * DESCRIPTION : This function moves the cursor the line (row) y and
* 				 (column) x  on the LCD module.
 *
 * PARAMETERS  : U8_t y: 1 for first row, 2: for the second row
 * 				 U8_t x: from 1 to 16 (represents columns)
 *
 * Return Value: Void.
 ***********************************************************************/
void EF_void_LCD_goto(U8_t y, U8_t x);

/***********************************************************************
* Function    : void EF_void_LCD_print(U8_t *string);
*
* DESCRIPTION : This function used to display String which user
*               wants to display .
*
* PARAMETERS  : U8_t *string:  String User want to send.
*
* Return Value: Void.
***********************************************************************/
void EF_void_LCD_print(unsigned char *string);

/***********************************************************************
* Function    : void EF_void_LCD_print_ByLength
*
* DESCRIPTION : This function used to display String which user
*               wants to display .
*
* PARAMETERS  : U8_t *string:  String User want to send.
* 				U8_t  length:  number of characters to be displayed
*
* Return Value: Void.
***********************************************************************/
void EF_void_LCD_print_ByLength(unsigned char *string, U8_t length);


/***********************************************************************
* Function    : EF_void_LCD_Clear_Screen(void);
*
* DESCRIPTION : This function used to clear the LCD screen
*
* PARAMETERS  : Void.
*
* Return Value: Void.
***********************************************************************/
void EF_void_LCD_Clear_Screen(void);

/***********************************************************************
* Function    : EF_void_LCD_print_NUM(S16_t s16Numberstring,U8_t u8ColNumber);
*
* DESCRIPTION : This function used to convert integer number to ASCII
* 				then print it in LCD
*
* PARAMETERS  : s16Numberstring: integer number wanted to be displayed in LCD
*				u8RowNumber    : to define which row to display ,
*				take 1 for first row, 2: for the second row
*
* Return Value: Void.
***********************************************************************/
void EF_void_LCD_print_NUM(S16_t s16Numberstring,U8_t u8RowNumber);


/***********************************************************************
* Function    :EF_void_uploadCustomChar(U8_t location, unsigned char *character);
*
* DESCRIPTION : This function used to upload a custom char which user wants to map
* 				it using CGRAM (Character Generator Ram Address set).
*
* PARAMETERS  : location : to define which CGRAM ADD offset. It takes from (0 to 7).
* 				character: pointer to 8 pointer to char as CGRAM needs 8 char to map
*
* Return Value: Void.
***********************************************************************/
void EF_void_uploadCustomChar(U8_t location, unsigned char *character);

/***********************************************************************
* Function    : EF_void_LCD_HextoASCII(U8_t *hex);
*
* DESCRIPTION : This function used to convert hex to ASCII, then display it
*
* PARAMETERS  : hex: pointer to hex (one byte)
*
* Return Value: Void.
***********************************************************************/
void EF_void_LCD_HextoASCII(U8_t *hex);

#endif /* EF_LCD_H_ */
