TARGET := carproject
SRC :=	src/button.c \
		src/fix8.c \
		src/irsens.c \
		src/lcd.c \
		src/led.c \
		src/main.c \
		src/manager.c \
		src/measurer.c \
		src/motor.c \
		src/controller.c \
		src/state_drive.c \
		src/state_error.c \
		src/state_idle.c \
		src/state_recover.c \
		src/steering.c \
		src/tacho.c

RM := rm -rf
OBJS := $(SRC:%.c=%.o)

all: $(TARGET).hex

%.o: %.c
	@echo 'Compiling $<'
	@avr-gcc -Wall -Os -fpack-struct -fshort-enums -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega2560 -DF_CPU=16000000UL -MMD -MP -MF"$(@:%.o=%.dep)" -MT"$(@:%.o=%.dep)" -c -o"$@" "$<"

$(TARGET).elf: $(OBJS) $(USER_OBJS)
	@echo 'Linking $@'
	@avr-gcc -mmcu=atmega2560 -o$(TARGET).elf $(OBJS) $(USER_OBJS) $(LIBS)
	@-avr-size --format=avr --mcu=atmega2560 $(TARGET).elf

$(TARGET).hex: $(TARGET).elf
	@echo 'Creating $@'
	@-avr-objcopy -R .eeprom -O ihex $(TARGET).elf  $(TARGET).hex

program: $(TARGET).hex
	@echo 'Programming'
	@avrdude -c stk500v2 -p m2560 -P /dev/ttyACM0 -U flash:w:$(TARGET).hex

clean:
	-$(RM) $(OBJS) $(TARGET).elf $(TARGET).hex $(SRC:%.c=%.dep)

.PHONY: all clean dependents sizedummy program program_check
.SECONDARY:

-include $(SRC:%.c=%.dep)
