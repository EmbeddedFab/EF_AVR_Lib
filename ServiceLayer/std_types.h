/***************************************************************
 *  Source File: std_types.h
 *
 *  Description: standard data types
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


#ifndef STD_TYPES_H
#define STD_TYPES_H

typedef unsigned char BOOLEAN;

typedef unsigned char U8_t;
typedef char S8_t;

typedef unsigned int U16_t;
typedef signed int S16_t;

typedef unsigned long U32_t;
typedef signed long S32_t;

typedef float FP32;
typedef double FP64;

typedef int MAINRETURN_t;

#define BYTE S8_t
#define UBYTE U8_t

#define WORD S16_t
#define UWORD U16_t

#define LONG S32_t
#define ULONG U32_t

#define TRUE 1
#define FALSE 0

#define FAILURE  0
#define SUCESS   1

//#ifndef NULL
#define NULL 0
//#endif
#endif
