 /***************************************************************
 *  Source File: EF_nRF2401.c
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
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "../ServiceLayer/std_types.h"
#include "../MCAL/EF_SPI.h"
#include "../MCAL/EF_SPI_cfg.h"
#include "../MCAL/EF_UART.h"
#include "EF_nRF2401_cfg.h"

/* --------------- Global Variables ----------------------------------- */
U8_t temp_Buffer[32] = {0};

/* --------------- Local Functions ------------------------------------ */

/*********************************************************************
* Function    : nRF_writeRegister
*
* DESCRIPTION : to write defined value to a specific register
*
* PARAMETERS  : U8_t Reg_Add: register address
* 				U8_t value  : value wanted to write to this register
*
* Return Value: None
**********************************************************************/
void nRF_writeRegister (U8_t Reg_Add, U8_t value)
{
	LOW_CSN;
	/* send write command with the address of wanted register then send data*/
	EF_SpiTransfer (W_REG_CMD|Reg_Add);
	EF_SpiTransfer (value);

	HIGH_CSN;
}


/* --------------- API Functions ------------------------------------- */


/*********************************************************************
* Function    : EF_void_nRF_init()
*
* DESCRIPTION : initialise the SPI ,External Interrupt and nRF module
*
* PARAMETERS  : None
*
* Return Value: None
**********************************************************************/
void EF_void_nRF_init()
{
	/* initialise the SPI module */
	EF_SpiInit(MASTER_TYPE);

	/* to ensure that  CE and CSN as the default ==> CE: low and CSN: high*/
	CE_DDR  |= 1<<CE_PIN;
	CE_PORT &= ~(1<<CE_PIN);
	HIGH_CSN;

	/*
	 * Enable External Interrupt ,if IRQ pin is needed to use
	 * (connect it the corresponding pin (ex. INT0))
	 * * */
	/* Set external interrupt on falling edge */
	MCUCR = ((0<<ISC11)|(0<<ISC10)|(1<<ISC01)|(0<<ISC00));
	/* Activate INT0 */
    GICR  = ((0<<INT1)|(1<<INT0));
    /* ensure the flag is zero */
    GIFR   |=  1<<INTF0;
    /* enable global interrupt */
    sei();

    /* delay large than 10ms */
	_delay_ms(50);

	/* define the channel */
	nRF_writeRegister (RF_CHANNEL_REGISTER, CHANNEL );

	/* Enable and define RX Add pipe */
	nRF_writeRegister (EN_RXADDR_REGISTER, EN_ADD_P0);

	/* data rate & power dB */
	nRF_writeRegister (RE_SETUP_REGISTER, RF_DR_1Mbps|RF_PWR );

	/* define Add. width */
	nRF_writeRegister (SETUP_AW_REGISTER, ADD_WID_5Byte );

	/*
	 * Receive address data pipe 0. 5 Bytes maximum length. (LSByte is written first.
	 * Write the number of bytes defined by SETUP_AW)
	 * * */
	LOW_CSN;
	EF_SpiTransfer (W_REG_CMD|RX_ADDR_P0_REGISTER);
	temp_Buffer[0] = temp_Buffer[1] = temp_Buffer[2] = temp_Buffer[3] = temp_Buffer[4] = ADD_RX_P0;
	EF_void_SPI_TransferArray (temp_Buffer, temp_Buffer, 5 );
	HIGH_CSN;

	/*
	 * Transmit address. Used for a PTX device only . (LSByte is written first) Set RX_ADDR_P0
	 * equal to this address to handle automatic acknowledge if this is a PTX device with
	 * Enhanced ShockBurst™ enabled.
	 * * */
	LOW_CSN;
	EF_SpiTransfer (W_REG_CMD|TX_ADDR);
	temp_Buffer[0] = temp_Buffer[1] = temp_Buffer[2] = temp_Buffer[3] = temp_Buffer[4] = ADD_RX_P0;
	EF_void_SPI_TransferArray (temp_Buffer, temp_Buffer, 5 );
	HIGH_CSN;

	if (STATIC_PW == 1)
	{
	    /* define Payload (data) width if static  */
		nRF_writeRegister (RX_PW_P0_REGISTER, PAYLOAD_WIDTH_BYTES);

	}
	else
	{
		/* Enable Dynamic Payload length */
		LOW_CSN;
		/* make these commands to activate DYNPD and FEATURE registers */
		EF_SpiTransfer (ACTIVATE_CMD);
		EF_SpiTransfer (0x73);
		HIGH_CSN;

		nRF_writeRegister (EN_AA_REGISTER, 1<<PIPE_0);
		nRF_writeRegister (FEATURE_REGISTER,1<<EN_DPL);
		nRF_writeRegister (DYNPD_REGISTER,1<<PIPE_0);
	}

	if ( NOACK )
	{
		/* Enables the W_TX_PAYLOAD_NOACK command */
		LOW_CSN;
		EF_SpiTransfer (ACTIVATE_CMD);
		EF_SpiTransfer (0x73);
		HIGH_CSN;
		nRF_writeRegister (FEATURE_REGISTER, 1<<EN_DYN_ACK);
	}

	if (AUTO_RETRANSMIT)
	{
		/* define the Auto Retransmit Delay and the Auto Retransmit Counts */
		nRF_writeRegister (SETUP_RETR_REGISTER, (ARD_4000uS)|(ARC_15_RET));
	}

	if (AUTO_ACKNOWLEDGEMENT)
	{
		 /* Enable Auto Acknowledgement for pipe 0 */
		nRF_writeRegister (EN_AA_REGISTER, 1<<PIPE_0);
	}

	/* Enable CRC and Power up */
	nRF_writeRegister (CONFIG_REGISTER, (1<<EN_CRC)|(1<<PWR_UP));

	/* delay to ensure the module is power up and stable */
	_delay_ms(100);
}


/*********************************************************************
* Function    : EF_void_nRF_TXSetup()
*
* DESCRIPTION : establish the module to transmit
*
* PARAMETERS  : None
*
* Return Value: None
**********************************************************************/
void EF_void_nRF_TXSetup(void)
{
    /* put PRIM = 0 to define it is the transmit mode */
	nRF_writeRegister (CONFIG_REGISTER, (1<<EN_CRC)|(1<<PWR_UP));

	/* delay more than 130uS for TX Setting */
	_delay_us(150);

}

/*********************************************************************
* Function    : EF_BOOLEAN_nRF_SendData
*
* DESCRIPTION : transmit data by the nRF module
*
* PARAMETERS  : U8_t* Data: pointer to transmitted data
* 			    U8_t DataLength: no of bytes of transmitted data
*
* Return Value:  BOOLEAN to check for Errors
**********************************************************************/
BOOLEAN EF_BOOLEAN_nRF_SendData(U8_t* Data, U8_t DataLength)
{
	/* don't forget to call EF_void_nRF_TXSetup Function first  */
	BOOLEAN b_Status=1;
	/* check for length */
	if (DataLength > PAYLOAD_WIDTH_BYTES)
	{
		b_Status = FALSE;
	}

	/* delay more than 130uS for TX Setting */
	_delay_us(150);

	/* ensure and make the TX Buffer is empty */
	LOW_CSN;
	EF_SpiTransfer (FLUSH_TX_CMD);
	HIGH_CSN;

	/* send by SPI the TX command then data, the command differs if NOACK is enabled */
	if ( NOACK )
	{
		LOW_CSN;
		EF_SpiTransfer (W_TX_PAY_NOACK_CMD);
		EF_void_SPI_TransferArray (Data, temp_Buffer, DataLength );
		HIGH_CSN;

	}
	else
	{
		LOW_CSN;
		EF_SpiTransfer (W_TX_PAYLOAD_CMD);
		EF_void_SPI_TransferArray (Data, temp_Buffer, DataLength );
		HIGH_CSN;

	}

	/* make CE =1 least 10uS to transmit data wireless */
	CE_PORT |= 1<<CE_PIN;
	_delay_us(10);
	CE_PORT &= ~(1<<CE_PIN);


	return b_Status;
}


/*********************************************************************
* Function    : EF_BOOLEAN_nRF_RXSetup()
*
* DESCRIPTION : establish the module to receive
*
* PARAMETERS  : None
*
* Return Value: None
**********************************************************************/
void EF_BOOLEAN_nRF_RXSetup(void)
{
    /* make PRIM = 1 to enable the receive mode */
	nRF_writeRegister (CONFIG_REGISTER, (1<<EN_CRC)|(1<<PWR_UP)|(1<<PRIM_RX));

	/* Activate RX ,listen for data*/
	CE_PORT |= 1<<CE_PIN;

	/* delay more than 130uS for RX Setting */
	_delay_us(150);
}

/*********************************************************************
* Function    : EF_BOOLEAN_nRF_GetData
*
* DESCRIPTION : receive data by the nRF module
*
* PARAMETERS  : U8_t* Data: pointer to received data
* 			    U8_t DataLength: no of bytes of received data
*
* Return Value:  BOOLEAN to check for Errors
**********************************************************************/
BOOLEAN EF_BOOLEAN_nRF_GetData(U8_t* Data , U8_t DataLength)
{
	U8_t Status = 0;
	/* check for length */
	if (DataLength > PAYLOAD_WIDTH_BYTES)
	{
		return FALSE;
	}

	/* wait until RX flag = 1 */
	do
	{
		LOW_CSN;
		/* send No operation command to receive Status register (SPI Mythology )*/
		Status = EF_SpiTransfer (NOP_CMD);
		HIGH_CSN;

	}while((Status & 1<<RX_DR) == 0);

	/* send read command then get data */
	LOW_CSN;
	EF_SpiTransfer (R_RX_PAYLOAD_CMD);
	EF_void_SPI_TransferArray(temp_Buffer, Data , DataLength);
	HIGH_CSN;

#if (GET_WIDTH_IF_DYNAMIC == 1)
	if (STATIC_PW == 0)
	{
		/* Dynamic , to print the data width*/
		LOW_CSN;
		Status = EF_SpiTransfer(R_RX_PL_WID);
		EF_void_SPI_TransferArray(temp_Buffer, temp_Buffer , 1);
		EF_void_UART_HextoASCII(&temp_Buffer[0]);
		HIGH_CSN;

	}
#endif

	/* remove the data from RX Buffer to help receiving data again */
	LOW_CSN;
	EF_SpiTransfer (FLUSH_RX_CMD);
	HIGH_CSN;

	/* make RX flag = 0 to help receiving data again */
	nRF_writeRegister (STATUS_REGISTER, (1<<RX_DR));

	return Status;
}


/*********************************************************************
* Function    : EF_u8_nRF_ReadRegister
*
* DESCRIPTION : read specific register located the nRF module
*
* PARAMETERS  : U8_t Reg_Add: address of wanted register
*
* Return Value: the data located in this register
**********************************************************************/
U8_t EF_u8_nRF_ReadRegister(U8_t Reg_Add)
{
	U8_t data;

	/* send read register command with register Address then get data */
	LOW_CSN;
	EF_SpiTransfer(R_REG_CMD | Reg_Add);
	data = EF_SpiTransfer(NOP_CMD);
	HIGH_CSN;

	return data;
}


/* if using IRQ pin as External Interrupt */
ISR ( EXT_INT_VECT )
{
	U8_t Status = 0;

	/* make Flag=0 to help in finding Edges next times */
	GIFR |= 1 << INTF1;

	/* read and send any register if you want */
	U8_t data_test = EF_u8_nRF_ReadRegister (STATUS_REGISTER);
    EF_void_UART_HextoASCII(&data_test );

    /* get the status register to know which flag interrupted */
	LOW_CSN;
	_delay_us(10);
	Status = EF_SpiTransfer (NOP_CMD);
	HIGH_CSN;

	/* if it is ACK that ensure the data was sent correctly, zeroing the flag*/
	if ( Status & (1<<TX_DS))
	{
		nRF_writeRegister (STATUS_REGISTER, (1<<TX_DS));
	}

	/* if it is Max. Retransmit flag , start transmit again */
	else if ( Status & (1<<MAX_RT))
	{
		/* test led */
		PORTD ^= 1<<6;
		/* zeroing the flag */
		nRF_writeRegister (STATUS_REGISTER, (1<<MAX_RT));
		/* try starting transmit again */
		CE_PORT |= 1<<CE_PIN;
		_delay_us(10);
		CE_PORT &= ~(1<<CE_PIN);

	}

	/* if TX buffer was Full, flush it */
	else if ( Status & (1<<TX_FULL))
	{
		nRF_writeRegister (STATUS_REGISTER, (1<<TX_FULL));

		LOW_CSN;
		EF_SpiTransfer (FLUSH_TX_CMD);
		HIGH_CSN;
	}

}




