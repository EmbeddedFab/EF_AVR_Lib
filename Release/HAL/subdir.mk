################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../HAL/EF_7Segment.c \
../HAL/EF_Bluetooth.c \
../HAL/EF_DS1307.c \
../HAL/EF_E2PROM_24C16.c \
../HAL/EF_ESP8266.c \
../HAL/EF_Keypad.c \
../HAL/EF_LCD.c \
../HAL/EF_SkyLabGPS.c \
../HAL/EF_UltraSonic.c \
../HAL/EF_Wavecom.c \
../HAL/EF_nRF2401.c 

OBJS += \
./HAL/EF_7Segment.o \
./HAL/EF_Bluetooth.o \
./HAL/EF_DS1307.o \
./HAL/EF_E2PROM_24C16.o \
./HAL/EF_ESP8266.o \
./HAL/EF_Keypad.o \
./HAL/EF_LCD.o \
./HAL/EF_SkyLabGPS.o \
./HAL/EF_UltraSonic.o \
./HAL/EF_Wavecom.o \
./HAL/EF_nRF2401.o 

C_DEPS += \
./HAL/EF_7Segment.d \
./HAL/EF_Bluetooth.d \
./HAL/EF_DS1307.d \
./HAL/EF_E2PROM_24C16.d \
./HAL/EF_ESP8266.d \
./HAL/EF_Keypad.d \
./HAL/EF_LCD.d \
./HAL/EF_SkyLabGPS.d \
./HAL/EF_UltraSonic.d \
./HAL/EF_Wavecom.d \
./HAL/EF_nRF2401.d 


# Each subdirectory must supply rules for building sources it contributes
HAL/%.o: ../HAL/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -Os -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega32 -DF_CPU=8000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


