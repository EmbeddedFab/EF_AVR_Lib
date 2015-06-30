/***************************************************************
 *  Source File: EF_DIO.c
 *
 *  Description: DIO driver For ATMEGA32
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
/**************************************************************
 * Notes: This driver can use with DIO module only.
 *
 **************************************************************/

#include <avr/io.h>
#include "EF_DIO.h"


/*********************************************************************
* Function    : DIO_InitPort(U8_t PortName, U8_t Value);
*
* DESCRIPTION : This function used to Initialize port.
*
* PARAMETERS  :  
*            
*             U8_t PortName: User write which port need to access.
*                            like 'A' or 'a'.
*             U8_t Value   : User write the value in range (0 -> 255).
*
* Return Value: The function will return -1 if user enter wrong inputs.
*               or return 1 if OK.  
* Note        : To configure PIN as output you should set the bit mask "1" 
*               and to set PIN input you should set bit mask "0"
***********************************************************************/
S8_t  EF_S8_DIO_InitPort (U8_t PortName, U8_t Value)
{
    S8_t Status;
    
    Status = 1;
	switch(PortName)
	{
	     case 'a':
		 case 'A':
             DDRA = Value;
		 break;
		 case 'b':
		 case 'B':
             DDRB = Value;
		 break;
		 case 'c':
		 case 'C':
             DDRC = Value;
		 break;
		 case 'd':
		 case 'D':
             DDRD = Value;
		 break;
		 default:
             Status = -1;

    }

    return(Status);
}
/*********************************************************************
* Function    : DIO_WritePort(U8_t PortName, U8_t Value);
*
* DESCRIPTION : This function used to write value to any port.
*
* PARAMETERS  :  
*            
*             U8_t PortName: User write which port need to access.
*                            like 'A' or 'a'.
*             U8_t Value   : User write the value in range (0 -> 255).
*
* Return Value: The function will return -1 if user enter wrong inputs.
*               or return 1 if OK.  
***********************************************************************/
S8_t  EF_S8_DIO_WritePort(U8_t PortName, U8_t Value)
{
    S8_t Status;
    
    Status = 1;
	switch(PortName)
	{
	     case 'a':
		 case 'A':
             PORTA = Value;
		 break;
		 case 'b':
		 case 'B':
             PORTB = Value;
		 break;
		 case 'c':
		 case 'C':
             PORTC = Value;
		 break;
		 case 'd':
		 case 'D':
             PORTD = Value;
		 break;
		 default:
             Status = -1;
    }

    return(Status);
}

/*********************************************************************
* Function    : S16_t DIO_ReadPort (U8_t PortName);
*
* DESCRIPTION : This function used to read value from any port.
*
* PARAMETERS  :  
*            
*             U8_t PortName: User write which port need to access.
*                            like 'A' or 'a'.
*
* Return Value: The function will return -1 if user enter wrong inputs.
*               or the value.
***********************************************************************/
S16_t EF_S16_DIO_ReadPort (U8_t PortName)
{
    volatile S8_t  Status;

    switch(PortName)
	{
	     case 'a':
		 case 'A':
             Status = PINA;
		 break;
		 case 'b':
		 case 'B':
             Status = PINB;
		 break;
		 case 'c':
		 case 'C':
             Status = PINC;
		 break;
		 case 'd':
		 case 'D':
             Status = PIND;
		 break;
		 default:
             Status = -1;
    }
    return(Status);
}


/*********************************************************************
* Function    : EF_B_DIO_InitPin (U8_t PortName, U8_t PinNumber,U8_t Direction)
*
* DESCRIPTION : This function used to Initialize port.
*
* PARAMETERS  :
*
*             U8_t PortName: User write which port need to access.
*                            like 'A' or 'a'.
*             U8_t Value   : Enter pin direction wither input or output
*
* Return Value: The function will return -1 if user enter wrong inputs.
*               or return 1 if OK.
* Note        : Nothing
***********************************************************************/
BOOLEAN  EF_B_DIO_InitPin (U8_t PortName, U8_t PinNumber,U8_t Direction)
{
    BOOLEAN bStatus;


    bStatus = 1;
	switch(PortName)
	{
	     case 'a':
		 case 'A':
			  switch(Direction)
			  {
			      case INPUT:
			     	   DDRA &= ~(1 << PinNumber);
			    	   break;
			      case OUTPUT:
			    	   DDRA |=  (1 << PinNumber);
			    	   break;
			      default:
			    	   break;
			  }
		 break;
		 case 'b':
		 case 'B':
			  switch(Direction)
			  {
			      case INPUT:
			 		   DDRB &= ~(1 << PinNumber);
			 		   break;
			 	  case OUTPUT:
			 		   DDRB |=  (1 << PinNumber);
			 		   break;
			 	  default:
			 		   break;
			  }
		 break;
		 case 'c':
		 case 'C':
			  switch(Direction)
			  {
			 	  case INPUT:
			 		   DDRC &= ~(1 << PinNumber);
			 		   break;
			 	  case OUTPUT:
			 		   DDRC |=  (1 << PinNumber);
			 		   break;
			 	  default:
			 		   break;
			  }
		 break;
		 case 'd':
		 case 'D':
			  switch(Direction)
			  {
			 	  case INPUT:
			 		   DDRD &= ~(1 << PinNumber);
			 		   break;
			 	  case OUTPUT:
			 		   DDRD |=  (1 << PinNumber);
			 		   break;
			 	  default:
			 		   break;
			   }
		 break;
		 default:
             bStatus = -1;

}

    return(bStatus);
}


/*********************************************************************
* Function    : S8_t DIO_SetPin (U8_t PortName, U8_t PinNumber);
*
* DESCRIPTION : This function used to set pin any port.
*
* PARAMETERS  :  
*            
*             U8_t PortName: User write which port need to access.
*                            like 'A' or 'a'.
*             U8_t PinNumber: User write the value in range (0 -> 7).
*
* Return Value: The function will return -1 if user enter wrong inputs.
*               or return 1 if OK.  
* Note        : This function set output PIN.
***********************************************************************/
S8_t  EF_S8_DIO_SetPin   (U8_t PortName, U8_t PinNumber)
{
    S8_t Status;

    if(PinNumber > 7)
	{
	    Status = -1;
    }
	else
	{    
		Status = 1;
	    switch(PortName)
		{
		     case 'a':
			 case 'A':
	             PORTA |= (1 << PinNumber);
			 break;
			 case 'b':
			 case 'B':
	             PORTB |= (1 << PinNumber);
			 break;
			 case 'c':
			 case 'C':
	             PORTC |= (1 << PinNumber);
			 break;
			 case 'd':
			 case 'D':
	             PORTD |= (1 << PinNumber);
			 break;
			 default:
	             Status = -1;
	    }
    }
    return(Status);

}

/*********************************************************************
* Function    : S8_t DIO_ClearPin (U8_t PortName, U8_t PinNumber)
*
* DESCRIPTION : This function used to clear Pin in port.
*
* PARAMETERS  :  
*            
*             U8_t PortName: User write which port need to access.
*                            like 'A' or 'a'.
*             U8_t PinNumber: User write the value in range (0 -> 7).
*
* Return Value: The function will return -1 if user enter wrong inputs.
*               or return 1 if OK.  
***********************************************************************/
S8_t  EF_S8_DIO_ClearPin (U8_t PortName, U8_t PinNumber)
{
    S8_t Status;

    if(PinNumber > 7)
	{
	    Status = -1;
    }
	else
	{
	    Status = 1;
	    switch(PortName)
		{
		     case 'a':
			 case 'A':
	             PORTA &= ~(1 << PinNumber);
			 break;
			 case 'b':
			 case 'B':
	             PORTB &= ~(1 << PinNumber);
			 break;
			 case 'c':
			 case 'C':
	             PORTC &= ~(1 << PinNumber);
			 break;
			 case 'd':
			 case 'D':
	             PORTD &= ~(1 << PinNumber);
			 break;
			 default:
	             Status = -1;
	    }
	}
    return(Status);

}
/*********************************************************************
* Function    : S8_t  DIO_SetPort  (U8_t PortName);
*
* DESCRIPTION : This function used to set port.
*
* PARAMETERS  :  
*            
*             U8_t PortName: User write which port need to access.
*                            like 'A' or 'a'.
*             U8_t Value   : User write the value in range (0 -> 255).
*
* Return Value: The function will return -1 if user enter wrong inputs.
*               or return 1 if OK.  
***********************************************************************/
S8_t  EF_S8_DIO_SetPort  (U8_t PortName)
{
    S8_t Status;
    
    Status = 1;
	switch(PortName)
	{
	     case 'a':
		 case 'A':
             PORTA = 0xFF;
		 break;
		 case 'b':
		 case 'B':
             PORTB = 0xFF;
		 break;
		 case 'c':
		 case 'C':
             PORTC = 0xFF;
		 break;
		 case 'd':
		 case 'D':
             PORTD = 0xFF;
		 break;
		 default:
             Status = -1;
    }

    return(Status);
}

/*********************************************************************
* Function    : S8_t  DIO_ClearPort(U8_t PortName);
*
* DESCRIPTION : This function used to clear port.
*
* PARAMETERS  :  
*            
*             U8_t PortName: User write which port need to access.
*                            like 'A' or 'a'.
*
* Return Value: The function will return -1 if user enter wrong inputs.
*               or return 1 if OK.  
***********************************************************************/
S8_t  EF_S8_DIO_ClearPort(U8_t PortName)
{
    S8_t Status;
    
    Status = 1;
	switch(PortName)
	{
	     case 'a':
		 case 'A':
             PORTA = 0x00;
		 break;
		 case 'b':
		 case 'B':
             PORTB = 0x00;
		 break;
		 case 'c':
		 case 'C':
             PORTC = 0x00;
		 break;
		 case 'd':
		 case 'D':
             PORTD = 0x00;
		 break;
		 default:
             Status = -1;
    }

    return(Status);
}

/*********************************************************************
* Function    : S8_t  DIO_TogglePin(U8_t PortName, U8_t PinNumber);
*
* DESCRIPTION : This function used to toggle any PIN in any PORT.
*
* PARAMETERS  :  
*            
*             U8_t PortName: User write which port need to access.
*                            like 'A' or 'a'.
*             U8_t PinNumber: User write the value in range (0 -> 7).
*
* Return Value: The function will return -1 if user enter wrong inputs.
*               or return 1 if OK.  
***********************************************************************/
S8_t  EF_S8_DIO_TogglePin(U8_t PortName, U8_t PinNumber)
{
    S8_t Status;
    
    if(PinNumber > 7)
	{
	    Status = -1;
    }
	else
	{
	    Status = 1;
		switch(PortName)
		{
		     case 'a':
			 case 'A':
	             PORTA ^= (1 << PinNumber);
			 break;
			 case 'b':
			 case 'B':
	             PORTB ^= (1 << PinNumber);
			 break;
			 case 'c':
			 case 'C':
	             PORTC ^= (1 << PinNumber);
			 break;
			 case 'd':
			 case 'D':
	             PORTD ^= (1 << PinNumber);
			 break;
			 default:
	             Status = -1;
	    }
    }
    return(Status);
}

/*********************************************************************
* Function    : S8_t  DIO_CheckPin (U8_t PortName, U8_t PinNumber);
*
* DESCRIPTION : This function used to Check pin status .
*
* PARAMETERS  :  
*            
*             U8_t PortName: User write which port need to access.
*                            like 'A' or 'a'.
*             U8_t PinNumber: User write the value in range (0 -> 7).
*
* Return Value: The function will return -1 if user enter wrong inputs.
*               or return 1 if Pin ON and 0 If pin OFF.  
***********************************************************************/
S8_t  EF_S8_DIO_CheckPin (U8_t PortName, U8_t PinNumber)
{
    S8_t Status = 0;
    
    if(PinNumber > 7)
	{
	    Status = -1;
    }
	else
	{
		switch(PortName)
		{
		     case 'a':
			 case 'A':
	             Status = (PINA & (1 << PinNumber));
			 break;
			 case 'b':
			 case 'B':
	             Status = (PINB & (1 << PinNumber));
			 break;
			 case 'c':
			 case 'C':
	             Status = (PINC & (1 << PinNumber));
			 break;
			 case 'd':
			 case 'D':
	             Status = (PIND & (1 << PinNumber));
			 break;
			 default:
	             Status = -1;
	    }
    }
    return(Status);
}
