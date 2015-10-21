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
#ifndef _EF_BLUETOOTH_CFG_H_
#define _EF_BLUETOOTH_CFG_H_

/* ------------------------- Definitions ----------------------*/
/* connect pin 34 in module to 3.3v to Enable all AT Command
 * if Connection is Success , then connect it to zero to Communication mode */
#define BLUETOOTH_UART_NUMBER       UART7
#define ENABLE_AT_COMMAND_PORT      'f'
#define ENABLE_AT_COMMAND_PIN        3

#define MAX_DIGIT_FOR_ITOA          4         /* to define the max. number of Digit using in itoa function, until now using in SMS read and BaudRate */
#define SIZE_OF_RETURNED_ARRAY      100       /* Max size of returned Array (RX UART Buffer) */
#define CONNECTED_STATUS             7

#define MASTER_MODE                  1
#define SLAVE_MODE                   0



#endif
