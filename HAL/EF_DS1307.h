/***************************************************************
 *  Source File: EF_DS1307.h
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

#ifndef EF_DS1307_H_
#define EF_DS1307_H_


#include <avr\io.h>
#include <util\delay.h>
#include "../MCAL/EF_I2C.h"
#include "../ServiceLayer/std_types.h"

/**************************************************************
 * Definitions
 **************************************************************/
/* Below values are fixed and should not be changed.
 Refer Ds1307 DataSheet for more info*/

#define DS1307_ID          0xD0 /* DS1307 ID */
#define SEC_ADDRESS        0x00 /* Address to access Ds1307 SEC register */
#define DATE_ADDRESS       0x04 /* Address to access Ds1307 DATE register */
#define CONTROL            0x07 /* Address to access Ds1307 CONTROL register */


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
extern void EF_void_DS1307_Init(void);

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
extern void EF_void_DS1307_SetTime(U8_t hh, U8_t mm, U8_t ss);


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
extern void EF_void_DS1307_SetDate(U8_t dd, U8_t mm, U8_t yy);

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
extern void EF_void_DS1307_GetTime(U8_t *h_ptr,U8_t *m_ptr,U8_t *s_ptr);


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
extern void EF_void_DS1307_GetDate(U8_t *d_ptr,U8_t *m_ptr,U8_t *y_ptr);

#endif /* EF_DS1307_H_ */
