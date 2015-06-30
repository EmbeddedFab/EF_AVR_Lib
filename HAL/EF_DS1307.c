/***************************************************************
 *  Source File: EF_DS1307.c
 *
 *  Description: This simple driver for RTC DS1307
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
#include "EF_DS1307.h"

/*
 * transform decimal value to bcd :Binary code decimal
 * convert each digit in decimal separately and put them to gather
 * Ex: (21)d >> convert 2 to binary 0010, 1: 0001 ,so
 * (21)d = (0b00100001) = (0x21), you can add 6 twice to get the number
 */
static U8_t DecToBcdConvert(U8_t val)
{
	return val + 6 * (val / 10);
}

/*
 * transform bcd value to decimal
 */
static U8_t BcdToDecConvert(U8_t val)
{
	return val - 6 * (val >> 4);
}


/*********************************************************************
 * Function    : void EF_void_DS1307_Init(void);
 *
 * DESCRIPTION : This function is used to initialize the Ds1307 RTC.
 *
 * PARAMETERS  : Void
 *
 * Return Value: Void
 *
 * Notes:
 ***********************************************************************/
void EF_void_DS1307_Init(void)
{
	EF_void_I2C_Init();                  /* Initilize the I2c module. 							  */
	EF_void_I2C_Start();                 /* Start I2C communication 							  */

	EF_void_I2C_Write(DS1307_ID);        /* Connect to DS1307 by sending its ID on I2c Bus 		  */
	EF_void_I2C_Write(CONTROL);          /* Select the Ds1307 ControlRegister to configure Ds1307 */

	EF_void_I2C_Write(0x00);             /* Write 0x00 to Control register to disable SQW-Out 	  */

	EF_void_I2C_Stop();                  /* Stop I2C communication after initilizing DS1307 	  */

	_delay_us(100);						 /* needing delay ,if setting date/time after this funct. */

}

/*********************************************************************
 * Function    : void EF_void_DS1307_SetTime(U8_t hh, U8_t mm, U8_t ss);
 *
 * DESCRIPTION : This function is used to set Time(hh,mm,ss) into the Ds1307 RTC.
 *
 * PARAMETERS  : U8_t hh : hour
 *               U8_t mm : minutes
 *               U8_t ss : second
 *
 * Return Value: Void
 ***********************************************************************/
void EF_void_DS1307_SetTime(U8_t hh, U8_t mm, U8_t ss)
{
	U8_t Hour_Temp   = 0;
	U8_t Minute_Temp = 0;
	U8_t Second_Temp = 0;
	/*The contents of the time and calendar registers are in the BCD format.*/
	Hour_Temp   = DecToBcdConvert(hh);
	Minute_Temp = DecToBcdConvert(mm);
	Second_Temp = DecToBcdConvert(ss);

	EF_void_I2C_Start();                 /* Start I2C communication 					   */
	EF_void_I2C_Write(DS1307_ID);        /* Connect to DS1307 by sending its ID on I2c Bus */
	EF_void_I2C_Write(SEC_ADDRESS);      /* Select the SEC RAM address 					   */

	EF_void_I2C_Write(Second_Temp);      /* Write sec on RAM address 00H				   */
	EF_void_I2C_Write(Minute_Temp);      /* Write min on RAM address 01H				   */
	EF_void_I2C_Write(Hour_Temp);        /* Write hour on RAM address 02H				   */

	EF_void_I2C_Stop(); 				 /* Stop I2C communication after Setting the Time  */
}

/*********************************************************************
 * Function    : void EF_void_DS1307_SetDate(U8_t dd, U8_t mm, U8_t yy);
 *
 * DESCRIPTION : This function is used to set Date(dd,mm,yy) into the Ds1307 RTC.
 *
 * PARAMETERS  : U8_t dd : day
 *               U8_t mm : month
 *               U8_t yy : year
 *
 * Return Value: Void
 *
 * Notes:
 ***********************************************************************/
void EF_void_DS1307_SetDate(U8_t dd, U8_t mm, U8_t yy)
{
	U8_t Day_Temp   = 0;
	U8_t Month_Temp = 0;
	U8_t Year_Temp  = 0;
	/*The contents of the time and calendar registers are in the BCD format.*/
	Day_Temp   = DecToBcdConvert(dd);
	Month_Temp = DecToBcdConvert(mm);
	Year_Temp = DecToBcdConvert(yy);

	EF_void_I2C_Start();                 /* Start I2C communication 					   */

	EF_void_I2C_Write(DS1307_ID);        /* Connect to DS1307 by sending its ID on I2c Bus */
	EF_void_I2C_Write(DATE_ADDRESS);     /* Request DAY RAM address at 04H 				   */

	EF_void_I2C_Write(Day_Temp);         /* Write date on RAM address 04H				   */
	EF_void_I2C_Write(Month_Temp);       /* Write month on RAM address 05H 				   */
	EF_void_I2C_Write(Year_Temp);        /* Write year on RAM address 06h 				   */

	EF_void_I2C_Stop();                  /* Stop I2C communication after Setting the Time */
}

/*********************************************************************************
 * Function    : void EF_void_DS1307_GetTime(U8_t *h_ptr,U8_t *m_ptr,U8_t *s_ptr);
 *
 * DESCRIPTION : This function is used to get the Time(hh,mm,ss) from Ds1307 RTC.
 *
 * PARAMETERS  : U8_t *h_ptr: pointer to returned hour
 *               U8_t *m_ptr: pointer to returned minute
 *               U8_t *s_ptr: pointer to returned seconds
 *
 * Return Value: Void
 *
 * Notes:
 *********************************************************************************/
void EF_void_DS1307_GetTime(U8_t *h_ptr,U8_t *m_ptr,U8_t *s_ptr)
{
	U8_t Hour_Temp   = 0;
	U8_t Minute_Temp = 0;
	U8_t Second_Temp = 0;

	EF_void_I2C_Start();                   /* Start I2C communication 					     */

	EF_void_I2C_Write(DS1307_ID);          /* Connect to DS1307 by sending its ID on I2c Bus */
	EF_void_I2C_Write(SEC_ADDRESS);        /* Select the SEC RAM address 					 */

	EF_void_I2C_Stop();                    /* Stop I2C communication after Setting the Time   */

	EF_void_I2C_Start();                   /* Start I2C communication 					      */

	EF_void_I2C_Write(0xD1); 			   /* connect to DS1307( ID with Read mode) 		  */
	/* The contents of the time and calendar registers are in the BCD format. */
	Second_Temp = EF_U8_I2C_Read_Byte(1);   /* read second and return Positive ACK		       */
	Minute_Temp = EF_U8_I2C_Read_Byte(1);   /* read minute and return Positive ACK 		       */
	Hour_Temp   = EF_U8_I2C_Read_Byte(0);   /* read hour and return Negative/No ACK 		   */

	*s_ptr = BcdToDecConvert(Second_Temp);
	*m_ptr = BcdToDecConvert(Minute_Temp);
	*h_ptr = BcdToDecConvert(Hour_Temp);

	EF_void_I2C_Stop();                    /* Stop I2C communication after Setting the Time   */
}



/*********************************************************************
 * Function    : void EF_void_DS1307_GetDate(U8_t *d_ptr,U8_t *m_ptr,U8_t *y_ptr);
 *
 * DESCRIPTION :  This function is used to get the Date(y,m,d) from Ds1307 RTC.
 *
 * PARAMETERS  : U8_t *d_ptr: pointer to returned day
 *               U8_t *m_ptr: pointer to returned month
 *               U8_t *y_ptr: pointer to returned year
 *
 * Return Value: Void
 *
 * Notes:
 ***********************************************************************/
void EF_void_DS1307_GetDate(U8_t *d_ptr,U8_t *m_ptr,U8_t *y_ptr)
{
	U8_t Day_Temp   = 0;
	U8_t Month_Temp = 0;
	U8_t Year_Temp  = 0;

	EF_void_I2C_Start();                 /* Start I2C communication */

	EF_void_I2C_Write(DS1307_ID);        /* Connect to DS1307 by sending its ID on I2c Bus */
	EF_void_I2C_Write(DATE_ADDRESS);     /* Request DAY RAM address at 04H */

	EF_void_I2C_Stop();                  /* Stop I2C communication after Setting the Time */

	EF_void_I2C_Start();                 /* Start I2C communication */

	EF_void_I2C_Write(0xD1);             /* connect to DS1307( ID with Read mode) */

	/* The contents of the time and calendar registers are in the BCD format. */
	Day_Temp   = EF_U8_I2C_Read_Byte(1);     /* read Day and return Positive ACK */
	Month_Temp = EF_U8_I2C_Read_Byte(1);     /* read Month and return Positive ACK */
	Year_Temp  = EF_U8_I2C_Read_Byte(0);     /* read Year and return Negative/No ACK */

	*d_ptr = BcdToDecConvert(Day_Temp);
	*m_ptr = BcdToDecConvert(Month_Temp);
	*y_ptr = BcdToDecConvert(Year_Temp);

	EF_void_I2C_Stop();                  /* Stop I2C communication after Setting the Time */
}

