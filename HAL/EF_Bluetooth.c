/***************************************************************
 *  Source File: EF_Bluetooth.c
 *
 *  Description: Bluetooth wifi driver
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
#include "../MCAL/EF_UART.h"
#include "../MCAL/EF_SpechialTIMER.h"
#include "../MCAL/EF_DIO.h"
#include <string.h>
#include "EF_Bluetooth.h"
#include "EF_Bluetooth_cfg.h"
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "EF_LCD.h"

/* ----------------------------- Global Definitions ----------------------------*/

/* connect pin 34 in module to 3.3v to Enable all AT Command
 * if Connection is Success , then connect it to zero to Communication mode */
#define ENABLE_AT_COMMAND_MODE       EF_S8_DIO_SetPin   (ENABLE_AT_COMMAND_PORT, ENABLE_AT_COMMAND_PIN)
#define ENABLE_COMMUNICATION_MODE    EF_S8_DIO_ClearPin (ENABLE_AT_COMMAND_PORT, ENABLE_AT_COMMAND_PIN)



enum CommandsArrayElements
{
    RESTORE_FACTORY_SETTING,
    GET_VERSION,
    GET_MODULE_MAC_ADD,                 /* get your Bluetooth Module MAC Add */
    SET_MODULE_NAME,                    /* get your Bluetooth Module Name */
    SET_MODULE_PASSWORD,                /* get your Bluetooth Module Name */
    CLEAR_PREVIOUSLY_PAIRED_DEVICES,
    SET_CONNECTION_TO_ANY_DEVICE,
    SET_MASTER,
    RESET_MODULE,                       /* if reset when pin43 is high ,baud rate will be 38400 */
    SET_SLAVE,
    GET_MODULE_STATUS,
    INQUIRE_ACCESS_MODE=0,
    INITALIZE_SPP_PROFILE,
    SCAN_DEVICES,
    GET_REMOTE_DEVICE_NAME,
    PAIRING,
    BINDING,
    SET_CONNECTION_TO_DEFINIED_ADD,
    CONNECT_TO_REMOTE_DEVICE,
    DISCONNECT
};

/*Command_Array has all needed CMDs ,should using as Command_Array[one of CommandArrayElements ENUM]*/
const char InitCommandsArray [15][15] =
{ {"AT+ORGL\r\n"},  /* followed by 0\r\n for slave or 1\r\n for Master */
  {"AT+VERSION\r\n"},
  {"AT+ADDR\r\n"},
  {"AT+NAME="},    /* followed by Module name then \r\n */
  {"AT+PSWD="},    /* followed by Module Password then \r\n */
  {"AT+RMAAD\r\n"},
  {"AT+CMODE=1\r\n"},
  {"AT+ROLE=1\r\n"},
  {"AT+RESET\r\n"},
  {"AT+ROLE=0\r\n"},
  {"AT+STATE?\r\n"}
};


const char CommuncationCommandsArray [15][15] =
{ {"AT+INQM=0,"},         /* followed by DevicesNumber,TimeOut\r\n */
  {"AT+INIT\r\n"},
  {"AT+INQ\r\n"},
  {"AT+RNAME?"},        /* followed by MAC_add\r\n */
  {"AT+PAIR="},         /* followed by MAC_add,TimeOut\r\n */
  {"AT+BIND="},         /* followed by MAC_add\r\n */
  {"AT+CMODE=0\r\n"},
  {"AT+LINK="},         /* followed by MAC_add\r\n */
  {"AT+DISC\r\n"}

};

U8_t Returned_Array[SIZE_OF_RETURNED_ARRAY] ={0};  /* Rx UART Buffer */

U8_t ReceviedData[30] = {0};                   /* using in EF_BOOLEAN_ESP8266_GetRxData function as small Rx buffer */

/* ------------------------------------ Local Functions -----------------------------------------*/


/*********************************************************************
* Function    :  EF_BOOLEAN_ESP8266_GetCharArray
*
* DESCRIPTION : This function used to receive from UART array until defined given NewLine Number
*               or until number of definied tries or until reach to the Max Size of RX Buffer Array
*
* PARAMETERS  : UART_Number     : Number ,max. digits of it should be equalled  MAX_DIGIT_FOR_ITOA
*               Returned_Array   : pointer to the returned Array
*               NewLine_Numbers :  Max New Line receive character to stop when reaching it.
*
* Return Value: BOOLEAN to check for Errors
**********************************************************************/
BOOLEAN EF_BOOLEAN_Bluetooth_GetCharArray( U8_t * Returned_Array, U16_t NewLine_Numbers )
{
    U16_t index = 0;             /* Iterator */
    U8_t ReturnStatus = 0;      /* Using in return */
    U16_t TempIndex=0;           /* count every iteration until reaching definied tries number then extract from function*/
    U16_t NewLine_Count = 0;     /* count until reaching defined given NewLine Number */
    U8_t ReceviedByte = 0;

    do
    {
//        _delay_us(6);
       EF_BOOLEAN_UART_GetChar( &ReceviedByte );
       /* check if byte is received or not */
       if ( ReceviedByte != FALSE)
       {
            *(Returned_Array + index) = ReceviedByte;

            if ( *(Returned_Array + index) == '\n')
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

    *(Returned_Array + index) = 0;

//    EF_void_LCD_print(( U8_t*) "%s\n",Returned_Array);

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
* Function    : EF_BOOLEAN_Bluetooth_ReadUntilExpectedKeyword
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
BOOLEAN EF_BOOLEAN_Bluetooth_ReadUntilExpectedKeyword (U8_t* ExpectedKeyword, U8_t ExpectedKeyword_Length, U16_t TimeOut)
{
    U8_t index = 0 ;
    U8_t RxData[20] = {0};    /* array to receive uart in it and compare with Expected Array */
    /* using Timer to extract if reaching time out */
    EF_void_TimerDelete(Bluetooth_FIND_KEYWORD_ID);

    EF_void_TimerCreate(Bluetooth_FIND_KEYWORD_ID , TimeOut);

    EF_void_TimerStart(Bluetooth_FIND_KEYWORD_ID);
    _delay_us(25);
    for (index =0 ; index < ExpectedKeyword_Length; index++)
    {
        EF_BOOLEAN_UART_GetChar(   &RxData[index] );
    }
    _delay_us(25);

    while (EF_BOOLEAN_TimerCheck( Bluetooth_FIND_KEYWORD_ID ) == FALSE)
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
                EF_void_LCD_print(( U8_t*)  (U8_t*)"Found Keyword");
                EF_void_LCD_goto (1 , 1);
                EF_void_LCD_print(( U8_t*) ExpectedKeyword);
                EF_void_TimerStop ( Bluetooth_FIND_KEYWORD_ID );
                EF_void_TimerReset ( Bluetooth_FIND_KEYWORD_ID );
                return TRUE;
            }
        }

        _delay_us(25);

        /* shift one byte and receive new byte then compare */
        for (index =0 ; index < ExpectedKeyword_Length-1; index++)
        {
            RxData[index] = RxData[index+1];
        }

        EF_BOOLEAN_UART_GetChar(   &RxData[ExpectedKeyword_Length-1] );
    }

	EF_void_LCD_Clear_Screen();
	EF_void_LCD_goto (1 , 1);
    EF_void_LCD_print(( U8_t*)  (U8_t*)"Time Out");


    EF_void_TimerStop(Bluetooth_FIND_KEYWORD_ID);
    EF_void_TimerReset(Bluetooth_FIND_KEYWORD_ID);

    return FALSE;
}



/*********************************************************************
* Function    :  itoa_Convert (U32_t Number , U8_t * NumberASCII_ptr ,U8_t* NumberOFDigits_ptr)
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
* Function    : EF_BOOLEAN_Bluetooth_ModuleInit ( BOOLEAN bMaster, U8_t* Password ,
*                                               U8_t* ModuleName , U8_t ModuleName_Length);
*
* DESCRIPTION : This function used to initialise UART, Enable AT Command Pin (make it 3.3v to at command
*               and 0 for Communication Mode) , Timer and Send some Init Commands
*
* PARAMETERS  : bMaster   : 1 to make Bluetooth Module to master and 0 to be slave
*               Password  : Set Bluetooth Module Password (4 Number characters (ASCII) like: "1234" )
*               ModuleName: Set Bluetooth Module Name
*
* Return Value: BOOLEAN to check for Errors
**********************************************************************/
BOOLEAN EF_BOOLEAN_Bluetooth_ModuleInit ( BOOLEAN bMaster, U8_t* Password , U8_t* ModuleName , U8_t ModuleName_Length)
{
    U8_t ReturnStatus = 0;

    EF_void_LCD_init();

    EF_void_LCD_Clear_Screen();

    /* using timer and global interrupt to unstuck , to define TimeOut */
    EF_void_TimerInit();
    /*
     * choose which UART Number to work with
     * Default Baud Rate is 38400 , you can change it ,but RESTORE_FACTORY_SETTING command return it to
     * the default baud rate
     *  */
    UART_cfg_str  uart_cfg = {38400 , 8, ONE_STOP_BIT, NO_PARITY, TRUE, FALSE, TRUE, TRUE};

    EF_void_UART_Init (&uart_cfg);

    /* connect pin 34 in module to 3.3v to Enable all AT Command
     * if Connection is Success , then connect it to zero to Communication mode */
    EF_B_DIO_InitPin ( ENABLE_AT_COMMAND_PORT, ENABLE_AT_COMMAND_PIN, OUTPUT);

    ENABLE_AT_COMMAND_MODE;
    _delay_ms(1000);

    /**************** RESTORE_FACTORY_SETTING Command *************/
    /* Restore to the following settings:  Device Mode: Slave  , UART: 38400bps, 8 bit, 1 stop bit, no parity
       Pairing Code: 1234 , Device Name: H-C-2010-06-01 */
    EF_void_UART_SendArray (   (U8_t*)InitCommandsArray[RESTORE_FACTORY_SETTING],
            strlen(InitCommandsArray[RESTORE_FACTORY_SETTING]));

    ReturnStatus = EF_BOOLEAN_Bluetooth_ReadUntilExpectedKeyword ((U8_t*)"OK", strlen("OK"),1000);

    if (ReturnStatus == FALSE)
    {
    	EF_void_LCD_Clear_Screen();
    	EF_void_LCD_goto (1 , 1);
        EF_void_LCD_print(( U8_t*)  (U8_t*)"Error: in Init Function at RESTORE_FACTORY_SETTING Command");
        return FALSE;
    }

    _delay_ms(1000);

    /**************** GET_VERSION Command *************/
    EF_void_UART_SendArray (   (U8_t*)InitCommandsArray[GET_VERSION],
            strlen(InitCommandsArray[GET_VERSION]));
//    _delay_ms(10);
    ReturnStatus = EF_BOOLEAN_Bluetooth_GetCharArray(   Returned_Array, 2 );

	EF_void_LCD_Clear_Screen();
	EF_void_LCD_goto (1 , 1);
    EF_void_LCD_print(( U8_t*) (U8_t*) Returned_Array);

    _delay_ms(1000);

    /**************** SET_MODULE_NAME Command *************/
    EF_void_UART_SendArray (   (U8_t*)InitCommandsArray[SET_MODULE_NAME],
            strlen(InitCommandsArray[SET_MODULE_NAME]));

    EF_void_UART_SendArray(   ModuleName , ModuleName_Length);
    EF_void_UART_SendArray(   (U8_t*) "\r\n",2);

    ReturnStatus = EF_BOOLEAN_Bluetooth_ReadUntilExpectedKeyword ((U8_t*)"OK", strlen("OK"),1000);

    if (ReturnStatus == FALSE)
    {
    	EF_void_LCD_Clear_Screen();
    	EF_void_LCD_goto (1 , 1);
        EF_void_LCD_print(( U8_t*) (U8_t*) "Error: in Init Function at SET_MODULE_NAME Command");
        return FALSE;
    }

    _delay_ms(1000);

    /**************** SET_MODULE_PASSWORD Command *************/
    EF_void_UART_SendArray (   (U8_t*)InitCommandsArray[SET_MODULE_PASSWORD],
            strlen(InitCommandsArray[SET_MODULE_PASSWORD]));
    /* followed by Module name then \r\n */

    EF_void_UART_SendArray(   Password , 4);
    EF_void_UART_SendArray(   (U8_t*) "\r\n",2);

    ReturnStatus = EF_BOOLEAN_Bluetooth_ReadUntilExpectedKeyword ((U8_t*)"OK", strlen("OK"),1000);

    if (ReturnStatus == FALSE)
    {
    	EF_void_LCD_Clear_Screen();
    	EF_void_LCD_goto (1 , 1);
        EF_void_LCD_print(( U8_t*) "Error: in Init Function at SET_MODULE_PASSWORD Command");
        return FALSE;
    }

    _delay_ms(1000);

    /**************** CLEAR_PREVIOUSLY_PAIRED_DEVICES Command *************/
    EF_void_UART_SendArray (   (U8_t*)InitCommandsArray[CLEAR_PREVIOUSLY_PAIRED_DEVICES],
            strlen(InitCommandsArray[CLEAR_PREVIOUSLY_PAIRED_DEVICES]));

    ReturnStatus = EF_BOOLEAN_Bluetooth_ReadUntilExpectedKeyword ((U8_t*)"OK", strlen("OK"),1000);

    if (ReturnStatus == FALSE)
    {
    	EF_void_LCD_Clear_Screen();
    	EF_void_LCD_goto (1 , 1);
        EF_void_LCD_print(( U8_t*) "Error: in Init Function at CLEAR_PREVIOUSLY_PAIRED_DEVICES Command");
        return FALSE;
    }

    _delay_ms(1000);

    /**************** SET_CONNECTION_TO_ANY_DEVICE Command *************/
    EF_void_UART_SendArray (   (U8_t*)InitCommandsArray[SET_CONNECTION_TO_ANY_DEVICE],
            strlen(InitCommandsArray[SET_CONNECTION_TO_ANY_DEVICE]));

    ReturnStatus = EF_BOOLEAN_Bluetooth_ReadUntilExpectedKeyword ((U8_t*)"OK", strlen("OK"),1000);

    if (ReturnStatus == FALSE)
    {
    	EF_void_LCD_Clear_Screen();
    	EF_void_LCD_goto (1 , 1);
        EF_void_LCD_print(( U8_t*) "Error: in Init Function at SET_CONNECTION_TO_ANY_DEVICE Command");
        return FALSE;
    }

    _delay_ms(1000);

    if (bMaster == TRUE)
    {
        /**************** SET_MASTER Command *************/
        EF_void_UART_SendArray (   (U8_t*)InitCommandsArray[SET_MASTER],
                strlen(InitCommandsArray[SET_MASTER]));

        ReturnStatus = EF_BOOLEAN_Bluetooth_ReadUntilExpectedKeyword ((U8_t*)"OK", strlen("OK"),1000);

        if (ReturnStatus == FALSE)
        {
        	EF_void_LCD_Clear_Screen();
        	EF_void_LCD_goto (1 , 1);
            EF_void_LCD_print(( U8_t*) "Error: in Init Function at SET_MASTER Command");
            return FALSE;
        }

        _delay_ms(1000);
        /**************** RESET_MODULE Command *************/

        /* after changing the role , you should reset the module */
        EF_void_UART_SendArray (   (U8_t*)InitCommandsArray[RESET_MODULE],
                strlen(InitCommandsArray[RESET_MODULE]));

        ReturnStatus = EF_BOOLEAN_Bluetooth_ReadUntilExpectedKeyword ((U8_t*)"OK", strlen("OK"),1000);

        if (ReturnStatus == FALSE)
        {
        	EF_void_LCD_Clear_Screen();
        	EF_void_LCD_goto (1 , 1);
            EF_void_LCD_print(( U8_t*) "Error: in Init Function at RESET_MODULE Command");
            return FALSE;
        }

        _delay_ms(1000);


    }
    else
    {
        /**************** SET_SLAVE Command *************/
        EF_void_UART_SendArray (   (U8_t*)InitCommandsArray[SET_SLAVE],
                strlen(InitCommandsArray[SET_SLAVE]));

        ReturnStatus = EF_BOOLEAN_Bluetooth_ReadUntilExpectedKeyword ((U8_t*)"OK", strlen("OK"),1000);

        if (ReturnStatus == FALSE)
        {
        	EF_void_LCD_Clear_Screen();
        	EF_void_LCD_goto (1 , 1);
            EF_void_LCD_print(( U8_t*) "\n*** Error: in Init Function at SET_SLAVE Command ***\n");
            return FALSE;
        }

        _delay_ms(1000);

        /**************** RESET_MODULE Command *************/
        /* after changing the role , you should reset the module */
         EF_void_UART_SendArray (   (U8_t*)InitCommandsArray[RESET_MODULE],
                 strlen(InitCommandsArray[RESET_MODULE]));

         ReturnStatus = EF_BOOLEAN_Bluetooth_ReadUntilExpectedKeyword ((U8_t*)"OK", strlen("OK"),1000);

         if (ReturnStatus == FALSE)
         {
         	EF_void_LCD_Clear_Screen();
         	EF_void_LCD_goto (1 , 1);
             EF_void_LCD_print(( U8_t*) "\n*** Error: in Init Function at RESET_MODULE Command ***\n");
             return FALSE;
         }

         _delay_ms(1000);

         /**************** INITALIZE_SPP_PROFILE Command *************/
       EF_void_UART_SendArray (   (U8_t*)CommuncationCommandsArray[INITALIZE_SPP_PROFILE],
                 strlen(CommuncationCommandsArray[INITALIZE_SPP_PROFILE]));
        /*may return error17 means : SPP already initialized */

         _delay_ms(1000);

         ENABLE_COMMUNICATION_MODE;
         _delay_ms(1000);

    }

    return TRUE;

}



/*********************************************************************
* Function    : EF_BOOLEAN_Bluetooth_MasterScan (U8_t ScanDevicesNumber ,
*                                               U8_t TimeOut, U8_t* ReturnedAddresses );
*
* DESCRIPTION : This function used to query/search for the nearby discoverable devices, print and
*               return the MAC Add. which is found.
*
* PARAMETERS  : ScanDevicesNumber: max number of devices to stop searching when reaching it.
*               TimeOut          : max time to stop searching when reaching it.
*               ReturnedAddresses: MAC Add. of existing devices, (you should give it pointer to
*                                  Large array Like 50 character (ex: 15* Wanted Devices Number))
*
* Return Value: BOOLEAN to check for Errors
**********************************************************************/
BOOLEAN EF_BOOLEAN_Bluetooth_MasterScan (U8_t ScanDevicesNumber , U8_t TimeOut, U8_t* ReturnedAddresses )
{
    U8_t ScanDevicesNumberASCII[2] = {0};       /* to convet ScanDevicesNumber to ascii and save in it */
    U8_t ScanDevicesNoOfDigits = 0;             /* to help in converting ScanDevicesNumber to ascii */
    U8_t TimeOutASCII[4] = {0};                 /* to convet TimeOut to ascii and save in it */
    U8_t TimeOutNoOfDigits = 0;                 /* to help in converting TimeOut to ascii */
//    U8_t Counter = 0 ;
    U8_t LoopIterator = 0;                      /* to make a loop to scan devices */
    U8_t ReturnStatus =0;
//    U8_t TempIterator = 0;

    itoa_Convert ( ScanDevicesNumber , ScanDevicesNumberASCII , &ScanDevicesNoOfDigits);

    itoa_Convert ( TimeOut , TimeOutASCII , &TimeOutNoOfDigits);

    ENABLE_AT_COMMAND_MODE;
    _delay_ms(1000);

    /**************** INQUIRE_ACCESS_MODE Command *************/
    EF_void_UART_SendArray (   (U8_t*)CommuncationCommandsArray[INQUIRE_ACCESS_MODE],
            strlen(CommuncationCommandsArray[INQUIRE_ACCESS_MODE]));

    EF_void_UART_SendArray(   ScanDevicesNumberASCII , ScanDevicesNoOfDigits);
    EF_BOOLEAN_UART_PutChar(   ',');
    EF_void_UART_SendArray(   TimeOutASCII , TimeOutNoOfDigits);
    EF_void_UART_SendArray(   (U8_t*) "\r\n",2);

    ReturnStatus = EF_BOOLEAN_Bluetooth_ReadUntilExpectedKeyword ((U8_t*)"OK", strlen("OK"),1000);

    if (ReturnStatus == FALSE)
    {
    	EF_void_LCD_Clear_Screen();
    	EF_void_LCD_goto (1 , 1);
        EF_void_LCD_print(( U8_t*) "\n*** Error: in EF_BOOLEAN_Bluetooth_MasterScan Function at INQUIRE_ACCESS_MODE Command ***\n");
        return FALSE;
    }

    _delay_ms(1000);

    /**************** INITALIZE_SPP_PROFILE Command *************/
    EF_void_UART_SendArray (   (U8_t*)CommuncationCommandsArray[INITALIZE_SPP_PROFILE],
            strlen(CommuncationCommandsArray[INITALIZE_SPP_PROFILE]));
    /* may return error17 means : SPP already initialized */

    _delay_ms(1000);

    /**************** SCAN_DEVICES Command *************/
    /* scan many times to take module chance to catch all avaliable devices */
    for (LoopIterator = 0 ; LoopIterator < ScanDevicesNumber; LoopIterator++)
    {

        EF_void_UART_SendArray (   (U8_t*)CommuncationCommandsArray[SCAN_DEVICES],
                strlen(CommuncationCommandsArray[SCAN_DEVICES]));

        _delay_ms(1000);

        //    EF_BOOLEAN_Bluetooth_GetCharArray( BLUETOOTH_UART_NUMBER , ReturnedAddresses , 25 );

        /* should receive +INQ:.....
         * ... = Number of avaliable MAC ADD. */


        ReturnStatus = EF_BOOLEAN_Bluetooth_ReadUntilExpectedKeyword ((U8_t*)"+INQ:", strlen("+INQ:"), TimeOut*1000 );

//        _delay_ms(1000);
        if (ReturnStatus == TRUE)
        {
            ReturnStatus = EF_BOOLEAN_Bluetooth_GetCharArray(   ReturnedAddresses, 10 );
        	EF_void_LCD_Clear_Screen();
        	EF_void_LCD_goto (1 , 1);
            EF_void_LCD_print(( U8_t*) "Exist MAC Addresses are :");
            EF_void_LCD_goto (2 , 1);
            EF_void_LCD_print(( U8_t*) ReturnedAddresses);

            /* the commented code is anther method but may not success */
//            TempIterator = TempIterator + 15;
//            for (Counter =0 ; Counter < 14; Counter++)
//            {
//                EF_BOOLEAN_UART_GetChar(  (ReturnedAddresses + Counter + TempIterator) );
//            }
//            ReturnedAddresses[TempIterator] = '\n';
        }
    }

//    ReturnedAddresses[ TempIterator + 1 ] = 0;

    /* print Array */
//	EF_void_LCD_Clear_Screen();
//	EF_void_LCD_goto (1 , 1);
//    EF_void_LCD_print(( U8_t*) "\n*Exist MAC Addresses are : %s\n", ReturnedAddresses);

    _delay_ms(1000);

    return TRUE;

}



/*********************************************************************
* Function    : EF_BOOLEAN_Bluetooth_MasterConnect (U8_t SlaveMAC_Add ,
*                                                  U8_t TimeOut,
*                                                  U8_t* ReturnSlaveName );
*
* DESCRIPTION : This function used to pair , connect to definied slave device with
*               it's MAC ADD , print and return Slave Device name.
*
* PARAMETERS  : SlaveMAC_Add   : 14 characters (12 of them is the MAC ADD of the wanted Slave
*                                device. give it as : 4char,2char,6char == "1CAF,05,D69CE9" )
*               TimeOut        : max time to enable Slave to enter Password and pairing
*               ReturnSlaveName:  return Slave Device name.
*
* Return Value: BOOLEAN to check for Errors
**********************************************************************/
BOOLEAN EF_BOOLEAN_Bluetooth_MasterConnect (U8_t * SlaveMAC_Add, U8_t TimeOut, U8_t* ReturnSlaveName )
{
    U8_t TimeOutASCII[4] = {0};     /* to convet ScanDevicesNumber to ascii and save in it */
    U8_t TimeOutNoOfDigits = 0;     /* to help in converting ScanDevicesNumber to ascii */
    U8_t ReturnStatus = 0 ;

    itoa_Convert ( TimeOut , TimeOutASCII , &TimeOutNoOfDigits);

    ENABLE_AT_COMMAND_MODE;
    _delay_ms(1000);

    /**************** GET_REMOTE_DEVICE_NAME Command *************/
    EF_void_UART_SendArray (   (U8_t*)CommuncationCommandsArray[GET_REMOTE_DEVICE_NAME],
            strlen(CommuncationCommandsArray[GET_REMOTE_DEVICE_NAME]));
    /* followed by MAC_add\r\n */

//    _delay_ms(1000);

    EF_void_UART_SendArray(   SlaveMAC_Add , 14 );
    EF_void_UART_SendArray(   (U8_t*) "\r\n",2);

//    ReturnStatus = EF_BOOLEAN_Bluetooth_GetCharArray(   ReturnSlaveName, 7 );

    ReturnStatus = EF_BOOLEAN_Bluetooth_ReadUntilExpectedKeyword ((U8_t*)"+RNAME:", strlen("+RNAME:"), 10000 );

    if (ReturnStatus == TRUE)
    {
        ReturnStatus = EF_BOOLEAN_Bluetooth_GetCharArray(   ReturnSlaveName, 3 );
    	EF_void_LCD_Clear_Screen();
    	EF_void_LCD_goto (1 , 1);
        EF_void_LCD_print(( U8_t*) "Slave name is :");
        EF_void_LCD_goto (2 , 1);
        EF_void_LCD_print(( U8_t*) ReturnSlaveName);
    }
    else
    {
    	EF_void_LCD_Clear_Screen();
    	EF_void_LCD_goto (1 , 1);
        EF_void_LCD_print(( U8_t*) "\n*** Error: in EF_BOOLEAN_Bluetooth_MasterConnect Function at GET_REMOTE_DEVICE_NAME Command ***\n");
        return FALSE;
    }
    _delay_ms(1000);


    /**************** PAIRING Command *************/
    /* the mobile should accept the request of paring and write the Password */
    EF_void_UART_SendArray (   (U8_t*)CommuncationCommandsArray[PAIRING],
            strlen(CommuncationCommandsArray[PAIRING]));


    EF_void_UART_SendArray(   SlaveMAC_Add , 14 );
    EF_BOOLEAN_UART_PutChar(   ',');
    EF_void_UART_SendArray(   TimeOutASCII , TimeOutNoOfDigits);
    EF_void_UART_SendArray(   (U8_t*) "\r\n",2);

//    _delay_ms(1000);

//    ReturnStatus = EF_BOOLEAN_Bluetooth_GetCharArray(   Returned_Array, 3 );
//	EF_void_LCD_Clear_Screen();
//	EF_void_LCD_goto (1 , 1);
//    EF_void_LCD_print(( U8_t*) "\n%s\n", Returned_Array);

    /* wait 12000 sec until receive OK to take Mobile chance to pair */
    ReturnStatus = EF_BOOLEAN_Bluetooth_ReadUntilExpectedKeyword ((U8_t*)"OK", strlen("OK"),12000);
//
    if ( ReturnStatus == FALSE )
    {
    	EF_void_LCD_Clear_Screen();
    	EF_void_LCD_goto (1 , 1);
        EF_void_LCD_print(( U8_t*) "\n*** Error: in EF_BOOLEAN_Bluetooth_MasterConnect Function at PAIRING Command ***\n");
        return FALSE;
    }
    _delay_ms(1000);


    /**************** BINDING Command *************/
    EF_void_UART_SendArray (   (U8_t*)CommuncationCommandsArray[BINDING],
            strlen(CommuncationCommandsArray[BINDING]));

    EF_void_UART_SendArray(   SlaveMAC_Add , 14 );
    EF_void_UART_SendArray(   (U8_t*) "\r\n",2);

//    ReturnStatus = EF_BOOLEAN_Bluetooth_GetCharArray(   Returned_Array, 2 );
//	EF_void_LCD_Clear_Screen();
//	EF_void_LCD_goto (1 , 1);
//    EF_void_LCD_print(( U8_t*) "\n%s\n", Returned_Array);

    ReturnStatus = EF_BOOLEAN_Bluetooth_ReadUntilExpectedKeyword ((U8_t*)"OK", strlen("OK"),1000);

    if (ReturnStatus == FALSE)
    {
    	EF_void_LCD_Clear_Screen();
    	EF_void_LCD_goto (1 , 1);
        EF_void_LCD_print(( U8_t*) "\n*** Error: in EF_BOOLEAN_Bluetooth_MasterConnect Function at BINDING Command ***\n");
        return FALSE;
    }
    _delay_ms(1000);

    /**************** SET_CONNECTION_TO_DEFINIED_ADD Command *************/
    EF_void_UART_SendArray (   (U8_t*)CommuncationCommandsArray[SET_CONNECTION_TO_DEFINIED_ADD],
            strlen(CommuncationCommandsArray[SET_CONNECTION_TO_DEFINIED_ADD]));

    ReturnStatus = EF_BOOLEAN_Bluetooth_ReadUntilExpectedKeyword ((U8_t*)"OK", strlen("OK"),1000);

    if (ReturnStatus == FALSE)
    {
    	EF_void_LCD_Clear_Screen();
    	EF_void_LCD_goto (1 , 1);
        EF_void_LCD_print(( U8_t*) "\n*** Error: in EF_BOOLEAN_Bluetooth_MasterConnect Function at SET_CONNECTION_TO_DEFINIED_ADD Command ***\n");
        return FALSE;
    }
    _delay_ms(1000);

    /**************** CONNECT_TO_REMOTE_DEVICE Command *************/
    EF_void_UART_SendArray (   (U8_t*)CommuncationCommandsArray[CONNECT_TO_REMOTE_DEVICE],
            strlen( CommuncationCommandsArray[CONNECT_TO_REMOTE_DEVICE]) );


    EF_void_UART_SendArray (   SlaveMAC_Add , 14 );
    EF_void_UART_SendArray (   (U8_t*) "\r\n",2);

//    _delay_ms(5000);

//    ReturnStatus = EF_BOOLEAN_Bluetooth_GetCharArray(   Returned_Array, 3 );
//	EF_void_LCD_Clear_Screen();
//	EF_void_LCD_goto (1 , 1);
//    EF_void_LCD_print(( U8_t*) "\n%s\n", Returned_Array);

    ReturnStatus = EF_BOOLEAN_Bluetooth_ReadUntilExpectedKeyword ((U8_t*)"OK", strlen("OK"),7000);
//
    if (ReturnStatus == FALSE)
    {
    	EF_void_LCD_Clear_Screen();
    	EF_void_LCD_goto (1 , 1);
        EF_void_LCD_print(( U8_t*) "\n*** Error: in EF_BOOLEAN_Bluetooth_MasterConnect Function at CONNECT_TO_REMOTE_DEVICE Command ***\n");
        return FALSE;
    }
    _delay_ms(1000);

    return TRUE;

}



/*********************************************************************
* Function    : EF_BOOLEAN_Bluetooth_DisConnect ();
*
* DESCRIPTION : This function used to disconnect Connection whether Module was Master or Slave
*
* PARAMETERS  : None.
*
* Return Value: BOOLEAN to check for Errors
**********************************************************************/
BOOLEAN EF_BOOLEAN_Bluetooth_DisConnect ()
{
    U8_t ReturnStatus = 0 ;

    ENABLE_AT_COMMAND_MODE;
    _delay_ms(1000);

    /**************** CONNECT_TO_REMOTE_DEVICE Command *************/
    EF_void_UART_SendArray (   (U8_t*)CommuncationCommandsArray[CONNECT_TO_REMOTE_DEVICE],
            strlen(CommuncationCommandsArray[CONNECT_TO_REMOTE_DEVICE]));


    ReturnStatus = EF_BOOLEAN_Bluetooth_ReadUntilExpectedKeyword ((U8_t*)"OK", strlen("OK"),1000);

    if (ReturnStatus == FALSE)
    {
    	EF_void_LCD_Clear_Screen();
    	EF_void_LCD_goto (1 , 1);
        EF_void_LCD_print(( U8_t*) "\n*** Error: in EF_BOOLEAN_Bluetooth_MasterConnect Function at CONNECT_TO_REMOTE_DEVICE Command ***\n");
        return FALSE;
    }
    _delay_ms(1000);

    return TRUE;
}


/*********************************************************************
* Function    : EF_BOOLEAN_Bluetooth_GetModuleStatus ( U8_t* Status );
*
* DESCRIPTION : This function used to print and return the Module Status
*               returned +STATE:<stat>\r\nOK\r\n  , <stat> are  9 status
*
* PARAMETERS  : Status: pointer to the returned status
*
* Return Value: return 1 if INITIALIZED : Slave or Master after power up
*               return 2 if READY
*               return 3 if PAIRABLE    : Slave  after AT+INIT
*               return 4 if PAIRED      : Master after pairing or binding
*               return 5 if INQUIRING   : Master after quering
*               return 6 if CONNECTING  : Master after pairing when Communication Enable (0 v to pin)
*               return 7 if CONNECTED   : Slave or Master after linking
*               return 8 if DISCONNECTED: Master or Slave
*               return 9 if UNKNOWN
*               return 0 if error
**********************************************************************/
U8_t EF_u8_Bluetooth_GetModuleStatus ( U8_t* Status )
{

    U8_t ReturnStatus = 0;

    ENABLE_AT_COMMAND_MODE;
    _delay_ms(1000);

    /**************** GET_MODULE_STATUS Command *************/
    EF_void_UART_SendArray (   (U8_t*)InitCommandsArray[GET_MODULE_STATUS],
            strlen(InitCommandsArray[GET_MODULE_STATUS]));

//    ReturnStatus = EF_BOOLEAN_Bluetooth_GetCharArray(   Returned_Array, 7 );


    ReturnStatus = EF_BOOLEAN_Bluetooth_ReadUntilExpectedKeyword ((U8_t*)"+STATE:", strlen("+STATE:"),10000);

    if (ReturnStatus == TRUE)
    {
        /* check with the expected returned values */
        ReturnStatus = EF_BOOLEAN_Bluetooth_GetCharArray(   Status, 3 );

    	EF_void_LCD_Clear_Screen();
    	EF_void_LCD_goto (1 , 1);
        EF_void_LCD_print(( U8_t*) "Status is:");
        EF_void_LCD_goto (2 , 1);
        EF_void_LCD_print(( U8_t*) Status);

        if ( strncmp("INITIALIZED", (const char *) Status      , strlen("INITIALIZED")) == 0 )       return 1;

        else if ( strncmp("READY", (const char *) Status       , strlen("READY")) == 0 )             return 2;

        else if ( strncmp("PAIRABLE", (const char *) Status    , strlen("PAIRABLE")) == 0 )          return 3;

        else if ( strncmp("PAIRED", (const char *) Status      , strlen("PAIRED")) == 0 )            return 4;

        else if ( strncmp("INQUIRING", (const char *) Status   , strlen("INQUIRING")) == 0 )         return 5;

        else if ( strncmp("CONNECTING", (const char *) Status  , strlen("CONNECTING")) == 0 )        return 6;

        else if ( strncmp("CONNECTED", (const char *) Status   , strlen("CONNECTED")) == 0 )         return CONNECTED_STATUS;

        else if ( strncmp("DISCONNECTED", (const char *) Status, strlen("DISCONNECTED")) == 0 )      return 8;

        else if ( strncmp("UNKNOWN", (const char *) Status     , strlen("UNKNOWN")) == 0 )           return 9;

    }
    else
    {
    	EF_void_LCD_Clear_Screen();
    	EF_void_LCD_goto (1 , 1);
        EF_void_LCD_print(( U8_t*) "\n*** Error: in EF_u8_Bluetooth_GetModuleStatus Function at GET_MODULE_STATUS Command ***\n");
        return FALSE;
    }


    _delay_ms(1000);

    return FALSE;
}


/*********************************************************************
* Function    : EF_BOOLEAN_Bluetooth_SendData (U8_t* Data, U8_t DataLength );
*
* DESCRIPTION : This function used to
*
* PARAMETERS  : Data : pointer to send data
*               DataLength : number of characters
*
* Return Value: return FALSE if the status is not connected
*               return TRUE if ok
**********************************************************************/
BOOLEAN EF_BOOLEAN_Bluetooth_SendData (U8_t* Data, U8_t DataLength )
{
    U8_t Status [10] = {0};
    U8_t ReturnStatus = 0;

//    ReturnStatus = EF_u8_Bluetooth_GetModuleStatus ( Status );
//
//    if (ReturnStatus == CONNECTED_STATUS )
    {
        ENABLE_COMMUNICATION_MODE;
        _delay_ms(1000);

        EF_void_UART_SendArray(   Data , DataLength );

//        ENABLE_AT_COMMAND_MODE;
//        _delay_ms(1000);
    }
//    else
//    {
    //   	EF_void_LCD_Clear_Screen();
    //   	EF_void_LCD_goto (1 , 1);
    //      EF_void_LCD_print(( U8_t*) "\n*** Error: in EF_BOOLEAN_Bluetooth_SendData Function ***\n");
//
//        return FALSE;
//    }

    return TRUE;

}



/*********************************************************************
* Function    : EF_BOOLEAN_Bluetooth_GetDataIfExist ( U8_t* RxData_ptr );
*
* DESCRIPTION : This function used to
*
* PARAMETERS  : RxData_ptr: pointer to returned data.
*
* Return Value: return FALSE if the status is not connected
*               return TRUE if ok
**********************************************************************/
BOOLEAN EF_BOOLEAN_Bluetooth_GetDataIfExist ( U8_t* RxData_ptr )
{

    U8_t Status [10] = {0};
    U8_t ReturnStatus = 0;
    U8_t counter = 0;

//    ReturnStatus = EF_u8_Bluetooth_GetModuleStatus ( Status );

//    if (ReturnStatus == CONNECTED_STATUS )
//    {
        ENABLE_COMMUNICATION_MODE;
        _delay_ms(1000);

    	EF_void_LCD_Clear_Screen();
    	EF_void_LCD_goto (1 , 1);

        while (EF_BOOLEAN_UART_CheckForRxData() == TRUE )
        {
            EF_BOOLEAN_UART_GetChar(  (RxData_ptr + counter) );
            EF_void_LCD_send_data( (U8_t)*(RxData_ptr +counter) );

            counter ++;
         }

//        ENABLE_AT_COMMAND_MODE;
//        _delay_ms(300);
//     }
//     else
//     {
//         EF_void_LCD_print(( U8_t*) "\n*** Error: in EF_BOOLEAN_Bluetooth_GetDataIfExist Function ***\n");
//
//         return FALSE;
//     }

     return TRUE;


}


