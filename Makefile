TARGET := carproject
SRC := 	src/button.c \
		src/fix8.c \
		src/irsens.c \
		src/lcd.c \
		src/led.c \
		src/main.c \
		src/manager.c \
		src/measurer.c \
		src/motor.c \
		src/pid.c \
		src/state_drive.c \
		src/state_error.c \
		src/state_idle.c \
		src/state_measure.c \
		src/state_recover.c \
		src/steering.c \
		src/tacho.c

RM := rm -rf
OBJS := $(SRC:%.c=%.o)

# All Target
all: $(TARGET).hex

# Tool invocations
%.o: %.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -Os -fpack-struct -fshort-enums -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega2560 -DF_CPU=16000000UL -MMD -MP -MF"$(@:%.o=%.dep)" -MT"$(@:%.o=%.dep)" -c -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

$(TARGET).elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: AVR C Linker'
	avr-gcc -mmcu=atmega2560 -o$(TARGET).elf $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

$(TARGET).hex: $(TARGET).elf
	@echo 'Create Flash image (ihex format)'
	-avr-objcopy -R .eeprom -O ihex $(TARGET).elf  $(TARGET).hex
	@echo 'Finished building: $@'
	@echo ' '

sizedummy: $(TARGET).elf
	@echo 'Invoking: Print Size'
	-avr-size --format=berkeley -t $(TARGET).elf
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJS) $(TARGET).elf $(TARGET).hex $(SRC:%.c=%.dep)
	-@echo ' '

program: $(TARGET).hex
	@echo 'Invoking: Avrdude'
	avrdude -c stk500v2 -p m2560 -P /dev/ttyACM0 -U flash:w:$(TARGET).hex
	@echo 'Finished uploading: $@'
	@echo ' '

program_check:
	@echo 'Invoking: Avrdude'
	avrdude -c stk500v2 -p m2560 -P /dev/ttyACM0
	@echo ' '

.PHONY: all clean dependents sizedummy program program_check
.SECONDARY:

-include $(SRC:%.c=%.dep)
