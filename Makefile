# WinAVR cross-compiler toolchain is used here
CC = avr-gcc
OBJCOPY = avr-objcopy
DUDE = avrdude

# If you are not using ATtiny2313 and the USBtiny programmer,
# update the lines below to match your configuration

CFLAGS = -std=gnu99 -W -Wall -pedantic -Wstrict-prototypes -Wundef -Werror -funsigned-char -funsigned-bitfields -ffunction-sections -fpack-struct -fshort-enums -ffreestanding -Os -g -gdwarf-2 -fwhole-program -fno-inline-small-functions -fno-split-wide-types -fno-tree-scev-cprop -Wl,--relax,--gc-sections -Os -I -mmcu=attiny85
#-Wall --Pedantic -Os -I -mmcu=attiny85
OBJFLAGS = -j .text -j .data -O ihex
DUDEFLAGS =-c usbtiny -p t85 -v -e -U lfuse:w:0xe2:m -U hfuse:w:0xdf:m -U efuse:w:0xff:m

# Object files for the firmware (usbdrv/oddebug.o not strictly needed I think)
OBJECTS = farm_factory.o

# By default, build the firmware and command-line client, but do not flash
all: farm_factory.hex

# With this, you can flash the firmware by just typing "make flash" on command-line
flash: farm_factory.hex
	$(DUDE) $(DUDEFLAGS) -U flash:w:$<

# One-liner to compile the command-line client from usbtest.c
#$(CMDLINE): usbtest.c
#	gcc -I ./libusb/include -L ./libusb/lib/gcc -O -Wall usbtest.c -o usbtest.exe -lusb

# Housekeeping if you want it
clean:
	$(RM) *.o *.hex *.elf

# From .elf file to .hex
%.hex: %.elf
	$(OBJCOPY) $(OBJFLAGS) $< $@

# Main.elf requires additional objects to the firmware, not just main.o
farm_factory.elf: $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $@

# Without this dependance, .o files will not be recompiled if you change
# the config! I spent a few hours debugging because of this...
$(OBJECTS):

# From C source to .o object file
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# From assembler source to .o object file
%.o: %.S
	$(CC) $(CFLAGS) -x assembler-with-cpp -c $< -o $@
