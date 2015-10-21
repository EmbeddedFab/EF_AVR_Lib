/*
 * main.c
 *
 *  Created on: ??þ/??þ/????
 *      Author: Kareem
 */


#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <string.h>
#include <stdlib.h>

#include "HAL/EF_Wavecom.h"
#include "HAL/EF_SkyLabGPS.h"
#include "MCAL/EF_PWM.h"
#include "HAL/EF_LCD.h"
#include "HAL/EF_UltraSonic.h"
//#include "MCAL/EF_I2C.h"
//#include "HAL/EF_7Segment.h"
#include "MCAL/EF_SpechialTimer.h"
#include "MCAL/EF_InputCapture.h"
//#include "HAL/EF_KeyPad.h"
//#include "MCAL/EF_UART.h"
//#include "HAL/EF_E2PROM_24C16.h"
//#include "MCAL/EF_ADC.h"
//#include "HAL/EF_DS1307.h"
//#include "MCAL/EF_DIO.h"
//#include "MCAL/EF_EEPROM.h"
////#define RTC_ENABLE
////#define EEPROM_ENABLE
//
//#define TIMEOUR_1SEC     5000
//
//extern volatile  U16_t SystemTick1Sec;
//extern volatile  U16_t SystemTick1MSec;
//
#define BUZZER_PIN   5
/* 3 global variables used for Wavecom Module , don't comment it. */
volatile U8_t gSmsReadyFlag         = 0;   		 /* flag to define  ready SMS read in the next response */
U8_t gRX_SMS           [MAX_SMS_SIZE]    = {0};  /* Array which will include the Read SMS */
volatile U16_t gNumberOfRxBytes 	= 0;    	 /* save number of SMS bytes */




void LCD_update_time(U8_t hours, U8_t minutes, U8_t seconds);

S8_t temp[2] , string[5];
U8_t u8Counter = 0, Hours, Mintes , Seconds;
U16_t u16DigitalValue =0 ;
U8_t customArray[] = {0x4, 0x1f, 0x11, 0x11, 0x11, 0x1f, 0x1f, 0x1f};
int main (void)
 {



	 U16_t Period;
//	DDRD |= 1<<7;
	PORTD = 0x00;

	EF_void_TimerInit();
	EF_void_LCD_init();
	EF_void_LCD_Clear_Screen();
	EF_void_LCD_goto(1, 2);
//		_delay_ms(1000);
//		EF_void_LCD_print(" EmbeddedFab");
		_delay_ms(1000);

//	GICR  = (1<<INT0);					// Enable INT0
//	MCUCR = (1 << ISC00) | (1 << ISC01);
	sei();
//	EF_void_PWM_init(TIMER_0);
//	EF_void_InputCapture_Init();
//	EF_void_PWM_SetDutyCycle (50 ,TIMER_0);
//	u8Counter = EF_BOOLEAN_InputCapture_GetPeriod (&Period, 100);
//	EF_void_LCD_Clear_Screen();
//	EF_void_LCD_goto(1, 2);
//	EF_void_LCD_print_NUM( Period, 2);
//	EF_void_LCD_print_NUM( u8Counter, 1);

	while(1)
	{
//		PORTD ^= (1 << 7);

//		_delay_ms(100);
		EF_void_uploadCustomChar(0 , customArray);
//		PORTD |= 1 << 7;
		_delay_ms(500);


	}

#if 0
	U8_t temp[2];
	U8_t number;


	DDRD  = 0xff;
	PORTD = 0x00;
	EF_void_LCD_init();
	EF_void_KeyPad_Init();

#if RTC_ENABLE
	void_TIMER_init();
	EF_void_DS1307_Init();
	sei();              /*Enable interrupts*/
	EF_void_DS1307_SetTime(17, 13, 30);
#endif

	EF_void_LCD_Clear_Screen();
	EF_void_LCD_goto(1,2);
	EF_void_LCD_print("Embedded");

#ifdef PWM_test
	/*don't forget init. the PWM*/
	EF_void_PWM_SetDutyCycle (0,TIMER_0);
	_delay_ms(500);
	EF_void_PWM_SetDutyCycle (23,TIMER_0);
	_delay_ms(500);
	EF_void_PWM_SetDutyCycle (50,TIMER_0);
	_delay_ms(500);
	EF_void_PWM_SetDutyCycle (80,TIMER_0);
	_delay_ms(500);
	EF_void_PWM_SetDutyCycle (100,TIMER_0);
	_delay_ms(500);
	_delay_ms(100);
#endif
#ifdef EEPROM_ENABLE
	EF_void_I2C_Init();
	EF_EEPROM_Write_Byte(6,0xA0,0x65);
	number = EF_EEPROM_Read_Byte(6,0xA0);
	if (0x15 == number)
	{
		EF_void_LCD_goto(2,8);
		EF_void_LCD_HextoASCII(&number);
	}
	else
	{
		EF_void_LCD_goto(2,8);
		EF_void_LCD_HextoASCII(&number);
	}
#endif
#ifdef INPUT_CAPTURE_TEST
	/* enable timer init and sei()*/
	EF_void_PWM_init(TIMER_0);
	EF_void_InputCapture_Init();
	EF_void_PWM_SetDutyCycle (90 ,TIMER_0);
	u8Counter = EF_BOOLEAN_InputCapture_GetPeriod (&Period, 100);
	EF_void_LCD_Clear_Screen();
	EF_void_LCD_goto(1, 2);
	EF_void_LCD_print_NUM( Period, 2);
	EF_void_LCD_print_NUM( u8Counter, 1);
#endif
#ifdef ADC_ENABLE
	/* don't forget ADC_init before while */
	u16DigitalValue = EF_u16_ADC_read(0);
	EF_void_LCD_Clear_Screen();
//		u16DigitalValue = (u16DigitalValue*5)/1024;
	itoa(u16DigitalValue, string, 10);
	EF_void_LCD_goto(1, 2);
	EF_void_LCD_print(string);

#endif
#ifdef EEPROM_INTERNAL
	EF_void_EEPROM_WriteByte(0, 0x99);
	number = EF_u8_EPROM_ReadByte (0);
	if (0x15 == number)
	{
		EF_void_LCD_goto(2,8);
		EF_void_LCD_HextoASCII(&number);
	}
	else
	{
		EF_void_LCD_goto(2,8);
		EF_void_LCD_HextoASCII(&number);
	}

#endif
	while(1)
	{
#ifdef RTC_ENABLE
		if((SystemTick1MSec - TimeOut) > TIMEOUR_1SEC)
		{
			TimeOut   = SystemTick1MSec;
			EF_void_DS1307_GetTime(&Hours, &Mintes, &Seconds);
			LCD_update_time(Hours, (Mintes), (Seconds));
			PORTD = 0x7F;
		}
#endif
#ifdef WAVECOM
		/* using ISR */
		volatile U8_t gSecondTimeEntery     = 0;    	 /* flag to define  the second response to get SMS */
		volatile U8_t gNumberOfPunctuation  = 0;     	 /* flag to know when SMS begin */
		volatile U16_t gNumberOfRxBytes 	= 0;    	 /* save number of SMS bytes */
		volatile U16_t counter = 0;					 /* using in SMS array in ISR */

		U8_t gBuffer [50];
		Wavecom_Sms_cfg_str Wavecom_Sms_cfg ={ENABLE,ENABLE,ENABLE};
		char const SMS[] = "Again , it's ATMEGA \n :) ;)";
		Wavecom_Call_cfg_str Wavecom_Call_cfg = {DISABLE, DISABLE};
		Wavecom_ReadSms_cfg_str Wavecom_ReadSms_cfg ={0, All_list, 0 ,10 , (U8_t*)&gBuffer};
		sei();
	//	EF_B_Wavecom_SendSms((U8_t*)"01091067575",11,(U8_t*)SMS, strlen(SMS));
	//	EF_B_Wavecom_Call ((U8_t*)"01117079935",11);
	//	EF_B_Wavecom_ReadSms (&Wavecom_ReadSms_cfg);
		while (1)
		{
	//		PORTD ^= 0xc0;
			EF_B_Wavecom_InitModule();
			EF_B_Wavecom_InitSms(&Wavecom_Sms_cfg);
			EF_B_Wavecom_InitCall (&Wavecom_Call_cfg);
			_delay_ms(1000);
			EF_void_LCD_Clear_Screen();
			EF_void_LCD_goto(1, 1);
			_delay_ms(1000);
			EF_void_LCD_goto(2, 1);
	//		EF_B_Wavecom_CallRelease();

		}




		ISR (USART_RXC_vect)
		{
			 static volatile U8_t RX_data = 0;

			 RX_data = UDR;
			 if ((RX_data != '\r') && (RX_data != '\n'))
			 {
				 if (gSmsReadyFlag == 1)
				 {
					 if (gNumberOfPunctuation == 6)
					 {
						 gRX_SMS[counter] = RX_data;
						 //TODO modify this check.
						 if ( gRX_SMS[counter] == 'K' && gRX_SMS[counter-1] == 'O')
						 {
							 gNumberOfRxBytes   = counter-1;
							 counter=0;
							 gSmsReadyFlag     = 0;
							 gSecondTimeEntery = 0;
						 }
						 counter++;
					 }
					 else
					 {
						 if (RX_data == '\"')
						 {
							 gNumberOfPunctuation++;
						 }

					 }
				 }
				 else
				 {
					 EF_void_LCD_send_data (RX_data);
					 if (RX_data == 'K')
					 {
						 EF_void_LCD_send_data (' ');
					 }
				 }

			 }


		}




#endif

#ifdef SKYLAB_GPS
		double D_Longitude = 0;
		double D_Latitude  = 0;
		BOOLEAN b_Datavaild  = 0;
		EF_void_LCD_init();
		EF_B_SkyLabGPS_Init();
		//timer OS
		EF_void_TimerInit();
	    EF_void_LCD_Clear_Screen();
	    DDRD = 0xc0;
	    while (1)
	    {
	    	b_Datavaild = EF_B_SkyLabGPS_GetPosition( &D_Latitude , &D_Longitude);
	    	if (b_Datavaild == DATA_VAILD)
	    	{
	    		EF_B_SkyLabGPS_PrintPosition(  D_Latitude,  D_Longitude);
	    	}
	    	PORTD ^=0xc0;
	    }
#endif

#ifdef ULTRASONIC
	    /* it used timer of timer.h , be careful if you
			depend on this timer in anther application */
		EF_UltraSonic_Init();
		U8_t Distance_Cm ;
		while (1)
		{
			 Distance_Cm = EF_UltraSonic_GetDistance();
	#if INTERRUPT_BASED ==   0
			EF_void_LCD_Clear_Screen();
			itoa(Distance_Cm, string, 10);
			EF_void_LCD_goto(1, 2);
			EF_void_LCD_print(string);
	#endif
			_delay_ms (20);
		}
#endif
#if KEYPAD_ENABLE
		number = EF_u8_KeyPad_Scan();
		if(number != 0)
		{
			EF_void_LCD_goto(2,1);
			itoa(number,temp,10);
			EF_void_LCD_print(temp);
		}
#endif
	}
#endif
	return 0;
}


//ISR(INT0_vect)
//{
//	PORTD = (1 << 5);
//	_delay_ms(3000);
//}

void LCD_update_time(U8_t hours, U8_t minutes, U8_t seconds)
{
	char temp[2];

	EF_void_LCD_goto(2,6);

	itoa( (int) hours/10,temp,10);
	EF_void_LCD_print( (U8_t*) temp);
	itoa( (int) hours%10,temp,10);
	EF_void_LCD_print( (U8_t*)temp);


	EF_void_LCD_goto(2,9);
	itoa( (int) minutes/10,temp,10);
	EF_void_LCD_print( (U8_t*) temp);
	itoa( (int) (minutes%10),temp,10);
	EF_void_LCD_print( (U8_t*) temp);

	EF_void_LCD_goto(2,12);
	itoa( (int) seconds/10,temp,10);
	EF_void_LCD_print( (U8_t*) temp);
	itoa( (int) seconds%10,temp,10);
	EF_void_LCD_print( (U8_t*) temp);
}

#ifdef WAVECOM
ISR (USART_RXC_vect)
{
	 static volatile U8_t RX_data = 0;

	 RX_data = UDR;
	 if ((RX_data != '\r') && (RX_data != '\n'))
	 {
		 if (gSmsReadyFlag == 1)
		 {
			 if (gNumberOfPunctuation == 6)
			 {
				 gRX_SMS[counter] = RX_data;
				 //TODO modify this check.
				 if ( gRX_SMS[counter] == 'K' && gRX_SMS[counter-1] == 'O')
				 {
					 gNumberOfRxBytes   = counter-1;
					 counter=0;
					 gSmsReadyFlag     = 0;
					 gSecondTimeEntery = 0;
				 }
				 counter++;
			 }
			 else
			 {
				 if (RX_data == '\"')
				 {
					 gNumberOfPunctuation++;
				 }

			 }
		 }
		 else
		 {
			 EF_void_LCD_send_data (RX_data);
			 if (RX_data == 'K')
			 {
				 EF_void_LCD_send_data (' ');
			 }
		 }

	 }


}

#endif

#ifdef nRF2401_RX_MAIN

UART_cfg_str Uart_Paramters = {4800, 8, ONE_STOP_BIT, NO_PARITY,0,0,1,1};
U8_t  data_arr[5];
U8_t data_test;
U8_t RX_Buffer [10][33];
U8_t CRC_Rx =1;
volatile U8_t counter =0 , iterator = 0;

int main()
{
	DDRD |= (1<<7)|(1<<6);
	PORTD |= 1<<7;
	_delay_ms(1000);


	EF_void_UART_Init(&Uart_Paramters);
	EF_void_UART_SendArray((U8_t*)"eee",3);

//	EF_void_SPI_Init(SLAVE_TYPE);
//
//
//	while (1)
//	{
//		EF_u8_SPI_TransferByte(0xEF);
//	}
	EF_void_nRF_init();
	EF_BOOLEAN_nRF_RXSetup();

//	_delay_ms(1);

	while(1)
	{
//		EF_BOOLEAN_nRF_RXSetup();
//		_delay_ms(1);

		for (iterator = 0 ; iterator <3 ; iterator++)
		{
			EF_BOOLEAN_nRF_GetData(&(RX_Buffer[iterator][0]), 32 );
			PORTD ^= 1<<7;
		}

		CRC_Rx = 1;
		for (iterator = 0 ; iterator <3 ; iterator++)
		{
			for (counter = 0 ; counter <31 ; counter++)
			{
				CRC_Rx = (CRC_Rx) ^ (RX_Buffer[iterator][counter]) ^
						  	  	    (RX_Buffer[iterator][counter+1]);
			}
		}
//		EF_BOOLEAN_nRF_GetData ((U8_t*)data_arr , 3);
//		EF_void_UART_SendArray ((U8_t*)data_arr , 3);
//		data_test = EF_u8_nRF_ReadRegister (0x17);
		EF_void_UART_HextoASCII (&CRC_Rx);
		EF_void_UART_PutChar(' ');
//		data_test = EF_u8_nRF_ReadRegister (0x7);
//		EF_void_UART_HextoASCII (&data_test);
//		EF_void_UART_PutChar('-');
//		_delay_ms(100);
//		PORTD ^= 1<<7;
//
//		PORTB &= ~(1<<3);
//		_delay_ms(1);
	}



	return 0;
}
#endif

#ifdef nRF_2401_TX_MAIN

#define F_CPU 1000000UL
#include <util/delay.h>

UART_cfg_str Uart_Paramters = {4800, 8, ONE_STOP_BIT, NO_PARITY,0,0,1,1};
U8_t  data_arr[3] ={0};
U8_t  data_test =0;
 U8_t CRC_Tx =1;
volatile U8_t counter =0 , iterator = 0;
static U8_t Large_array[10][33] = {"0123456789 0123456789 0123456789",
								   "0123455555 0123456789 0123456789",
								   "0123456789 0123444444 0123456789"};

#define LOW_CSN    	     		SPI_PORT &= ~(1<<SS_BIT)
#define HIGH_CSN    	 		SPI_PORT |=  (1<<SS_BIT)

int main()
{
	DDRD |= 1<<7|1<<6;

	EF_void_UART_Init(&Uart_Paramters);

	EF_void_nRF_init();

	EF_void_nRF_TXSetup();
	for (iterator = 0 ; iterator <3 ; iterator++)
	{
		for (counter = 0 ; counter <31 ; counter++)
		{
			CRC_Tx = (CRC_Tx) ^ (Large_array[iterator][counter]) ^
					  	  	    (Large_array[iterator][counter+1]);
		}
	}

	while(1)
	{

//		EF_void_UART_HextoASCII ((U8_t*)&CRC_Tx);
		counter = 0;
		for (iterator = 0 ; iterator <3 ; iterator++)
		{
			EF_BOOLEAN_nRF_SendData(&(Large_array[iterator][0]), 32 );
			_delay_ms(100);
			PORTD ^= 1<<7;
		}
//		_delay_ms(1000);
//		_delay_ms(1000);
//		PORTD ^= 1<<7;
	}



	return 0;
}
#endif
