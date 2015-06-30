 /***************************************************************
 *  Source File: EF_SPI_cfg.h
 *
 *  Description: SPI driver for ATMEGA32
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

#ifndef EF_SPI_CFG_H_
#define EF_SPI_CFG_H_

/**************************************************************
 * Definitions
 **************************************************************/

#define MASTER_TYPE       1
#define SLAVE_TYPE        0

#define SPI_DDR			  DDRB

#define MISO_BIT          6
#define MOSI_BIT          5
#define SCK_BIT           7
#define SS_BIT            4


#define SPI_TRANSMIT
//#define SPI_RECEIVE



#endif /* EF_SPI_CFG_H_ */
