
# Define the target
TARGET = main

# -------------------- Compiler Settings --------------------------------

CC = avr-gcc
OBJCOPY = avr-objcopy
CFLAGS = -c -g -Os -w -ffunction-sections -fdata-sections -MMD -mmcu=$(MCU) -DF_CPU=$(F_CPU) -I$(ROOT_DIRECTORY)/lib
LFLAGS = -mmcu=$(MCU) -o "build/$(PRODUCT_TITLE).elf" "build/$(TARGET).c.o" -Lbuild/

# Include this line for floating point printf
#LFLAGS +=-Wl,-u,vfprintf -lprintf_flt -lm

OBJ_COPY_HEX_FLAGS = -O ihex -R .eeprom "build/$(PRODUCT_TITLE).elf" "build/$(PRODUCT_TITLE).hex"
OBJ_COPY_EEP_FLAGS = -O ihex -j .eeprom --set-section-flags=.eeprom=alloc,load --no-change-warnings
OBJ_COPY_EEP_FLAGS += --change-section-lma .eeprom=0 "build/$(PRODUCT_TITLE).elf" "build/$(PRODUCT_TITLE).eep"

PRODUCT_TITLE = Bluebox
ROOT_DIRECTORY = $(shell pwd)
MKDIR_BUILD = $(shell mkdir -p build)

# ----------------------- MCU Configuration ------------------------------

# Microcontroller
MCU = atmega328p

# Clock rate
F_CPU = 16000000L

# --------------------- AVRDUDE Settings ------------------------------

PROGRAMMER = usbtiny

# Upload Baud rate
FLASH_BAUD = 115200

FLASH_FLAGS = -V

# --------------------------- Make targets ----------------------------------

all : obj_copy mk_build

obj_copy : link
	$(OBJCOPY) $(OBJ_COPY_EEP_FLAGS)
	$(OBJCOPY) $(OBJ_COPY_HEX_FLAGS)

link : src mk_build
	$(CC) $(LFLAGS)

mk_build :
	$(MKDIR_BUILD)

src : mk_build
	$(CC) $(CFLAGS) src/main.c -o build/main.c.o

upload : link mk_build
	@echo
	@echo Uploading to AVR chip
	avrdude $(FLASH_FLAGS) -c $(PROGRAMMER) -p $(MCU) -b $(FLASH_BAUD) -U flash:w:build/$(PRODUCT_TITLE).hex

.PHONY : clean

clean:
	rm -rf build
