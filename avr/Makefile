MCU = atmega328p
F_CPU = 16000000UL
CC = avr-gcc
CFLAGS = -Wall -Os -DF_CPU=$(F_CPU) -mmcu=$(MCU)
OBJCOPY = avr-objcopy
TARGET = main

all: $(TARGET).hex

$(TARGET).elf: $(TARGET).c
	$(CC) $(CFLAGS) -o $@ $<

$(TARGET).hex: $(TARGET).elf
	$(OBJCOPY) -O ihex -R .eeprom $< $@

clean:
	rm -f *.elf *.hex

