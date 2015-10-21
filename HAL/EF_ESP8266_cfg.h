/***************************************************************
 *  Source File: EF_ESP8266_cfg.h
 *
 *  Description: ESP8266 wifi driver
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
#ifndef _EF_ESP8266_CFG_H_
#define _EF_ESP8266_CFG_H_

/* ------------------------- Definitions ----------------------*/
#define ESP8266_UART_NUMBER UART5
#define ESP8266_UART_BASE   UART5_BASE

#define USING_ISR           0         /* Put 0 to work without ISR and put 1 if using ISR */

#define MAX_CMD_SIZE                35        /* to define the size of CommandArray */
#define MAX_PACKET_SIZE             200       /* to define the max size of Array RX Packet Buffer */
#define MAX_DIGIT_FOR_ITOA          4         /* to define the max. number of Digit using in itoa function, until now using in SMS read and BaudRate */
#define MAX_DIGITS_Of_MOBILE_NUMBER 11        /* using in check mobile number function */
#define MAX_ATTEMPS                 3         /* using in send data function to repeat steps until reaching this number if error occured */
#define SIZE_OF_RETURNED_ARRAY      100       /* Max size of returned Array (RX UART Buffer) */
#define FIRST_DATA_ELEMENT         9




#endif
