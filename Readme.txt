/=======================================================/
Embedded Fab provides OpenSource Atmel Library
Any one can modify or fix any bugs and pull it to us
/=======================================================/

The library depends on the Abstraction Layers with Application programming Interface (API).
|-----------------------------------|
| Application layer |               |
|-------------------| service Layer |
|     HAL           |               |
|-----------------------------------|
|		 MCAL               |
|-----------------------------------|
There are:
	-MCAL file (drivers handle with the modules in uC)
	-HAL file (drivers on the Board or out , there are not modules in uC)
	-ServiceLayer (data types definition)
	-References File in HAL (references to some externel Modules
				and Embedded Fab Red kit features )
	-Documentation File in HAL ,(Documentation to some externel Modules)
	- GuideLines_to_Learn_Git pdf 
