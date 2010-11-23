###############################################################################
# Makefile for the project RearLight
###############################################################################

## General Flags
PROJECT = RearLight
MCU = atmega128
TARGET = RearLight.elf
CC = avr-gcc

CPP = avr-g++

## Options common to compile, link and assembly rules
COMMON = -mmcu=$(MCU)

## Compile options common for all C compilation units.
CFLAGS = $(COMMON)
CFLAGS += -Wall -gdwarf-2 -std=gnu99 -Os -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums
CFLAGS += -MD -MP -MT $(*F).o -MF dep/$(@F).d 

## Assembly specific flags
ASMFLAGS = $(COMMON)
ASMFLAGS += $(CFLAGS)
ASMFLAGS += -x assembler-with-cpp -Wa,-gdwarf2

## Linker flags
LDFLAGS = $(COMMON)
LDFLAGS +=  -Wl,-Map=RearLight.map


## Intel Hex file production flags
HEX_FLASH_FLAGS = -R .eeprom -R .fuse -R .lock -R .signature

HEX_EEPROM_FLAGS = -j .eeprom
HEX_EEPROM_FLAGS += --set-section-flags=.eeprom="alloc,load"
HEX_EEPROM_FLAGS += --change-section-lma .eeprom=0 --no-change-warnings


## Include Directories
INCLUDES = -I"D:\Dokumentumok\BME\2010-2011 1. félév\rendszertervezés\SVN\AVRStudio\RearLight\..\Common" -I"D:\Dokumentumok\BME\2010-2011 1. félév\rendszertervezés\SVN\AVRStudio\RearLight\." 

## Objects that must be built in order to link
OBJECTS = main.o PRC.o dpy_trm_s01.o mcp2515.o mcu_avr_atmega128_api.o TIMER.o 

## Objects explicitly added by the user
LINKONLYOBJECTS = 

## Build
all: $(TARGET) RearLight.hex RearLight.eep RearLight.lss size

## Compile
main.o: ../main.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

PRC.o: ../PRC.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

dpy_trm_s01.o: ../../Common/dpy_trm_s01.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

mcp2515.o: ../../Common/mcp2515.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

mcu_avr_atmega128_api.o: ../../Common/mcu_avr_atmega128_api.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

TIMER.o: ../../Common/TIMER.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

##Link
$(TARGET): $(OBJECTS)
	 $(CC) $(LDFLAGS) $(OBJECTS) $(LINKONLYOBJECTS) $(LIBDIRS) $(LIBS) -o $(TARGET)

%.hex: $(TARGET)
	avr-objcopy -O ihex $(HEX_FLASH_FLAGS)  $< $@

%.eep: $(TARGET)
	-avr-objcopy $(HEX_EEPROM_FLAGS) -O ihex $< $@ || exit 0

%.lss: $(TARGET)
	avr-objdump -h -S $< > $@

size: ${TARGET}
	@echo
	@avr-size -C --mcu=${MCU} ${TARGET}

## Clean target
.PHONY: clean
clean:
	-rm -rf $(OBJECTS) RearLight.elf dep/* RearLight.hex RearLight.eep RearLight.lss RearLight.map


## Other dependencies
-include $(shell mkdir dep 2>NUL) $(wildcard dep/*)

