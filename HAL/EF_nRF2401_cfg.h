 /***************************************************************
 *  Source File: EF_nRF2401_cfg.h
 *
 *  Description: Simple driver for nRF2401+ module
 *
 *  History:     Version 1.0  - INIT Version
 *  Date   :     28/7/2015
 *  -------------------------------------------------------
 *  Author :     EmbeddedFab.
 *
 *  Copyright (C) <2015>  <EmbeddedFab>

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

#ifndef _EF_NRF2401_
#define _EF_NRF2401_

/* SPI chip select, to enable the connection to nRF by SPI used LOW_CSN*/
#define LOW_CSN    	     		SPI_PORT &= ~(1<<SS_BIT)
#define HIGH_CSN    	 		SPI_PORT |=  (1<<SS_BIT)

/* CE pin (chip Enable) at the nRF module to enable transmit or receive data */
#define CE_DDR					DDRB
#define CE_PORT					PORTB
#define CE_PIN					3

#define STATIC_PW				1		/* static payload(data) width select */
#define GET_WIDTH_IF_DYNAMIC    0		/* to get the width if dynamic at RX mode */
#define PAYLOAD_WIDTH_BYTES		32		/* payload width, used if static */
#define NOACK					0		/* NOACK select */
#define AUTO_RETRANSMIT			1		/* Auto retransmit select */
#define AUTO_ACKNOWLEDGEMENT	1		/* Enable Auto Acknowledgement */
#define CHANNEL					22		/* define the channel (0-6 bits)*/
#define ADD_RX_P0				0x12	/* Address of RX pipe */
#define EXT_INT_VECT			INT0_vect
/* --------- nRF Register Address --------------*/
#define CONFIG_REGISTER  		0
#define EN_AA_REGISTER      	1
#define EN_RXADDR_REGISTER		2
#define SETUP_AW_REGISTER		3
#define SETUP_RETR_REGISTER 	4
#define RF_CHANNEL_REGISTER		5
#define RE_SETUP_REGISTER		6
#define STATUS_REGISTER			7
#define OBSERVE_TX_REGISTER 	8
#define RX_ADDR_P0_REGISTER		0x0A
#define TX_ADDR					0x10
#define RX_PW_P0_REGISTER   	0x11
#define FIFO_STATUS_REGISTER	0x17
#define DYNPD_REGISTER			0x1C
#define FEATURE_REGISTER 		0x1D
/* ----------- Commands Address ------------------*/
#define R_REG_CMD				0
#define W_REG_CMD		 		0b00100000
#define R_RX_PAYLOAD_CMD		0b01100001
#define W_TX_PAYLOAD_CMD 		0b10100000
#define FLUSH_TX_CMD 			0b11100001
#define FLUSH_RX_CMD			0b11100010
#define ACTIVATE_CMD			0b01010000  /* Followed by 0x73 */
#define W_TX_PAY_NOACK_CMD      0b10110000
#define NOP_CMD					0b11111111
/* ----------- bits in Registers ----------------- */
#define EN_DYN_ACK 				0
#define EN_ACK_PAY  			1
#define EN_DPL					2
#define PIPE_0					0
#define ARD_750uS				5
#define ARD_4000uS				0xf0
#define ARC_2_RET				1
#define ARC_15_RET				0x0f
#define EN_CRC					3
#define PWR_UP  				1
#define PRIM_RX					0  /* 1: Rx , 0: Tx */
#define RX_DR					6
#define TX_DS					5
#define MAX_RT					4
#define TX_FULL					0
#define RF_PWR					6
#define RF_DR_2Mbps				(1<<3)
#define RF_DR_1Mbps				0
#define ADD_WID_5Byte			3
#define EN_ADD_P0				1
#define MASK_RX_DR				6

#endif
