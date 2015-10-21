/***************************************************************
 *  Source File: EF_ESP8266.c
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
#include "EF_ESP8266.h"
#include "../MCAL/EF_UART.h"
#include "../MCAL/EF_SpechialTIMER.h"
#include "../MCAL/EF_DIO.h"
#include <string.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "EF_LCD.h"

/* ----------------------------- Global Definitions ----------------------------*/

U8_t ReturnedArray[SIZE_OF_RETURNED_ARRAY] ={0};  /* Rx UART Buffer */

U8_t Recevied_Data[30] = {0};                   /* using in EF_BOOLEAN_ESP8266_GetRxData function as small Rx buffer */

enum CommandArrayElements{
	AT = 0,                    			  /* Send AT CMD to Module */
	ECHO0,                 			      /* Disable CMD Echo */
	ECHO1,                   			  /* Enable CMD Echo */
	SET_BAUD_RATE,            			  /* Set Baud-rate 9600 , Problem with ATMEGA */
	AT_RST,
	WIFI_MODE,
	LIST_AVALIABLE_AP,
	JOIN_AP,
	DISCONNECT_FROM_AP,
	GET_VERSION,
	/* GPRS */
	MULTIPLE_IP_CONNECTION,
	OPEN_SERVER,
	GET_IP_ADD,							  /* Get Local IP Address */
	STARTUP_TCP_CONNECTION,				  /* Enter URL here */
	SEND_DATA_WITH_TCP,					  /* To Send Data through TCP ,follow it by GET, POST or.. (http commands) */
	SEND_DATA,
	CLOSE_TCP,
	SET_STATION_IP,
	SET_SERVER_TIMEOUT,
	CREATE_AP,
	MODE_AP_AND_ST                        /* mode acess point and station */
};

/*CommandArray has all needed CMDs ,should using as CommandArray[one of CommandArrayElements ENUM]*/
const char * CommandArray [25][25] =
{ {"AT\r\n"},
  {"ATE0\r\n"},
  {"ATE1\r\n"},
  {"AT+IPR=9600\r\n"},  /*TODO : Enhance :using itoa_convert and make BaudRate variable in #define, then sent '\r'*/
  {"AT+RST\r\n"},
  {"AT+CWMODE=1\r\n"},
  {"AT+CWLAP\r\n"},
  {"AT+CWJAP="},			/* follow by "SSID","password" */
  {"AT+CWQAP\r\n"},
  {"AT+GMR\r\n"},
  /* GPRS */
  {"AT+CIPMUX=1\r\n"},
  {"AT+CIPSERVER=1,80\r\n"},
  {"AT+CIFSR\r\n"},
  {"AT+CIPSTART=4,\"TCP\","},    /* follow by "IP",numberPort  */
  {"AT+CIPSEND\r\n"},			   /* follow by channelNumber,BytesLength */
  {"AT+CIPSEND=4,"},			 /* follow by data length + 2 for (/r/n)*/
  {"AT+CIPCLOSE\r\n"},
  {"AT+CIPSTA="}, 				/* followed by "IP" as "192.168.1.200" */
  {"AT+CIPSTO=7200\r\n"},		/* 7200 sec. */
  {"AT+CWSAP=\""},              /* followed by your created Acess point name */
  {"AT+CWMODE=3\r\n"},
};



/* ------------------------------------ Local Functions -----------------------------------------*/

/*********************************************************************
* Function    :  EF_BOOLEAN_ESP8266_GetCharArray
*
* DESCRIPTION : This function used to receive from UART array until defined given NewLine Number
*               or until number of definied tries or until reach to the Max Size of RX Buffer Array
*
* PARAMETERS  : UART_Number     : Number ,max. digits of it should be equalled  MAX_DIGIT_FOR_ITOA
* 				ReturnedArray   : pointer to the returned Array
* 				NewLine_Numbers :  Max New Line receive character to stop when reaching it.
*
* Return Value: BOOLEAN to check for Errors
**********************************************************************/
BOOLEAN EF_BOOLEAN_ESP8266_GetCharArray( U8_t * ReturnedArray, U16_t NewLine_Numbers )
{
    U8_t index = 0;             /* Iterator */
    U8_t ReturnStatus = 0;      /* Using in return */
    U8_t TempIndex=0;           /* count every iteration until reaching definied tries number then extract from function*/
    U8_t NewLine_Count = 0;     /* count until reaching defined given NewLine Number */
    U8_t ReceviedByte = 0;

    do
    {
//        _delay_us(6);
       EF_BOOLEAN_UART_GetChar(   &ReceviedByte );
       /* check if byte is received or not */
       if ( ReceviedByte != FALSE)
       {
            *(ReturnedArray + index) = ReceviedByte;

            if ( *(ReturnedArray + index) == '\n')
            {
                NewLine_Count++;
            }

            index++;
       }
       else
       {
           /* not found byte at Rx UART Buffer */
           TempIndex++;
       }

    }while ( (NewLine_Count < NewLine_Numbers)  && (index< SIZE_OF_RETURNED_ARRAY ) && (TempIndex < (SIZE_OF_RETURNED_ARRAY *2) ));

    *(ReturnedArray + index) = 0;

//	EF_void_LCD_Clear_Screen();
//	EF_void_LCD_goto (1 , 1);
//    EF_void_LCD_print((U8_t*)ReturnedArray);

    if (NewLine_Count == NewLine_Numbers)
    {
        ReturnStatus = TRUE;
    }
    else
    {
        ReturnStatus = FALSE;
    }

    return ReturnStatus;
}



/*********************************************************************
* Function    : EF_BOOLEAN_ESP8266_ReadUntilExpectedKeyword
*
* DESCRIPTION : This function used to receive Array by UART, ans stop until Expected
*               keyword received or when timeout occured
*
* PARAMETERS  : ExpectedKeyword : Expected array to stop when find it or when time out
*               ExpectedKeyword_Length : length of ExpectedKeyword Array.
*               TimeOut : mili seconds (Ex: if timeout 5000 means 5sec ), using to extract
*                         from this function if ExpectedArray was not found
*
* Return Value: TRUE  : means OK, found this Expected word in Receiving progress.
*               FALSE : means time is out and not found the ExpectedArray
*
* NOTE: initialize the timer and global interrupt before using this function.
**********************************************************************/
BOOLEAN EF_BOOLEAN_ESP8266_ReadUntilExpectedKeyword (U8_t* ExpectedKeyword, U8_t ExpectedKeyword_Length, U16_t TimeOut)
{
    U8_t index = 0 ;
    U8_t RxData[20] = {0};    /* array to receive uart in it and compare with Expected Array */
    /* using Timer to extract if reaching time out */
    EF_void_TimerCreate(ESP8266_FIND_KEYWORD_ID , TimeOut);

    EF_void_TimerStart(ESP8266_FIND_KEYWORD_ID);

    for (index =0 ; index < ExpectedKeyword_Length; index++)
    {
        EF_BOOLEAN_UART_GetChar(   &RxData[index] );
    }

    while (EF_BOOLEAN_TimerCheck( ESP8266_FIND_KEYWORD_ID ) == FALSE)
    {
        /* compare with every receiving array with ExpectedKeyword_Length characters then shift one byte
         * to compare with every Similar width array */
        for (index =0 ; index < ExpectedKeyword_Length; index++)
        {
            if (RxData[index] != ExpectedKeyword[index])
            {
                break;
            }
            if (index == ExpectedKeyword_Length -1)
            {
            	EF_void_LCD_Clear_Screen();
            	EF_void_LCD_goto (1 , 1);
            	EF_void_LCD_print((U8_t*) (U8_t *)"Found Keyword" );
            	EF_void_LCD_Clear_Screen();
            	EF_void_LCD_goto (1 , 1);
                EF_void_LCD_print((U8_t*) ExpectedKeyword );
                EF_void_TimerStop ( ESP8266_FIND_KEYWORD_ID );
                EF_void_TimerReset ( ESP8266_FIND_KEYWORD_ID );
                return TRUE;
            }
        }

        /* shift one byte and receive new byte then compare */
        for (index =0 ; index < ExpectedKeyword_Length-1; index++)
        {
            RxData[index] = RxData[index+1];
        }

        EF_BOOLEAN_UART_GetChar(   &RxData[ExpectedKeyword_Length-1] );
    }

	EF_void_LCD_Clear_Screen();
	EF_void_LCD_goto (1 , 1);
    EF_void_LCD_print( (U8_t*)"Time Out");


    EF_void_TimerStop(ESP8266_FIND_KEYWORD_ID);
    EF_void_TimerReset(ESP8266_FIND_KEYWORD_ID);

    return FALSE;
}



/*********************************************************************
* Function    :  itoa_Convert (U16_t Number , U8_t * NumberASCII_ptr)
*
* DESCRIPTION : This function used to convert integer number to ASCII
*
* PARAMETERS  : U16_t Number : integer Number
*               U8_t * NumberASCII_ptr : pointer to the ASCII number after Conversion
*               NumberOFDigits_ptr: pointer to size NumberASCII_ptr array
*
* Return Value: None.
**********************************************************************/
static void itoa_Convert (U32_t Number , U8_t * NumberASCII_ptr ,U8_t* NumberOFDigits_ptr)
{
	U8_t  ArrayIndex = 0;
	U8_t NumberOfDigits = 0;
	U32_t Number_temp = Number;
	while ( Number == 0 )
	{
		NumberASCII_ptr[ArrayIndex] = '0';
		return;
	}
	/* get Number of digits of the wanted  number to be converted */
	while ( Number_temp > 0 )
	{
		Number_temp = Number_temp/10;
		NumberOfDigits++;
	}
	*NumberOFDigits_ptr = NumberOfDigits;

	for ( ArrayIndex =0 ; ArrayIndex < NumberOfDigits ; ArrayIndex++)
	{
		NumberASCII_ptr[ArrayIndex] = Number %10 +'0';
		Number = Number/10;
	}
	/* rotate , using Number as a temp */
	for ( ArrayIndex =0 ; ArrayIndex < NumberOfDigits/2 ; ArrayIndex++)
	{
		Number     = NumberASCII_ptr[ArrayIndex];
		NumberASCII_ptr[ArrayIndex]   = NumberASCII_ptr[NumberOfDigits-ArrayIndex-1];
		NumberASCII_ptr[NumberOfDigits-ArrayIndex-1] = Number;
	}
}

/*-----------------------------------------------------------------------------------------------------*/
/* -------------------------------------- API Functions -----------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------*/


/*********************************************************************
* Function    : EF_BOOLEAN_ESP8266_InitModule (void);
*
* DESCRIPTION : This function used to initialise ESP8266 Wifi, init UART
*               and Timer , Send some Init Commands
*
* PARAMETERS  : none.
*
* Return Value: BOOLEAN to check for Errors
**********************************************************************/
BOOLEAN EF_BOOLEAN_ESP8266_InitModule (void)
{
    U8_t ReturnStatus = 0;

    /* using timer and global interrupt to unstuck , to define TimeOut */
    EF_void_TimerInit();

    EF_void_LCD_init();

    EF_void_LCD_Clear_Screen();
#if USING_ISR == 0
    /* choose which UART Number to work with */
    UART_cfg_str  uart_cfg = {115200 , 8, ONE_STOP_BIT, NO_PARITY, FALSE, FALSE, TRUE, TRUE};

#elif USING_ISR == 1
    UART_cfg_str  uart_cfg = {115200 , 8, ONE_STOP_BIT, NO_PARITY, TRUE, FALSE, TRUE, TRUE};

#endif

    EF_void_UART_Init (&uart_cfg);


    /************************** Reset Command ******************************/
    EF_void_UART_SendArray(  (U8_t*)CommandArray[AT_RST],
            strlen( (const char *) CommandArray[AT_RST]));
//    _delay_ms(1000);

#if USING_ISR == 0
    /* should receive: many words such as "wdt reset" */
    ReturnStatus = EF_BOOLEAN_ESP8266_ReadUntilExpectedKeyword ((U8_t*)"wdt reset", strlen( (const char *) ("wdt reset")),10000);

    if (ReturnStatus == FALSE)
    {
    	EF_void_LCD_Clear_Screen();
    	EF_void_LCD_goto (1 , 1);
    	EF_void_LCD_print((U8_t*)(U8_t*)"Error: in Init Function at Reset Command");
        return FALSE;
    }
#if 0
    /* anther method to receive , reset the array before receiving */
    memset (ReturnedArray, 0 ,SIZE_OF_RETURNED_ARRAY);
    EF_BOOLEAN_ESP8266_GetCharArray (  ReturnedArray , 29 );
#endif

#endif
    _delay_ms(5000);

    /* to clear UART Buffer */
    EF_void_UART_Init (&uart_cfg);

    /************************** Change Mode (station Mode) Command ******************/
    EF_void_UART_SendArray(   (U8_t*)CommandArray[WIFI_MODE],
                            strlen( (const char *) (CommandArray[WIFI_MODE])));

#if USING_ISR == 0

    ReturnStatus = EF_BOOLEAN_ESP8266_ReadUntilExpectedKeyword ((U8_t*)"OK", strlen( (const char *) ("OK")),1000);

    if (ReturnStatus == FALSE)
    {
    	EF_void_LCD_Clear_Screen();
    	EF_void_LCD_goto (1 , 1);
        EF_void_LCD_print((U8_t*)(U8_t*)"Error: in Init Function at change Mode Command ");
        return FALSE;
    }
#if 0
    memset (ReturnedArray, 0 ,SIZE_OF_RETURNED_ARRAY);
    EF_BOOLEAN_ESP8266_GetCharArray (  ReturnedArray ,3);
#endif

#endif
    _delay_ms(1000);

    /************************** Multi Connection Command ******************************/
    EF_void_UART_SendArray(   (U8_t*)CommandArray[MULTIPLE_IP_CONNECTION],
            strlen( (const char *) (CommandArray[MULTIPLE_IP_CONNECTION])));

#if USING_ISR == 0

    ReturnStatus = EF_BOOLEAN_ESP8266_ReadUntilExpectedKeyword ((U8_t*)"OK", strlen( (const char *) ("OK")),1000);

    if (ReturnStatus == FALSE)
    {
    	EF_void_LCD_Clear_Screen();
    	EF_void_LCD_goto (1 , 1);
        EF_void_LCD_print((U8_t*)(U8_t*)"Error: in Init Function at Multi-Connection Command");
        return FALSE;
    }
#if 0
    memset (ReturnedArray, 0 ,SIZE_OF_RETURNED_ARRAY);
    EF_BOOLEAN_ESP8266_GetCharArray (  ReturnedArray ,3);
#endif

#endif
    _delay_ms(1000);

    /************************** Join to Network Command ******************************/
//    ReturnStatus = EF_BOOLEAN_ESP8266_JoinAP ("EmbeddedFab",strlen( (const char *)  ("EmbeddedFab"), "Embeddedfab55", strlen( (const char *) ("Embeddedfab55"));
//    if (ReturnStatus == FALSE)
//    {
//    		EF_void_LCD_Clear_Screen();
//    	  EF_void_LCD_goto (1 , 1);
//        EF_void_LCD_print((U8_t*)(U8_t*)"Error in Init Function at join Network Command");
//        return FALSE;
//    }
    //  EF_void_LCD_Clear_Screen();

    /************************** Get Version Command ******************************/
    EF_void_UART_SendArray(   (U8_t*)CommandArray[GET_VERSION],
            strlen( (const char *) (CommandArray[GET_VERSION])));
#if USING_ISR == 0

    ReturnStatus = EF_BOOLEAN_ESP8266_ReadUntilExpectedKeyword ((U8_t*)"OK", strlen( (const char *) ("OK")),2000);
    if (ReturnStatus == FALSE)
    {
    	EF_void_LCD_Clear_Screen();
    	EF_void_LCD_goto (1 , 1);
        EF_void_LCD_print((U8_t*)(U8_t*)"Error in Init Function at Get version Command");
        return FALSE;
    }
#if 0
    memset (ReturnedArray, 0 ,SIZE_OF_RETURNED_ARRAY);
    EF_BOOLEAN_ESP8266_GetCharArray (  ReturnedArray ,5);
#endif
#endif
    _delay_ms(2000);

    //  EF_void_LCD_Clear_Screen();

    /************************** Get IP Command ******************************/
    EF_void_UART_SendArray(   (U8_t*)CommandArray[GET_IP_ADD],
            strlen( (const char *) (CommandArray[GET_IP_ADD])));
#if USING_ISR == 0
    /* using this method to print WIFI module IP */
    memset (ReturnedArray, 0 ,SIZE_OF_RETURNED_ARRAY);
    EF_BOOLEAN_ESP8266_GetCharArray (  ReturnedArray ,5);

#endif
    _delay_ms(1000);

    return TRUE;

}



/*********************************************************************
* Function    : EF_BOOLEAN_ESP8266_JoinAP
*
* DESCRIPTION : This function used to connect to Given Access Point
*
* PARAMETERS  : AcessPoint: pointer to Acess Point name
*               AcessPoint_Length: length of AcessPoint array
*               Password: pointer to Password array
*               Password_Length: password array length
*
* Return Value: return TRUE if ok or FALSE if not Expected Answer
**********************************************************************/
BOOLEAN EF_BOOLEAN_ESP8266_JoinAP (U8_t* AcessPoint, U8_t AcessPoint_Length, U8_t* Password, U8_t Password_Length)
{
//    EF_void_LCD_Clear_Screen();
    U8_t Status = 0;
    U8_t ReturnStatus = 0;

    /**************** Found Avaliable Access Point Command ***********************/
    EF_void_UART_SendArray(   (U8_t*)CommandArray[LIST_AVALIABLE_AP],
                             strlen( (const char *) (CommandArray[LIST_AVALIABLE_AP])));

#if USING_ISR == 0
    // varaible 5 -7 ...
//    _delay_ms(100);
    memset (ReturnedArray, 0 ,SIZE_OF_RETURNED_ARRAY);
    EF_BOOLEAN_ESP8266_GetCharArray (  ReturnedArray ,5);
#endif

    _delay_ms(2000);

    /* init UART again , Just to clear UART Buffer */
#if USING_ISR == 0
    UART_cfg_str  uart_cfg = {115200 , 8, ONE_STOP_BIT, NO_PARITY, FALSE, FALSE, TRUE, TRUE};

#elif USING_ISR == 1
    UART_cfg_str  uart_cfg = {115200 , 8, ONE_STOP_BIT, NO_PARITY, TRUE, FALSE, TRUE, TRUE};
#endif

    EF_void_UART_Init (&uart_cfg);
//    EF_void_LCD_Clear_Screen();
    /**************** Join to defined Access Point Command ***********************/

    EF_void_UART_SendArray(   (U8_t*)CommandArray[JOIN_AP],
                             strlen( (const char *) (CommandArray[JOIN_AP])));

    EF_BOOLEAN_UART_PutChar(   '\"');
    EF_void_UART_SendArray(   AcessPoint , AcessPoint_Length);
    EF_void_UART_SendArray(  (U8_t*)"\",\"",3);
    EF_void_UART_SendArray(   Password , Password_Length);
    EF_void_UART_SendArray(   (U8_t*) "\"\r\n",3);

    _delay_ms(2000);

#if USING_ISR == 0

#if 0
    memset (ReturnedArray, 0 ,SIZE_OF_RETURNED_ARRAY);
    EF_BOOLEAN_ESP8266_GetCharArray (  ReturnedArray , 3 );
#endif
    ReturnStatus = EF_BOOLEAN_ESP8266_ReadUntilExpectedKeyword ((U8_t*)"OK", strlen( (const char *) ("OK")),10000);

    //    if ( ReturnedArray [AcessPoint_Length -1 + Password_Length -1 + strlen( (const char *) (CommandArray[JOIN_AP]) -1 + 7 + 6] == 'O')
        if (ReturnStatus == TRUE)
        {
            Status = TRUE;
        }
        else
        {
            Status = FALSE;
            EF_void_LCD_Clear_Screen();
            EF_void_LCD_goto (1 , 1);
            EF_void_LCD_print((U8_t*)"Fail to Connect to Your Access Point!");
        }
#endif


//    _delay_ms(1000);

    return Status;
}




/*********************************************************************
* Function    : EF_void_ESP8266_SetIp
*
* DESCRIPTION : This function used to Set Ip for Wifi Module
*
* PARAMETERS  : IP_ptr: pointer to IP
*               IP_Length: IP Length
*
* Return Value: BOOLEAN to check for Errors
**********************************************************************/
BOOLEAN EF_BOOLEAN_ESP8266_SetIp (U8_t* IP_ptr, U8_t IP_Length)
{
    /* IP as : 192.168.4.1  , set the three numbers as the Access point which you want to connect to */
//	EF_void_LCD_Clear_Screen();
    U8_t ReturnStatus = 0;
	EF_void_UART_SendArray(    (U8_t*)CommandArray[SET_STATION_IP],
			                 strlen( (const char *) (CommandArray[SET_STATION_IP])));


	EF_BOOLEAN_UART_PutChar(   '\"');
	EF_void_UART_SendArray(    IP_ptr , IP_Length);
	EF_void_UART_SendArray(   (U8_t*) "\"\r\n",3);

#if USING_ISR == 0

    ReturnStatus = EF_BOOLEAN_ESP8266_ReadUntilExpectedKeyword ((U8_t*)"OK", strlen( (const char *) ("OK")),1000);
    if (ReturnStatus == FALSE)
    {
    	EF_void_LCD_Clear_Screen();
    	EF_void_LCD_goto (1 , 1);
        EF_void_LCD_print((U8_t*)"Error: in SetIp Function at Set Ip Command");
        return FALSE;
    }


#endif

	_delay_ms(1000);

	return TRUE;
}




/*********************************************************************
* Function    : EF_BOOLEAN_ESP8266_OpenServer
*
* DESCRIPTION : This function used to make Wifi Module Opening Server
*
* PARAMETERS  : none.
*
* Return Value: BOOLEAN to check for Errors
**********************************************************************/
BOOLEAN EF_BOOLEAN_ESP8266_OpenServer ( void )
{
    U8_t ReturnStatus = 0;
//	EF_void_LCD_Clear_Screen();

    /********************* Multi-Connection Command **********************/
    EF_void_UART_SendArray(   (U8_t*)CommandArray[MULTIPLE_IP_CONNECTION],
                            strlen( (const char *) (CommandArray[MULTIPLE_IP_CONNECTION])));

#if USING_ISR == 0

    EF_BOOLEAN_ESP8266_ReadUntilExpectedKeyword ((U8_t*)"OK", strlen( (const char *) ("OK")),1000);

#if 0
    memset (ReturnedArray, 0 ,SIZE_OF_RETURNED_ARRAY);
    EF_BOOLEAN_ESP8266_GetCharArray (  ReturnedArray ,3);
#endif

#endif
    _delay_ms(1000);


    /************************** Open Server Command ******************************/
	EF_void_UART_SendArray(    (U8_t*)CommandArray[OPEN_SERVER],
			                 strlen( (const char *) (CommandArray[OPEN_SERVER])));
#if USING_ISR == 0

    ReturnStatus = EF_BOOLEAN_ESP8266_ReadUntilExpectedKeyword ((U8_t*)"OK", strlen( (const char *) ("OK")),1000);
    if (ReturnStatus == FALSE)
    {
    	EF_void_LCD_Clear_Screen();
    	EF_void_LCD_goto (1 , 1);
        EF_void_LCD_print((U8_t*)"Error: in OpenServer Function at enable open Server Command");
        return FALSE;
    }

#if 0
    memset (ReturnedArray, 0 ,SIZE_OF_RETURNED_ARRAY);
    EF_BOOLEAN_ESP8266_GetCharArray (  ReturnedArray ,4);
#endif

#endif
	_delay_ms(1000);

    /************************** Define SendData TimeOut Command ***********************/
    EF_void_UART_SendArray(   (U8_t*) CommandArray[SET_SERVER_TIMEOUT],
                            strlen( (const char *) (CommandArray[SET_SERVER_TIMEOUT])));

#if USING_ISR == 0

    ReturnStatus = EF_BOOLEAN_ESP8266_ReadUntilExpectedKeyword ((U8_t*)"OK", strlen( (const char *) ("OK")),1000);
    if (ReturnStatus == FALSE)
    {
    	EF_void_LCD_Clear_Screen();
    	EF_void_LCD_goto (1 , 1);
        EF_void_LCD_print((U8_t*)"Error: in OpenServer Function at set Server Time Command");

        return FALSE;
    }

#if 0
    memset (ReturnedArray, 0 ,SIZE_OF_RETURNED_ARRAY);
    EF_BOOLEAN_ESP8266_GetCharArray (  ReturnedArray ,3);
#endif

#endif
    _delay_ms(1000);


    return TRUE;
}


/*********************************************************************
* Function    : EF_BOOLEAN_ESP8266_CreateAcessPoint
*
* DESCRIPTION : This function used to Set Ip for Wifi Module
*
* PARAMETERS  : AP_ptr    : pointer to AP name which is wanted to create
*               AP_Length : AP Length
*
* Return Value: BOOLEAN to check for Errors
**********************************************************************/
BOOLEAN EF_BOOLEAN_ESP8266_CreateAcessPoint (U8_t* AP_ptr, U8_t AP_Length)
{
    /* create open acess point only ... without Password , except this AP cant be created ! */

     U8_t ReturnStatus = 0;
     /************************** select Mode Command ***********************/

    EF_void_UART_SendArray(    (U8_t*)CommandArray[MODE_AP_AND_ST],
                             strlen( (const char *) (CommandArray[MODE_AP_AND_ST])));

#if USING_ISR == 0

    ReturnStatus = EF_BOOLEAN_ESP8266_ReadUntilExpectedKeyword ((U8_t*)"OK", strlen( (const char *) ("OK")),1000);
    if (ReturnStatus == FALSE)
    {
    	EF_void_LCD_Clear_Screen();
    	EF_void_LCD_goto (1 , 1);
        EF_void_LCD_print((U8_t*)"Error: in CreateAceessPoint Function at select Mode Command");
        return FALSE;
    }
#if 0
    memset (ReturnedArray, 0 ,SIZE_OF_RETURNED_ARRAY);
    EF_BOOLEAN_ESP8266_GetCharArray (  ReturnedArray ,3);
#endif

#endif



    /************************** Create Access Point Command ***********************/
    EF_void_UART_SendArray(    (U8_t*)CommandArray[CREATE_AP],
                             strlen( (const char *) (CommandArray[CREATE_AP])));


    EF_void_UART_SendArray(    AP_ptr , AP_Length);
    EF_void_UART_SendArray(   (U8_t*) "\",\"\",1,0\r\n", strlen( (const char *) ("\",\"\",1,0\r\n") ));

#if USING_ISR == 0
    _delay_ms(1000);

    ReturnStatus = EF_BOOLEAN_ESP8266_ReadUntilExpectedKeyword ((U8_t*)"OK", strlen( (const char *) ("OK")),10000);
    if (ReturnStatus == FALSE)
    {
    	EF_void_LCD_Clear_Screen();
    	EF_void_LCD_goto (1 , 1);
        EF_void_LCD_print((U8_t*)"Error: in CreateAceessPoint Function at Create Access Point Command");
        return FALSE;
    }
#if 0
    memset (ReturnedArray, 0 ,SIZE_OF_RETURNED_ARRAY);
    EF_BOOLEAN_ESP8266_GetCharArray (  ReturnedArray ,3);
#endif

#endif

    _delay_ms(1000);

    return TRUE;

}



/*********************************************************************
* Function    : EF_BOOLEAN_ESP8266_SendToServer
*
* DESCRIPTION : Send data to Server , (to Server Ip), and if errors repeat all
*               steps for number of attemps = MAX_ATTEMPS
*
* PARAMETERS  : IP_ptr : Tx IP
*               IP_Length : length of Ip array
*               Data_ptr : pointer to Data wanted to send
*               DataLength : Data Length
*
* Return Value: BOOLEAN to check for Errors
**********************************************************************/
BOOLEAN EF_BOOLEAN_ESP8266_SendToServer (U8_t* IP_ptr, U8_t IP_Length, U8_t* Data_ptr, U8_t DataLength )
{
	U8_t NumberASCII_ptr [4] = {0};            /* using in converting to ASCII */
	volatile U8_t AttempsNumber = 0 ;          /* using to repeat steps if error until reaching MAX_ATTEMPS */
	volatile U8_t Status = 0;                  /* Return Status */
	U8_t NumberOFDigits = 0;                   /* Number of Digits to help in converting */
	U8_t ExpectedWordStatus = 0;               /* Using after Start TCP Connection Command */
	U8_t SendDataStatus = 0;                   /* Using after Send Data Required Command */
	U8_t DataStatus = 0;                       /* Using after Start TCP  Comman*/
	volatile U8_t ExpectedWordTries = 0;       /* using to repeat Start TCP Connection if error occured */
//	EF_void_LCD_Clear_Screen();

#if USING_ISR == 0
do{
#endif

    /********************* Multi-Connection Command **********************/
	EF_void_UART_SendArray(   (U8_t*)CommandArray[MULTIPLE_IP_CONNECTION],
			                strlen( (const char *) (CommandArray[MULTIPLE_IP_CONNECTION])));

#if USING_ISR == 0

    EF_BOOLEAN_ESP8266_ReadUntilExpectedKeyword ((U8_t*)"OK", strlen( (const char *) ("OK")),50);

#if 0
    memset (ReturnedArray, 0 ,SIZE_OF_RETURNED_ARRAY);
    EF_BOOLEAN_ESP8266_GetCharArray (  ReturnedArray ,3);
#endif

#endif
	_delay_ms(500);

#if 0
	do{
#endif
//	EF_void_LCD_Clear_Screen();

    /********************* Start TCP Connection Command **********************/
	/* like : AT+CIPSTART="tcp","192.168.1.100","80"   , should return  OK  CONNECT OK */
	EF_void_UART_SendArray(   (U8_t*) CommandArray[STARTUP_TCP_CONNECTION],
							strlen( (const char *) (CommandArray[STARTUP_TCP_CONNECTION])));
	EF_BOOLEAN_UART_PutChar(  '\"');
	EF_void_UART_SendArray(     IP_ptr, IP_Length);
	EF_BOOLEAN_UART_PutChar(  '\"');
	EF_void_UART_SendArray(    (U8_t*) ",80\r\n", 5);

#if USING_ISR == 0

    ExpectedWordStatus = EF_BOOLEAN_ESP8266_ReadUntilExpectedKeyword ((U8_t*)"CONNECT", strlen( (const char *) ("CONNECT")), 10000);

#if 0
    memset (ReturnedArray, 0 ,SIZE_OF_RETURNED_ARRAY);
    EF_BOOLEAN_ESP8266_GetCharArray (  ReturnedArray ,4);
#endif

#if 0
    ExpectedWordTries++;
    _delay_ms(1000);
    }while (ExpectedWordStatus != TRUE && ExpectedWordTries < MAX_ATTEMPS);
#endif



#endif

/*
//#if USING_ISR == 0
//    UART_cfg_str  uart_cfg = {  115200, NUMBER_OF_BITS_8, ONE_STOP_BIT,NO_PARITY,FALSE,FALSE,TRUE,TRUE};
//#elif USING_ISR == 1
//    UART_cfg_str  uart_cfg = {  115200, NUMBER_OF_BITS_8, ONE_STOP_BIT,NO_PARITY,TRUE,FALSE,TRUE,TRUE};
//#endif

//    EF_void_UART_Init (&uart_cfg);
*/
	_delay_ms(2000);

    /********************* Send Data Required Command **********************/

	EF_void_UART_SendArray(   (U8_t*)CommandArray[SEND_DATA],
				                strlen( (const char *) (CommandArray[SEND_DATA])));
	/* +2 for /r/n */

	itoa_Convert ( DataLength+2 , NumberASCII_ptr  , &NumberOFDigits);
	EF_void_UART_SendArray(   NumberASCII_ptr , NumberOFDigits);
	EF_void_UART_SendArray(   (U8_t*) "\r\n", 2);

#if USING_ISR == 0
	//    _delay_ms(1000);
	//3 when ok , and 4 when link is not valid

#if 0
    memset (ReturnedArray, 0 ,SIZE_OF_RETURNED_ARRAY);
    EF_BOOLEAN_ESP8266_GetCharArray (  ReturnedArray ,4);
#endif

    SendDataStatus = EF_BOOLEAN_ESP8266_ReadUntilExpectedKeyword ((U8_t*)"OK", strlen( (const char *) ("OK")),10000);
//    EF_void_UART_Init (&uart_cfg);
#endif

    _delay_ms(1000);

    /********************* Send Data Command **********************/
	EF_void_UART_SendArray(   Data_ptr , DataLength );

	EF_void_UART_SendArray(   (U8_t*) "\r\n", 2);

#if USING_ISR == 0

    _delay_ms(1);
    DataStatus = EF_BOOLEAN_ESP8266_ReadUntilExpectedKeyword ((U8_t*)"SEND OK", strlen( (const char *) ("SEND OK")),7000);
#if 0
    memset (ReturnedArray, 0 ,SIZE_OF_RETURNED_ARRAY);
    EF_BOOLEAN_ESP8266_GetCharArray (  ReturnedArray , 2);
#endif

#endif

    _delay_ms(3000);

#if USING_ISR == 0

    AttempsNumber++;

    }while ( ( (ExpectedWordStatus != TRUE) || (SendDataStatus != TRUE) || (DataStatus != TRUE) )  && (AttempsNumber < MAX_ATTEMPS)  );
//    }while (ReturnedArray[DataLength-1+9] != 'O' && (AttempsNumber <7) );

//    if ( (AttempsNumber == MAX_ATTEMPS) && ReturnedArray[DataLength-1+9] != 'O')
    if ( (AttempsNumber == MAX_ATTEMPS) && ( DataStatus == FALSE) )
    {
        Status = FALSE;
    	EF_void_LCD_Clear_Screen();
    	EF_void_LCD_goto (1 , 1);
        EF_void_LCD_print((U8_t*)"Unable to send To server");
    }
    else
    {
        Status = TRUE;
    }
#endif


	return Status;
}




/*********************************************************************
* Function    : EF_BOOLEAN_ESP8266_GetRxData
*
* DESCRIPTION : This function used to parcing to get the rx Data
*               and to print any data and to reinit if reset
*
* PARAMETERS  : UartBase
*               RxData_ptr: pointer to received data
*
* Return Value: 0 if byte is not received
*               1 if normal byte received
*               2 if data completed
*               3 if hardware reset  occured
**********************************************************************/
BOOLEAN EF_BOOLEAN_ESP8266_GetRxData (U32_t UartBase , U8_t* RxData_ptr)
{
     static U16_t index = 0;   /* Iterator */
     U8_t PrintIterator =0;                /* Iterator */
     U8_t ReturnStatus = 1;
     U8_t ResetStatus = 0;     /* check if reset occured  */
     volatile static U8_t MaybeData_Flag = 0 ;   /* check if Data is sent */
     volatile static U8_t ResetFlag =0 ;         /* inform reset is occured */

     /* check if Byte is received */
     if ( EF_BOOLEAN_UART_CheckForRxData( )== TRUE)
     {
         ReturnStatus = TRUE;
         /* if received "+I" , it may be data as data is "+IPD,x,N:Data\r\n  , N:DataLength"*/
         if (MaybeData_Flag == 1)
          {
               EF_BOOLEAN_UART_GetChar(  (Recevied_Data+index) );

              if ( (Recevied_Data[index] == '\r') || (index >= 30) )
              {
                  /* if iterator reached 30 or new line feed come , compare with action data */
                  /* receive order to toggle led */
                  if ( strncmp((const char* )&Recevied_Data[FIRST_DATA_ELEMENT], "LedT",index-FIRST_DATA_ELEMENT) == 0)
                  {
                      /* TODO : toggle any led to check */
                  }
                  Recevied_Data[index] = 0;
                  /*first element of data is 9*/
                  for (PrintIterator = FIRST_DATA_ELEMENT; PrintIterator < index; PrintIterator++)
                  {
//                      EF_void_LCD_send_data(Recevied_Data[PrintIterator] );
                  }
                  RxData_ptr = &Recevied_Data[FIRST_DATA_ELEMENT];
                  ReturnStatus = 2;
                  index = 0;
                  MaybeData_Flag = 0;
              }
              else
              {
                  index++;

              }
          }
         /* check if reset occured */
         else if (ResetFlag == 1)
         {
             /* if hardware reset , received::
              *    Ai-Thinker Technology Co.Ltd.
              *
              *    invalid
              *  */
             ResetStatus = EF_BOOLEAN_ESP8266_ReadUntilExpectedKeyword ((U8_t*)"invalid", strlen( (const char *) ("invalid")),10000);
             ResetFlag = 0;
             if ( ResetStatus == TRUE )
             {
                 /* h.w reset occured */
                 ReturnStatus = 3;
             }
         }
          else
          {
              /* receive One Byte in normal case and check for reset or Receiving Data */
        	  EF_BOOLEAN_UART_GetChar( (Recevied_Data+0));
              if ( Recevied_Data[0] == '+')
              {
            	  EF_BOOLEAN_UART_GetChar( (Recevied_Data+1) );
                  if ( (Recevied_Data[1] == 'I')  )
                  {
                      /* to enter the first if condition in the next entery to this function */
                      MaybeData_Flag = 1;
                      index = 2;
                  }
                  else
                  {
                	  EF_void_LCD_send_data('+' );
                	  EF_void_LCD_send_data( Recevied_Data[ 1 ] );
                  }

              }
              /* if hardware reset , received::
               *    Ai-Thinker Technology Co.Ltd.
               *
               *    invalid
               *  */
              else if ( Recevied_Data[0] == 'T')
              {
            	  EF_BOOLEAN_UART_GetChar( (Recevied_Data+1) );
                  if ( (Recevied_Data[1] == 'h')  )
                  {
                      /* to enter the second if condition in the next entery to this function to reset */

                      ResetFlag = 1;
                  }
                  else
                  {
                	  EF_void_LCD_send_data('T' );
                	  EF_void_LCD_send_data(Recevied_Data[1] );
                  }
              }
              else
              {
            	  EF_void_LCD_send_data(Recevied_Data[0] );
              }
          }
     }
     else
     {
         ReturnStatus = FALSE;
     }

     return ReturnStatus;

}
