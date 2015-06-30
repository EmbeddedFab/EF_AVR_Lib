/***************************************************************
 *  Source File: EF_Wavecom.c
 *
 *  Description: This simple driver for Wavecom  Fastrack M1306B (GSM module)
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
#include "EF_Wavecom.h"
#include <string.h>
/**************************************************
 * Define
 *************************************************/
#define WAVECOM_BAUDRATE 			9600      /* the Module default:15200, and Atmega has a large
 	 	 	 	 	 	 	 	 	 	 	 	 error with 15200, so convert it by terminal to 9600*/
#define MAX_CMD_SIZE 				30        /* to define the size of CommandArray */
#define MAX_PACKET_SIZE				200       /* to define the max size of Array RX Packet Buffer */
#define MAX_DIGIT_FOR_ITOA			4         /* to define the max. number of Digit using in itoa function, until now using in SMS read and BaudRate */
#define MAX_DIGITS_Of_MOBILE_NUMBER 11		  /* using in check mobilre number function */
/**************************************************
 * Global Definitions
 *************************************************/
enum CommandArrayElements{
	AT = 0,                    			  /* Send AT CMD to Module */
	ECHO0,                 			      /* Disable CMD Echo */
	ECHO1,                   			  /* Enable CMD Echo */
	PIN_CHECK,              			  /* Check PIN Number for Sim_Card*/
	NO_FLOW_CONROL,           			  /* Disable UART Flow Control */
	REGISTRATION_CHECK,       			  /* Check Network Registration Status*/
	SET_BAUD_RATE,            			  /* Set Baud-rate 9600 , Problem with ATMEGA */
	ERROR_INDICATION, 		   			  /* Set Global Error Indication */
	ACTIVITY_STAUS_CHECK,	  			  /* check SIM is ready to receive, should receive: \r\n+CPAS: 0\r\nOK\r\n   */
	GENERAL_INDICATION,					  /* Enable GENERAL INDICATION */
	NEW_MESSAGE_INDICATION,				  /* New Message Indication, SMS-DELIVERs are directly stored, SMS-STATUS- REPORTs are displayed*/
	SET_MODE_PARAMETERS,				  /* set text mode parameters ,SMS-SUBMIT message with a validity period (one day)*/
	TEXT_FORMAT,						  /* set SMS text format to be as ASCII ,not PDU format*/
	SEND_SMS,							  /* Send SMS Command, must follow by: "number"\rSMS(0x1A(ctrl+z)) */
	READ_LIST_SMS,						  /* Read list of SMSs ,follow by: "ReadListSmsArray[one of types]"\r*/
	READ_SMS_BY_NUMBER,					  /* Read one SMS ,follow by: SMS Number*/
	INCOMING_CALL_NUMBER_INDICATION,	  /* Enable presenting Incoming call Number*/
	INCOMING_CALL_EXTENDED_FORMAT,		  /* Define More Indication for incoming call, such as: define type as (CRING: Voice)*/
	NO_INCOMING_CALL_NUMBER_INDICATION,	  /* Disable presenting Incoming call Number */
	NO_INCOMING_CALL_EXTENDED_FORMAT,	  /* Disable More Indication for incoming call*/
	CALL,								  /* CMD to call , should follow by number; */
	CALL_ANSWER,						  /* CMD to answer to the incoming call */
	CALL_RELEASE						  /* CMD to end/hang-up the call */
};
/*Command_Array has all needed CMDs ,should using as Command_Array[one of CommandArrayElements ENUM]*/
const char Command_Array [][MAX_CMD_SIZE] =
{ {"AT\r"},
  {"ATE0\r"},
  {"ATE1\r"},
  {"AT+CPIN?\r"},
  {"AT+IFC=0,0\r"},
  {"AT+CREG=1\r"},
  {"AT+IPR=9600\r"},  /*TODO : Enhance :using itoa_convert and make BaudRate variable in #define, then sent '\r'*/
  {"AT+CMEE=1\r"},
  {"AT+CPAS\r"},
  {"AT+WIND=63\r"},
  {"AT+CNMI=0,1,1,1,0\r"},
  {"AT+CSMP=17,169,0,0\r"},
  {"AT+CMGF=1\r"},
  {"AT+CMGS="},
  {"AT+CMGL="},
  {"AT+CMGR="},
  {"AT+CLIP=1\r"},
  {"AT+CRC=1\r"},
  {"AT+CLIP=0\r"},
  {"AT+CRC=0\r"},
  {"ATD"},
  {"ATA\r"},
  {"ATH\r"},
};

/* Using in READ_SMS , argument: "Wavecom_ReadSms_cfg_str.ReadListType" should define which read type Status */
const char ReadListSmsTypesArray[6][15] =
{ {"\"ALL\"\x0D"},
  {"\"REC READ\"\x0D"},
  {"\"REC UNREAD\"\x0D"},
  {"\"STO SENT\"\x0D"},
  {"\"STO UNSENT\"\x0D"}
};
/* to configure the uart */
UART_cfg_str  uart_cfg = {WAVECOM_BAUDRATE , 8, ONE_STOP_BIT, NO_PARITY, TRUE, FALSE, TRUE, TRUE};

extern volatile U8_t gSmsReadyFlag     ;   			 /* flag to define  ready SMS read in the next response */
extern volatile U16_t gNumberOfRxBytes ;			 /* save number of SMS bytes */

extern U8_t gRX_SMS           [MAX_SMS_SIZE] ;       /* Array which will include the Read SMS */
/***************************************************************************
 ************************* Local Functions *********************************
 ***************************************************************************/

/*********************************************************************
* Function    :  CheckMobileNumber (U8_t* MobileNumber_ptr, U8_t NumberLength)
*
* DESCRIPTION : This function used to check the mobile number is correct or not, check
* 				for the first number = 0 , second = 1 , (third = 0 or 1 or 2) and length=11
*
* PARAMETERS  : U16_t NumberLength : should be equalled MAX_DIGITS_Of_MOBILE_NUMBER
* 				U8_t * MobileNumber_ptr : pointer to the ASCII number
*
* Return Value: BOOLEAN to check for Errors
**********************************************************************/

BOOLEAN CheckMobileNumber (U8_t* MobileNumber_ptr, U8_t NumberLength)
{
	U8_t ArrayIndex = 0;
	if ((NumberLength == MAX_DIGITS_Of_MOBILE_NUMBER) && (MobileNumber_ptr[0] == '0')&& (MobileNumber_ptr[1] == '1') &&
		((MobileNumber_ptr[2] == '2')|| (MobileNumber_ptr[2] == '1') || (MobileNumber_ptr[2] == '0')))
	{
		for (ArrayIndex = 0 ; ArrayIndex <NumberLength; ArrayIndex++)
		{
			if ( (MobileNumber_ptr[ArrayIndex] < '0') ||  (MobileNumber_ptr[ArrayIndex] > '9') )
			{
				return 0;
			}
		}
		/*TODO: modify return */
		return 1;
	}
	else
	{
		return 0;
	}
}


/*********************************************************************
* Function    :  itoa_Convert (U16_t Number , U8_t * NumberASCII_ptr)
*
* DESCRIPTION : This function used to convert integer number to ASCII
*
* PARAMETERS  : U16_t Number : Number ,max. digits of it should be equalled  MAX_DIGIT_FOR_ITOA
* 				U8_t * NumberASCII_ptr [MAX_DIGIT_FOR_ITOA] : pointer to the ASCII number after Conversion
*
* Return Value: BOOLEAN to check for Errors
**********************************************************************/
/* Enter Number of MAX_DIGIT */
void itoa_Convert (U32_t Number , U8_t * NumberASCII_ptr)
{
	U8_t  ArrayIndex ;
	for ( ArrayIndex =0 ; ArrayIndex < MAX_DIGIT_FOR_ITOA ; ArrayIndex++)
	{
		NumberASCII_ptr[ArrayIndex] = Number %10 +'0';
		Number = Number/10;
	}
	/* rotate , using Number as a temp */
	for ( ArrayIndex =0 ; ArrayIndex< MAX_DIGIT_FOR_ITOA/2 ; ArrayIndex++)
	{
		Number     = NumberASCII_ptr[ArrayIndex];
		NumberASCII_ptr[ArrayIndex]   = NumberASCII_ptr[MAX_DIGIT_FOR_ITOA-ArrayIndex-1];
		NumberASCII_ptr[MAX_DIGIT_FOR_ITOA-ArrayIndex-1] = Number;
	}
}

/***********************************************************************
 ********************** Global API Functions****************************
 ***********************************************************************/

/*********************************************************************
* Function    : EF_BOOLEAN_Wavecom_InitModule (void);
*
* DESCRIPTION : This function used to initialise GSM Module:
* 					disable Echo,
* 					Enable General Indication,
* 					Enable General Error Indication,
					no flow control,
* 					check for PIN code ready,
* 					check if the module is ready to receive or not,
* 					check the network registration.
*
* PARAMETERS  : none.
*
* Return Value: BOOLEAN to check for Errors
**********************************************************************/
BOOLEAN EF_BOOLEAN_Wavecom_InitModule (void)
{
	EF_void_LCD_Clear_Screen();

	EF_void_UART_Init(&uart_cfg);

	EF_void_UART_SendArray((U8_t*)Command_Array[ECHO0],
			                strlen(Command_Array[ECHO0]));
	EF_void_UART_SendArray((U8_t*)Command_Array[GENERAL_INDICATION],
			                strlen(Command_Array[GENERAL_INDICATION]));
	EF_void_UART_SendArray((U8_t*)Command_Array[ERROR_INDICATION],
			                strlen(Command_Array[ERROR_INDICATION]));
	EF_void_UART_SendArray((U8_t*)Command_Array[NO_FLOW_CONROL],
			                strlen(Command_Array[NO_FLOW_CONROL]));

	/*TODO: Enhance all delays in all functions*/

	 _delay_ms(2000);

	EF_void_LCD_Clear_Screen();

	/*check for CPIN, should return:  \r\n+CPIN: READY\r\n  ,else need pin 0000 or error*/
	EF_void_UART_SendArray((U8_t*)Command_Array[PIN_CHECK],
			                strlen(Command_Array[PIN_CHECK]));

	_delay_ms(3000);

	EF_void_LCD_Clear_Screen();

	/*check for activity status , should return: +CPAS: 0 , means it is ready for receiving*/
	EF_void_UART_SendArray((U8_t*)Command_Array[ACTIVITY_STAUS_CHECK],
			                strlen(Command_Array[ACTIVITY_STAUS_CHECK]));
	_delay_ms(3000);

	EF_void_LCD_Clear_Screen();

	/*check for Network registration , should return: \r\nOK\r\n\r\n+CREG: 1\r\n*/
	EF_void_UART_SendArray((U8_t*)Command_Array[REGISTRATION_CHECK],
			                strlen(Command_Array[REGISTRATION_CHECK]));
	_delay_ms(3000);

	/*TODO enhance all return statuses  and modify the "Return Value" in all Function API Comments*/
	return 1;
}

/********************************************************************************
* Function    : EF_BOOLEAN_Wavecom_InitSms (Wavecom_Sms_cfg_str* Wavecom_Sms_cfg_ptr);
*
* DESCRIPTION : This function used to set the SMS Indication Commands
*
* PARAMETERS  : Wavecom_Sms_cfg_ptr is a pointer to Wavecom_Sms_cfg_str to
* 				configure the SMS Indication CMDs
*
* Return Value: BOOLEAN to check for Errors
********************************************************************************/
BOOLEAN EF_BOOLEAN_Wavecom_InitSms (Wavecom_Sms_cfg_str* Wavecom_Sms_cfg_ptr)
{

	if (Wavecom_Sms_cfg_ptr->NewMessageIndication == ENABLE)
	{

		EF_void_LCD_Clear_Screen();
		EF_void_UART_SendArray( (U8_t*)Command_Array[NEW_MESSAGE_INDICATION],
				                 strlen(Command_Array[NEW_MESSAGE_INDICATION]));
		_delay_ms(1000);
	}

	if (Wavecom_Sms_cfg_ptr->SetModeParameters == ENABLE)
	{
		EF_void_LCD_Clear_Screen();
		EF_void_UART_SendArray( (U8_t*)Command_Array[SET_MODE_PARAMETERS],
				                 strlen(Command_Array[SET_MODE_PARAMETERS]));
		_delay_ms(1000);
	}

	if (Wavecom_Sms_cfg_ptr->TextFormat == ENABLE)
	{
		EF_void_LCD_Clear_Screen();
		EF_void_UART_SendArray( (U8_t*)Command_Array[TEXT_FORMAT],
								strlen(Command_Array[TEXT_FORMAT]));
		_delay_ms(1000);
	}

	/*TODO enhance return status for all functions*/
	return 0;
}

/*********************************************************************
* Function    : EF_BOOLEAN_Wavecom_SendSms  (U8_t* MobileNumber_ptr, U8_t NumberLength,
* 										 U8_t* SendData_ptr, U16_t DataLength);
*
* DESCRIPTION : This function used to check number and Data_ptr then Send SMS
*
* PARAMETERS  : U8_t* MobileNumber_ptr: pointer to Mobile Number in "ASCII"
* 				U8_t  NumberLength    : has the length of Mobile Number
* 			    U8_t* SendData_ptr    : pointer to the data which you want to send as SMS
*				U8_t  DataLength      : length of the data which you want to send as SMS
*
* Return Value: BOOLEAN to check for Errors
**********************************************************************/
BOOLEAN EF_BOOLEAN_Wavecom_SendSms (U8_t* MobileNumber_ptr, U8_t NumberLength, U8_t* SendData_ptr, U16_t DataLength)
{
	volatile BOOLEAN b_MobileStatus ;

	b_MobileStatus = CheckMobileNumber((U8_t*)MobileNumber_ptr,  NumberLength) ;
	if ( (b_MobileStatus == ERROR) || (SendData_ptr == NULL) )
	{
		return ERROR;
	}
	EF_void_LCD_Clear_Screen();


	/*frame: [Command_Array[SEND_SMS]"number"\rMessage\x1A*/
	EF_void_UART_SendArray( (U8_t*)Command_Array[SEND_SMS],
			                 strlen(Command_Array[SEND_SMS]));
	EF_BOOLEAN_UART_PutChar('\"');
	EF_void_UART_SendArray( (U8_t*) MobileNumber_ptr ,
			                  NumberLength );
	EF_BOOLEAN_UART_PutChar('\"');
	EF_BOOLEAN_UART_PutChar('\r');
	EF_void_UART_SendArray( (U8_t*) SendData_ptr ,
			                  DataLength );
	EF_BOOLEAN_UART_PutChar('\x1A');

	_delay_ms(7000);

	/*TODO enhance return status for all functions*/
	return 1;

}


/*********************************************************************
* Function    : EF_S16_Wavecom_ReadSms (Wavecom_ReadSms_cfg_str* Wavecom_ReadSms_cfg_ptr);
*
* DESCRIPTION : This function used to Read SMS
*
* PARAMETERS  : Wavecom_ReadSms_cfg_ptr is pointer to Wavecom_ReadSms_cfg_str
* 				to define what types of SMS which you want to read
*
*
* Return Value: -1		 : Error
* 				 0		 : SMS is not found
* 				 <number>: SMS length
**********************************************************************/
S16_t 	EF_S16_Wavecom_ReadSms (Wavecom_ReadSms_cfg_str* Wavecom_ReadSms_cfg_ptr)
{
	U8_t ASCII_OfNumber[MAX_DIGIT_FOR_ITOA];
	if (Wavecom_ReadSms_cfg_ptr->b_ListType  == ENABLE)
	{
		/*frame : Command_Array[READ_LIST_SMS]"TYPE"\r  */
		EF_void_LCD_Clear_Screen();
		EF_void_UART_SendArray((U8_t*) Command_Array[READ_LIST_SMS],
								strlen(Command_Array[READ_LIST_SMS]));
		EF_void_UART_SendArray( (U8_t*) (ReadListSmsTypesArray[Wavecom_ReadSms_cfg_ptr->ReadListType]) ,
				strlen( (const char*)(ReadListSmsTypesArray[Wavecom_ReadSms_cfg_ptr->ReadListType]) ) );
		_delay_ms(7000);
		/*TODO: handle RX SMS and store and display and >>> SMS max NumberOfMessags*/
		return (*(Wavecom_ReadSms_cfg_ptr->NumberOfMessags));
	}
	else
	{
		gSmsReadyFlag = 1;

		/*frame : Command_Array[READ_SMS_BY_NUMBER]MessageNumber\r*/
		EF_void_LCD_Clear_Screen();
		EF_void_UART_SendArray((U8_t*) Command_Array[READ_SMS_BY_NUMBER],
								strlen(Command_Array[READ_SMS_BY_NUMBER]));
		 /*convert to ASCII*/
		itoa_Convert(Wavecom_ReadSms_cfg_ptr->MessageNumber , ASCII_OfNumber);
		EF_void_UART_SendArray((U8_t*) ASCII_OfNumber, MAX_DIGIT_FOR_ITOA);
		EF_BOOLEAN_UART_PutChar('\r');
		_delay_ms(7000);
      /*TODO : extract Data and return it's length*/

		EF_void_LCD_print_ByLength(gRX_SMS, gNumberOfRxBytes);
		_delay_ms(5000);
		gSmsReadyFlag = 0;

	}

	return 1;
}


/*********************************************************************
* Function    : EF_BOOLEAN_Wavecom_InitCall (Wavecom_Call_cfg_str* Wavecom_Call_cfg_ptr);
*
* DESCRIPTION : This function used to configure the indication for incoming Call
*
* PARAMETERS  : Wavecom_Call_cfg_ptr is pointer to Wavecom_Call_cfg_str to
* 				define which indication is wanted to be enabled
*
*
* Return Value: BOOLEAN to check for Errors
**********************************************************************/
BOOLEAN EF_BOOLEAN_Wavecom_InitCall (Wavecom_Call_cfg_str* Wavecom_Call_cfg_ptr)
{
	EF_void_LCD_Clear_Screen();

	if ( Wavecom_Call_cfg_ptr->IncomingNumberIndication == ENABLE)
	{
		EF_void_UART_SendArray( (U8_t*)Command_Array[INCOMING_CALL_NUMBER_INDICATION],
							strlen(Command_Array[INCOMING_CALL_NUMBER_INDICATION]));
	}
	else
	{
		EF_void_UART_SendArray( (U8_t*)Command_Array[NO_INCOMING_CALL_NUMBER_INDICATION],
							strlen(Command_Array[NO_INCOMING_CALL_NUMBER_INDICATION]));
	}
	if ( Wavecom_Call_cfg_ptr->ExtendedFormat == ENABLE)
	{
		EF_void_UART_SendArray( (U8_t*)Command_Array[INCOMING_CALL_EXTENDED_FORMAT],
							strlen(Command_Array[INCOMING_CALL_EXTENDED_FORMAT]));
	}
	else
	{
		EF_void_UART_SendArray( (U8_t*)Command_Array[NO_INCOMING_CALL_EXTENDED_FORMAT],
							strlen(Command_Array[NO_INCOMING_CALL_EXTENDED_FORMAT]));
	}
	_delay_ms(1000);
	/*TODO enhance return status*/
	return 0;
}



/*********************************************************************
* Function    :  EF_BOOLEAN_Wavecom_Call (U8_t* MobileNumber_ptr ,U8_t NumberLength)
*
* DESCRIPTION : This function used to check number then call
*
* PARAMETERS  : U8_t* MobileNumber_ptr: pointer to Mobile Number in "ASCII"
* 				U8_t  NumberLength    : has the length of Mobile Number
*
*
* Return Value: -1: Communication error
* 				 0: MobileNumber is wrong
* 				 1: OK
**********************************************************************/
S8_t EF_BOOLEAN_Wavecom_Call (U8_t* MobileNumber_ptr ,U8_t NumberLength)
{

	volatile BOOLEAN b_MobileStatus = 0;

	b_MobileStatus = CheckMobileNumber((U8_t*)MobileNumber_ptr,  NumberLength) ;
	if ( b_MobileStatus == ERROR)
	{
		return ERROR;
	}
	EF_void_LCD_Clear_Screen();
	/*frame: [Command_Array[CALL]number;\r  */
	EF_void_UART_SendArray((U8_t*) Command_Array[CALL],
						strlen(Command_Array[CALL]));
	EF_void_UART_SendArray((U8_t*) MobileNumber_ptr,
							 NumberLength);
	EF_BOOLEAN_UART_PutChar(';');
	EF_BOOLEAN_UART_PutChar('\r');
	_delay_ms(3000);
	/*TODO enhance return status*/
	return 1;
}


/*********************************************************************
* Function    : EF_BOOLEAN_Wavecom_CallAnswer (void);
*
* DESCRIPTION : This function used to answer to the incoming call.
*
* PARAMETERS  : none.
*
* Return Value: BOOLEAN to check for Errors
**********************************************************************/
BOOLEAN EF_BOOLEAN_Wavecom_CallAnswer (void)
{
	/*should return CMD will be  OK if answering for the incoming Call,
	 * else return CMD will be NOI CARRIER */
	EF_void_LCD_Clear_Screen();

	EF_void_UART_SendArray( (U8_t*)Command_Array[CALL_ANSWER],
							strlen(Command_Array[CALL_ANSWER]));

	_delay_ms(1000);
	return 1;
}

/*********************************************************************
* Function    : EF_BOOLEAN_Wavecom_CallRelease (void);
*
* DESCRIPTION : This function used to end the call.
*
* PARAMETERS  : none.
*
* Return Value: BOOLEAN to check for Errors
**********************************************************************/
BOOLEAN EF_BOOLEAN_Wavecom_CallRelease (void)
{
	/*should return OK then +WIND: 6,1 if releasing the incoming Call*/
	EF_void_LCD_Clear_Screen();

	EF_void_UART_SendArray( (U8_t*)Command_Array[CALL_RELEASE],
							strlen(Command_Array[CALL_RELEASE]));

	_delay_ms(1000);
	return 1;
}

