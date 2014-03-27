DEVICE = atmega1284p
MCU = atmega1284p
AVRDUDE_DEVICE = m1284p
PORT ?= usb # use an external AVR programmer
#PORT ?=/dev/tty.usbmodem00055903 # use internal prgmr on my board via a Mac

CFLAGS=-g -Wall -mcall-prologues -std=c99 -mmcu=$(MCU) $(DEVICE_SPECIFIC_CFLAGS) -Os
CC=avr-gcc
OBJ2HEX=avr-objcopy 
LDFLAGS=-Wl,-gc-sections -lpololu_$(DEVICE) -Wl,-relax

AVRDUDE=avrdude
TARGET=lab2
OBJECT_FILES=lab2.o

all: $(TARGET).hex

clean:
	rm -f *.o *.hex *.obj *.hex

%.hex: %.obj
	$(OBJ2HEX) -R .eeprom -O ihex $< $@

$(TARGET).o: $(TARGET).c

dis: $(TARGET).o
	avr-objdump -d -S --target=ihex $(TARGET).o > $(TARGET).lsa
	avr-objdump -S --target=ihex $(TARGET).o > $(TARGET).lss
	

%.obj: $(OBJECT_FILES)
	$(CC) $(CFLAGS) $(OBJECT_FILES) $(LDFLAGS) -o $@

program: $(TARGET).hex
	$(AVRDUDE) -p $(AVRDUDE_DEVICE) -c avrisp2 -P $(PORT) -U flash:w:$(TARGET).hex
