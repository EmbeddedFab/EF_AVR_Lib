 /***************************************************************
 *  Source File: EF_SkyLabGPS.c
 *
 *  Description: Simple driver for SkyLab GPs module
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
//#define TIVAC
#ifdef TIVAC
#include "../MCU/TIVA_uart.h"
#include "utils/uartstdio.c"
#define UART_NUMBER UART5
#else
#include "../MCAL/EF_UART.h"
#include "EF_LCD.h"
#include <stdlib.h>
#endif
#include "EF_SkyLabGPS.h"
#include <string.h>
/**************************************************
 * Global Definitions, Variables
 *************************************************/
#define LATITUDE_START   19		/* start byte of latitude in the RX frame       */
#define LATITUDE_END	 27		/* last byte of latitude in the RX frame        */
#define LONGITUDE_START  31		/* start byte of longitude in the RX frame      */
#define LONGITUDE_END    40		/* last byte of longitude in the RX frame       */
#define VAILD_ELEMENT    17		/* Number of valid byte in the RX frame         */
#define NORTH_OR_SOUTH   29		/* byte indicates the direction in the RX frame */
#define WEST_OR_EAST     42		/* byte indicates the direction in the RX frame */

enum{LATITUDE,LONGITUDE};		/* used in the DegreeToDecimal function 		*/
U8_t RxBuffer[100] ={0};		/* Buffer to receive the RX Frame in it         */


/***************************************************************************
 ************************* Local Functions *********************************
 ***************************************************************************/

/*********************************************************************
* Function    : DegreeToDecimal (U8_t * DegreeMinute, BOOLEAN LatOrLong)
*
* DESCRIPTION : convert the array of degree and minutes (DDMM.MMMM)/(DDDMM.MMMM) to decimal
* 				decimal = degree + minutes/60
*
* PARAMETERS  : DegreeMinute: array of degree and minutes
* 				LatOrLong : take enum LATITUDE or LONGITUDE, to define the array is
* 				(DDMM.MMMM) or (DDDMM.MMMM)
*
* Return Value: double Decimal
**********************************************************************/
double DegreeToDecimal (U8_t * DegreeMinute, BOOLEAN LatOrLong)
{
	U8_t Minutedigit = 0;  /* define the start byte of Minute in DegreeMinute array */
	U8_t index       = 0;  /* iterator 												*/
	double Minute    = 0;  /* has Minute value after converting it from ASCII 		*/
	U16_t Multipier  = 0;
	double Decimal   = 0;  /* has the final converting number 						*/
	if (LatOrLong == LATITUDE)
	{
		/* DDMM.MMMM ,D:Degree and M: Minutes, So start digit of Minutes = 2 */
		Minutedigit = 2;
		Multipier   = 10;
	}
	else if (LatOrLong == LONGITUDE)
	{
		/* DDDMM.MMMM ,so start digit of Minute =3 */
		Minutedigit = 3;
		Multipier   = 100;
	}
	/* Decimal = degree + Minute/60 , first extract Degree then extract minutes then get Decimal */
	for (index = 0; index < Minutedigit; index++)
	{
		/* convert degree from ASCII to integer */
		Decimal = Decimal + (DegreeMinute[index]-'0')*Multipier;
		Multipier= Multipier/10;
	}
	/* Extract Minute and convert to float */
	Multipier = 10;
	/* Minutedigit+7 , because MM.MMMM are 7 digit */
	for (index=Minutedigit; index<(Minutedigit+7); index++)
	{
		/* to avoid the (.) */
		if (index == (Minutedigit + 2))
		{
			continue;
		}
		/* convert Minute from ASCII to integer */
		Minute = Minute +  (DegreeMinute[index]-'0')*Multipier;
		Multipier = Multipier/10;
	}
	/* Decimal = degree + Minute/60 */
	Decimal = Decimal + Minute/60;
	/* send to uartprintf as two integer with . between them */
	return Decimal;
}

#ifdef TIVAC
//uart utils
/*********************************************************************
* Function    : void_InitConsole(void)
*
* DESCRIPTION : initialise the uart utils in PA(0,1)
*
* PARAMETERS  : None
*
* Return Value: None
**********************************************************************/
void void_InitConsole(void)
{

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    SysCtlDelay(5);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    SysCtlDelay(5);
    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);
    UARTClockSourceSet(UART0_BASE, UART_CLOCK_PIOSC);
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
    UARTStdioConfig(0, 115200, 16000000);

}

#endif
/***********************************************************************
 ********************** Global API Functions****************************
 ***********************************************************************/
/*********************************************************************
* Function    : EF_B_SkyLabGPS_Init()
*
* DESCRIPTION : initialise the uart and LCD/UartUtilts
*
* PARAMETERS  : None
*
* Return Value: BOOLEAN to check for Errors
**********************************************************************/
BOOLEAN EF_B_SkyLabGPS_Init()
{
#ifdef TIVAC
	UART_cfg_str  uart_cfg = {UART_NUMBER, 9600, NUMBER_OF_BITS_8, ONE_STOP_BIT,NO_PARITY,FALSE,FALSE,TRUE,TRUE};
	/* uart utilts */
	void_InitConsole();
	UARTprintf("hello \n");
#else
	static UART_cfg_str  uart_cfg = {9600,8,ONE_STOP_BIT,NO_PARITY,FALSE,FALSE,TRUE,TRUE};
	EF_void_LCD_init();
#endif
	EF_void_UART_Init(&uart_cfg);
	/* TODO modify return */
	return 1;
}
/*********************************************************************
* Function    : EF_B_SkyLabGPS_GetPosition( double* D_LatitudePtr, double* D_LongitudePtr)
*
* DESCRIPTION : receive the frames which is sent by GPS module every 1sec,
* 				and parse it to get the latitude and longitude after converting
* 				them to double if data is valid
*
* PARAMETERS  : D_LatitudePtr  : pointer to return the latitude  "double" in it
* 				D_LongitudePtr : pointer to return the longitude "double" in it
*
* Return Value: returns DATA_NOT_VAILD or DATA_VAILD
**********************************************************************/
BOOLEAN EF_B_SkyLabGPS_GetPosition( double* D_LatitudePtr, double* D_LongitudePtr)
{
	volatile U8_t Flag_Frame =  0;   /* indicates the GPRMC frame is received      */
	U8_t latitude [10] 		 = {0};  /* the Latitude DDMM.MMMM ,D:degree ,M:Minute */
	U8_t longitude[10] 		 = {0};  /* the longitude DDDMM.MMMM 				   */
	U8_t RxByte 			 =  0;	 /* to receive byte in it					   */
	U8_t counter 			 =  0;	 /* used as iterator 						   */
	U8_t BufferIndex		 =  0;   /* index of RX buffer 						   */
	while (Flag_Frame == 0)
	{
		/* check for the start of Frame '$' */
#ifdef TIVAC
		EF_BOOLEAN_UART_GetChar ( UART_NUMBER, &RxByte);
#else
		EF_BOOLEAN_UART_GetChar ( &RxByte);
#endif
		if (RxByte == '$')
		{
			/* check if  GPRMC frame , because it has the latitude and longitude */
			for (BufferIndex = 0 ; BufferIndex < 5 ; BufferIndex++ )
			{
#ifdef TIVAC
				EF_BOOLEAN_UART_GetChar ( UART_NUMBER, &RxByte);
#else
				EF_BOOLEAN_UART_GetChar ( &RxByte);
#endif
			}
			/* check if  GPRMC frame */
			if ( (strncmp((const char *)RxBuffer, (const char *)"GPRMC", 5)) == 0)
			{
				Flag_Frame = 1;
				/* receive the remaining bytes of frame */
				do
				{
#ifdef TIVAC
					EF_BOOLEAN_UART_GetChar ( UART_NUMBER, &RxByte);
#else
					EF_BOOLEAN_UART_GetChar ( &RxByte);
#endif
					BufferIndex++;

				}while (*(RxBuffer+BufferIndex-1)!= '\n');

				/* check if data Valid , 'A' means valid , 'V' means not valid  */
	    		if (RxBuffer[VAILD_ELEMENT] != 'A')
	    		{
#ifdef TIVAC
	    			UARTprintf((U8_t*)"not valid data \n");
#else
	    			EF_void_LCD_print ((U8_t*)"not valid data");
#endif
					return DATA_NOT_VAILD;
				}
				/* if data valid, get and print latitude and longitude */
	    		else
	    		{
	    			counter =0;
	    			/* Latitude is DDMM.MMMM */
	    			for (BufferIndex=LATITUDE_START; BufferIndex <=LATITUDE_END ; BufferIndex++)
	    			{

	    				latitude[counter] = RxBuffer[BufferIndex];
	    				counter ++;
	    			}
	    			counter =0;
	    			/* Longitude is DDDMM.MMMM */
	    			for (BufferIndex=LONGITUDE_START ; BufferIndex<=LONGITUDE_END ; BufferIndex++)
	    			{

	    				longitude[counter] = RxBuffer[BufferIndex];
						counter++;
					}
#if 0
					///* RxBuffer[29] = N/S , North or South */
					//UARTprintf ("latitude: %s , %c\n",latitude , RxBuffer[29] );
					///* RxBuffer[29] = E/W, East or West */
					//UARTprintf ("longitude: %s , %c\n",longitude , RxBuffer[42]);
#endif
					/* convert the latitude from degree and minutes "array" to decimal "double"  */
	    			*D_LatitudePtr =  DegreeToDecimal(latitude, LATITUDE);
	    			/* convert the longitude from degree and minutes "array" to decimal "double" */
	    			*D_LongitudePtr = DegreeToDecimal(longitude, LONGITUDE);

				}
			}
		}
	}
	return DATA_VAILD;
}

/*********************************************************************
* Function    : EF_B_SkyLabGPS_PrintPosition( double D_Latitude, double D_longitude)
*
* DESCRIPTION : display the latitude and longitude in LCD/UART_Utilts
*
* PARAMETERS  : D_Latitude  : latitude  in double
* 				D_longitude : longitude in double
*
* Return Value: BOOLEAN to check for Errors
**********************************************************************/
BOOLEAN EF_B_SkyLabGPS_PrintPosition( double D_Latitude, double D_longitude)
{
	U32_t AfterPoint = 0;			/* to get the digits after dot in it */
	/*
	 * Number should be displayed as DD.DDDDDD (2Digit.6Digits) and this number is double
	 * So, integer(Number) is displayed then display (.) then convert the number after
	 * dot to be integer then display it as 6 digit, so if it less than 6 digit display
	 * zeros before it to complete the 6 digit and to be right number
	 * * * */
	/************** latitude ***************/
#ifdef TIVAC
	/* display the integer of D_latitude then (.)*/

	UARTprintf("latitude: %d.", (U16_t) D_Latitude);
#else
	U8_t NumOfDigit =0;				/* to get the number of digit to display zeros before it if it less than 6 digit*/
	U8_t i=0;						/* iterator 																	*/
	U8_t AsciiArray [7] = {0};      /* array to convert the number in it 											*/
	/* convert and display the integer of D_latitude then (.) */
	itoa((U16_t) D_Latitude, (char*) AsciiArray, 10);
	 EF_void_LCD_print_ByLength(AsciiArray, 2);
	EF_void_LCD_send_data('.');
#endif
	AfterPoint = ((double) (D_Latitude - (U16_t) D_Latitude))* 1000000;
#ifdef TIVAC
	/* display the digits after the dot */
	UARTprintf("%06d , %c\n", AfterPoint, RxBuffer[NORTH_OR_SOUTH]);
#else
	/* convert and display the digits after the dot */
	itoa((U16_t) AfterPoint,  (char*) AsciiArray, 10);
	NumOfDigit=0;
	/* get the number of digit */
	while (AfterPoint > 0)
	{
		AfterPoint = AfterPoint/10;
		NumOfDigit++;
	}
	/* display zeros if the number digit <6 */
	for (i=NumOfDigit ; i<6; i++)
	{
		EF_void_LCD_send_data ('0');
	}
	EF_void_LCD_print( (U8_t*) AsciiArray);
	EF_void_LCD_send_data (',');
	EF_void_LCD_send_data (RxBuffer[NORTH_OR_SOUTH]);
#endif
	/************** longitude ***************/
#ifdef TIVAC
	UARTprintf("longitude: %d.", (U16_t) D_longitude);
	#else
	itoa((int) D_longitude,  (char*) AsciiArray, 10);
	EF_void_LCD_print_NUM( (U16_t) D_longitude ,2);
	EF_void_LCD_send_data('.');
	#endif
	AfterPoint = ((double) (D_longitude - (U16_t) D_longitude))* 1000000;
#ifdef TIVAC
	UARTprintf("%06d , %c\n", AfterPoint, RxBuffer[WEST_OR_EAST]);
#else
	itoa((int) AfterPoint,  (char*) AsciiArray, 10);
	NumOfDigit=0;
	while (AfterPoint > 0)
	{
		AfterPoint = AfterPoint/10;
		NumOfDigit++;
	}
	for (i=NumOfDigit ; i<6; i++)
	{
		EF_void_LCD_send_data ('0');
	}
	EF_void_LCD_print( (U8_t*) AsciiArray);
	EF_void_LCD_send_data (',');
	EF_void_LCD_send_data (RxBuffer[WEST_OR_EAST]);
#endif

	//TODO modify return
	return 1;
}

