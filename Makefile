# Hey Emacs, this is a -*- makefile -*-
#----------------------------------------------------------------------------
# avr-gcc Makefile
#
# by Steffen Vogel <info@steffenvogel.de>
#
# Released to the Public Domain - Credits to:
# Eric B. Weddington, Jörg Wunsch, Peter Fleury, Tim Henigan Colin O'Flynn,
# Reiner Patommel, Markus Pfaff, Sander Pool, Frederik Rouleau, Carlos Lamas
#----------------------------------------------------------------------------
# On command line:
#
# make all = Make software
#
# make clean = Clean out built project files
#
# make program = Download the hex file to the device, using avrdude
#                Please customize the avrdude settings below first!
#
# make filename.s = Just compile filename.c into the assembler code only
#
# make filename.i = Create a preprocessed source file for use in submitting
#                   bug reports to the GCC project.
#
# To rebuild project do "make clean" then "make all".
#----------------------------------------------------------------------------

#------------------------------ General Options -----------------------------
# MCU name
#MCU = atmega8
#MCU = atmega328p
MCU = atmega16

# Processor frequency
#     This will define a symbol, F_CPU, in all source code files equal to the 
#     processor frequency. You can then use this symbol in your source code to 
#     calculate timings. Do NOT tack on a 'UL' at the end, this will be done
#     automatically to create a 32-bit value in your source code.
#     Typical values are:
F_CPU = 14746000
#F_CPU =  1843200
#F_CPU =  2000000
#F_CPU =  3686400
#F_CPU =  4000000
#F_CPU =  7372800
#F_CPU =  8000000
#F_CPU = 11059200
#F_CPU = 14745600
#F_CPU = 16000000
#F_CPU = 18432000
#F_CPU = 20000000

# Output format
#FORMAT = srec
#FORMAT = binary
FORMAT = ihex

# Target file name (without extension)
TARGET = main

# Object files directory
OBJDIR = obj

# List C source files here (C dependencies are automatically generated)
SRC = $(TARGET).c tetris.c display.c

# List C++ source files here (C dependencies are automatically generated)
CPPSRC = 

# List Assembler source files here
#	Make them always end in a capital .S.  Files ending in a lowercase .s
#	will not be considered source files but generated files (assembler
#	output from the compiler), and will be deleted upon "make clean"!
#	Even though the DOS/Win* filesystem matches both .s and .S the same,
#	it will preserve the spelling of the filenames, and gcc itself does
#	are about how the name is spelled on its command-line.
ASRC =

# Optimization level, can be [0, 1, 2, 3, ..., s]
#	0 = turn off optimization
#	s = optimize for size
#	(Note: 3 is not always the best optimization level. See avr-libc FAQ.)
OPT = s

# Debugging format
#	Native formats for AVR-GCC's -g are dwarf-2 [default] or stabs.
#	AVR Studio 4.10 requires dwarf-2.
#	AVR [Extended] COFF format requires stabs, plus an avr-objcopy run.
DEBUG = dwarf-2

# List any extra directories to look for include files here.
#	Each directory must be seperated by a space.
#	Use forward slashes for directory separators.
#	For a directory that has spaces, enclose it in quotes.
EXTRAINCDIRS = 

# Compiler flag to set the C Standard level
#	c89   = "ANSI" C
#	gnu89 = c89 plus GCC extensions
#	c99   = ISO C99 standard (not yet fully implemented)
#	gnu99 = c99 plus GCC extensions
CSTANDARD = -std=gnu99

# Place -D or -U options here for C sources
CDEFS = -DF_CPU=$(F_CPU)UL

# Place -D or -U options here for C++ sources
#CPPDEFS += -D__STDC_LIMIT_MACROS
#CPPDEFS += -D__STDC_CONSTANT_MACROS
CPPDEFS = -DF_CPU=$(F_CPU)UL

#------------------------------ Compiler Options -----------------------------
# C Compiler Options
#	-g		generate debugging information
#	-O		optimization level
#	-f		tuning, see GCC manual and avr-libc documentation
#	-Wall		warning level
#	-Wa		tell GCC to pass this to the assembler.
#	-adhlns	create assembler listing
#CFLAGS = -g$(DEBUG)
#CFLAGS += -mint8
#CFLAGS += -mshort-calls
#CFLAGS += -Wunreachable-code
#CFLAGS += -Wsign-compare
CFLAGS += -Wall
CFLAGS += -Wstrict-prototypes
CFLAGS += -Wundef
#CFLAGS += -fno-unit-at-a-time
CFLAGS += -funsigned-char
CFLAGS += -funsigned-bitfields
CFLAGS += -fpack-struct
CFLAGS += -fshort-enums
CFLAGS += $(CDEFS)
CFLAGS += -O$(OPT)
CFLAGS += -Wa,-adhlns=$(<:%.c=$(OBJDIR)/%.lst)
CFLAGS += $(patsubst %,-I%,$(EXTRAINCDIRS))
CFLAGS += $(CSTANDARD)

# C++ Compiler Options
#	-g		generate debugging information
#	-O		optimization level
#	-f		tuning, see GCC manual and avr-libc documentation
#	-Wall		warning level
#	-Wa		tell GCC to pass this to the assembler.
#	-adhlns	create assembler listing
#CPPFLAGS = -g$(DEBUG)
#CPPFLAGS += -mint8
#CPPFLAGS += -mshort-calls
#CPPFLAGS += $(CSTANDARD)
#CPPFLAGS += -Wstrict-prototypes
#CPPFLAGS += -Wunreachable-code
#CPPFLAGS += -Wsign-compare
CPPFLAGS += -Wall
CFLAGS += -Wundef
#CPPFLAGS += -fno-unit-at-a-time
CPPFLAGS += -funsigned-char
CPPFLAGS += -funsigned-bitfields
CPPFLAGS += -fpack-struct
CPPFLAGS += -fshort-enums
CPPFLAGS += -fno-exceptions
CPPFLAGS += $(CPPDEFS)
CPPFLAGS += -O$(OPT)
CPPFLAGS += -Wa,-adhlns=$(<:%.cpp=$(OBJDIR)/%.lst)
CPPFLAGS += $(patsubst %,-I%,$(EXTRAINCDIRS))

# Assembler Options
#	-Wa		tell GCC to pass this to the assembler.
#	-adhlms		create listing
#	-gstabs		have the assembler create line number information;
#			note that for use in COFF files, additional information
#			about filenames and function names needs to be present
#			in the assembler source files
ASFLAGS = -Wa,-adhlns=$(<:%.S=$(OBJDIR)/%.lst),-gstabs 


#------------------------------ Library Options ------------------------------
# Minimalistic printf version
PRINTF_LIB_MIN = -Wl,-u,vfprintf -lprintf_min

# Floating point printf version (requires MATH_LIB = -lm below)
PRINTF_LIB_FLOAT = -Wl,-u,vfprintf -lprintf_flt

# If this is left blank, then it will use the Standard printf version.
#PRINTF_LIB = $(PRINTF_LIB_MIN)
#PRINTF_LIB = $(PRINTF_LIB_FLOAT)
PRINTF_LIB =

# Minimalistic scanf version
SCANF_LIB_MIN = -Wl,-u,vfscanf -lscanf_min

# Floating point + %[ scanf version (requires MATH_LIB = -lm below)
SCANF_LIB_FLOAT = -Wl,-u,vfscanf -lscanf_flt

# If this is left blank, then it will use the Standard scanf version.
#SCANF_LIB = $(SCANF_LIB_MIN)
#SCANF_LIB = $(SCANF_LIB_FLOAT)
SCANF_LIB =

MATH_LIB = -lm

#-------------------------- External Memory Options --------------------------
# 64 KB of external RAM, starting after internal RAM (ATmega128!),
# used for variables (.data/.bss) and heap (malloc()).
#EXTMEMOPTS = -Wl,-Tdata=0x801100,--defsym=__heap_end=0x80ffff

# 64 KB of external RAM, starting after internal RAM (ATmega128!),
# only used for heap (malloc()).
#EXTMEMOPTS = -Wl,--defsym=__heap_start=0x801100,--defsym=__heap_end=0x80ffff
EXTMEMOPTS =


#------------------------------- Linker Options ------------------------------
# -Wl		tell GCC to pass this to linker.
# -Map		create map file
# --cref	add cross reference to  map file
LDFLAGS = -Wl,-Map=$(TARGET).map,--cref
LDFLAGS += $(EXTMEMOPTS)
LDFLAGS += $(PRINTF_LIB) $(SCANF_LIB) $(MATH_LIB) -lc


#------------------------ Programmer Options (avrdude) -----------------------
# Programmer hardware
#	avr910
#	avrisp
#	arduino
#	picoweb
#	pony-stk200
#	stk200
#	stk500
# "avrdude -c ?" to get a full listing
#AVRDUDE_PROGRAMMER = avr910
#AVRDUDE_PROGRAMMER = arduino
AVRDUDE_PROGRAMMER = avrisp2

# The port your programmer is connected to
#AVRDUDE_PORT = /dev/ttyUSB0
#AVRDUDE_PORT = /dev/ttyS0
#AVRDUDE_PORT = /dev/ttyACM0
AVRDUDE_PORT = usb

AVRDUDE_WRITE_FLASH = -U flash:w:$(TARGET).hex
#AVRDUDE_WRITE_EEPROM = -U eeprom:w:$(TARGET).eep

# Uncomment the following if you want avrdude's erase cycle counter.
# Note that this counter needs to be initialized first using -Yn,
# see avrdude manual.
#AVRDUDE_ERASE_COUNTER = -y

# Uncomment the following if you do /not/ wish a verification to be
# performed after programming the device.
#AVRDUDE_NO_VERIFY = -V

# Increase verbosity level
#AVRDUDE_VERBOSE = -v -v

AVRDUDE_FLAGS = -p $(MCU) -P $(AVRDUDE_PORT) -c $(AVRDUDE_PROGRAMMER)
AVRDUDE_FLAGS += $(AVRDUDE_NO_VERIFY)
AVRDUDE_FLAGS += $(AVRDUDE_VERBOSE)
AVRDUDE_FLAGS += $(AVRDUDE_ERASE_COUNTER)


#=========================== End of Configuration ===========================
# Define programs and commands.
SHELL = bash
CC = avr-gcc
OBJCOPY = avr-objcopy
OBJDUMP = avr-objdump
SIZE = avr-size
NM = avr-nm
AVRDUDE = avrdude
REMOVE = rm -f
REMOVEDIR = rm -rf
COPY = cp

# Define messages (english)
MSG_ERRORS_NONE = Errors: none
MSG_SIZE_BEFORE = Size before: 
MSG_SIZE_AFTER = Size after:
MSG_FLASH = Creating load file for Flash:
MSG_EEPROM = Creating load file for EEPROM:
MSG_BINARY = Creating binary:
MSG_EXTENDED_LISTING = Creating Extended Listing:
MSG_SYMBOL_TABLE = Creating Symbol Table:
MSG_LINKING = Linking:
MSG_COMPILING = Compiling C:
MSG_COMPILING_CPP = Compiling C++:
MSG_ASSEMBLING = Assembling:
MSG_CLEANING = Cleaning project:
MSG_CREATING_LIBRARY = Creating library:
MSG_PROGRAM = Flashing controller:

# Define all object files
OBJ = $(SRC:%.c=$(OBJDIR)/%.o) $(CPPSRC:%.cpp=$(OBJDIR)/%.o) $(ASRC:%.S=$(OBJDIR)/%.o) 

# Define all listing files
LST = $(SRC:%.c=$(OBJDIR)/%.lst) $(CPPSRC:%.cpp=$(OBJDIR)/%.lst) $(ASRC:%.S=$(OBJDIR)/%.lst) 

# Compiler flags to generate dependency files
GENDEPFLAGS = -MD -MP -MF .dep/$(@F).d

# Combine all necessary flags and optional flags
# Add target processor to flags
ALL_CFLAGS = -mmcu=$(MCU) -I. $(CFLAGS) $(GENDEPFLAGS)
ALL_CPPFLAGS = -mmcu=$(MCU) -I. -x c++ $(CPPFLAGS) $(GENDEPFLAGS)
ALL_ASFLAGS = -mmcu=$(MCU) -I. -x assembler-with-cpp $(ASFLAGS)

# Default target
all: gccversion sizebefore build sizeafter

# Change the build target to build a HEX file or a library
build: elf hex eep bin lss sym
#build: lib
#LIBNAME=lib$(TARGET).a

elf: $(TARGET).elf
hex: $(TARGET).hex
eep: $(TARGET).eep
bin: $(TARGET).bin
lss: $(TARGET).lss
sym: $(TARGET).sym
lib: $(LIBNAME)

# Display size of file
HEXSIZE = $(SIZE) --target=$(FORMAT) $(TARGET).hex
ELFSIZE = $(SIZE) -A $(TARGET).elf

sizebefore:
	@if test -f $(TARGET).elf; then echo; echo $(MSG_SIZE_BEFORE); $(ELFSIZE); \
	$(SHELL) avr-mem.sh $(TARGET) $(MCU); fi

sizeafter:
	@if test -f $(TARGET).elf; then echo; echo $(MSG_SIZE_AFTER); $(ELFSIZE); \
	$(SHELL) avr-mem.sh $(TARGET) $(MCU); fi

# Display compiler version information
gccversion: 
	@$(CC) --version

# Program the device
program: $(TARGET).hex $(TARGET).eep
	@echo $(MSG_PROGRAM)
	@echo
	$(AVRDUDE) $(AVRDUDE_FLAGS) $(AVRDUDE_WRITE_FLASH)
	$(AVRDUDE_WRITE_EEPROM)
	
# Create final output files (.hex, .eep) from ELF output file
%.hex: %.elf
	@echo
	@echo $(MSG_FLASH) $@
	$(OBJCOPY) -O $(FORMAT) -R .eeprom $< $@

%.eep: %.elf
	@echo
	@echo $(MSG_EEPROM) $@
	$(OBJCOPY) -j .eeprom --set-section-flags=.eeprom="alloc,load" \
--change-section-lma .eeprom=0 -O $(FORMAT) $< $@

# Create binary output
%.bin: %.elf
	@echo
	@echo $(MSG_BINARY) $@
	$(OBJCOPY) $(TARGET).elf -O binary $(TARGET).bin

# Create extended listing file from ELF output file
%.lss: %.elf
	@echo
	@echo $(MSG_EXTENDED_LISTING) $@
	$(OBJDUMP) -h -S $< > $@

# Create a symbol table from ELF output file
%.sym: %.elf
	@echo
	@echo $(MSG_SYMBOL_TABLE) $@
	$(NM) -n $< > $@

# Create library from object files
.SECONDARY: $(TARGET).a
.PRECIOUS: $(OBJ)
%.a: $(OBJ)
	@echo
	@echo $(MSG_CREATING_LIBRARY) $@
	$(AR) $@ $(OBJ)

# Link: create ELF output file from object files
.SECONDARY : $(TARGET).elf
.PRECIOUS : $(OBJ)
%.elf: $(OBJ)
	@echo
	@echo $(MSG_LINKING) $@
	$(CC) $(ALL_CFLAGS) $^ -o $@ $(LDFLAGS)

# Compile: create object files from C source files
$(OBJDIR)/%.o : %.c
	@echo $(MSG_COMPILING) $<
	$(CC) -c $(ALL_CFLAGS) $< -o $@

# Compile: create object files from C++ source files
$(OBJDIR)/%.o : %.cpp
	@echo
	@echo $(MSG_COMPILING_CPP) $<
	$(CC) -c $(ALL_CPPFLAGS) $< -o $@

# Compile: create assembler files from C source files
%.s : %.c
	$(CC) -S $(ALL_CFLAGS) $< -o $@

# Compile: create assembler files from C++ source files
%.s : %.cpp
	$(CC) -S $(ALL_CPPFLAGS) $< -o $@

# Assemble: create object files from assembler source files
$(OBJDIR)/%.o : %.S
	@echo
	@echo $(MSG_ASSEMBLING) $<
	$(CC) -c $(ALL_ASFLAGS) $< -o $@

# Create preprocessed source for use in sending a bug report
%.i : %.c
	$(CC) -E -mmcu=$(MCU) -I. $(CFLAGS) $< -o $@

# Clean project
clean:
	@echo $(MSG_CLEANING)
	$(REMOVE) $(TARGET).hex
	$(REMOVE) $(TARGET).bin
	$(REMOVE) $(TARGET).eep
	$(REMOVE) $(TARGET).elf
	$(REMOVE) $(TARGET).map
	$(REMOVE) $(TARGET).sym
	$(REMOVE) $(TARGET).lss
	$(REMOVEDIR) $(OBJDIR)
	$(REMOVE) $(SRC:.c=.s)
	$(REMOVE) $(SRC:.c=.d)
	$(REMOVEDIR) .dep

# Create object files directory
	mkdir $(OBJDIR)

# Include the dependency files
-include $(shell mkdir .dep 2>/dev/null) $(wildcard .dep/*)

# Listing of phony targets
.PHONY: all finish sizebefore sizeafter gccversion \
build elf hex eep lss sym \
clean program
