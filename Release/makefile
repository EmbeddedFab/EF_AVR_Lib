################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include MCAL/subdir.mk
-include HAL/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
LSS += \
RedKitDrivers.lss \

FLASH_IMAGE += \
RedKitDrivers.hex \

EEPROM_IMAGE += \
RedKitDrivers.eep \

SIZEDUMMY += \
sizedummy \


# All Target
all: RedKitDrivers.elf secondary-outputs

# Tool invocations
RedKitDrivers.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: AVR C Linker'
	avr-gcc -Wl,-Map,RedKitDrivers.map -mmcu=atmega32 -o "RedKitDrivers.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

RedKitDrivers.lss: RedKitDrivers.elf
	@echo 'Invoking: AVR Create Extended Listing'
	-avr-objdump -h -S RedKitDrivers.elf  >"RedKitDrivers.lss"
	@echo 'Finished building: $@'
	@echo ' '

RedKitDrivers.hex: RedKitDrivers.elf
	@echo 'Create Flash image (ihex format)'
	-avr-objcopy -R .eeprom -R .fuse -R .lock -R .signature -O ihex RedKitDrivers.elf  "RedKitDrivers.hex"
	@echo 'Finished building: $@'
	@echo ' '

RedKitDrivers.eep: RedKitDrivers.elf
	@echo 'Create eeprom image (ihex format)'
	-avr-objcopy -j .eeprom --no-change-warnings --change-section-lma .eeprom=0 -O ihex RedKitDrivers.elf  "RedKitDrivers.eep"
	@echo 'Finished building: $@'
	@echo ' '

sizedummy: RedKitDrivers.elf
	@echo 'Invoking: Print Size'
	-avr-size --format=avr --mcu=atmega32 RedKitDrivers.elf
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(FLASH_IMAGE)$(ELFS)$(OBJS)$(ASM_DEPS)$(EEPROM_IMAGE)$(S_DEPS)$(SIZEDUMMY)$(S_UPPER_DEPS)$(LSS)$(C_DEPS) RedKitDrivers.elf
	-@echo ' '

secondary-outputs: $(LSS) $(FLASH_IMAGE) $(EEPROM_IMAGE) $(SIZEDUMMY)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
