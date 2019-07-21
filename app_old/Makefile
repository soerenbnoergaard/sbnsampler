.PHONY: all clean cleanall

TARGET = sbnsampler

# Project main
SOURCES = main.c

# Project files
SOURCES += process.c
SOURCES += samplebank.c
SOURCES += voice.c
SOURCES += polyfilter.c
SOURCES += polyfilter_coeffs.c
SOURCES += vcf.c
SOURCES += adsr.c
SOURCES += preset.c
SOURCES += file_helpers.c
SOURCES += hardware.c

# Hardware dependent
SOURCES += dac.c
SOURCES += gpio.c
SOURCES += midi.c 
SOURCES += midi_stanford.c

SENSITIVITY = $(wildcard *.h) Makefile

OBJECTS = $(SOURCES:.c=.o)

LIBS = -lasound

BIN_DIR = ../buildroot/output/host/usr/bin/
CC = $(BIN_DIR)arm-linux-gcc
LD = $(BIN_DIR)arm-linux-ld
RM = rm -rf

CCARGS = -Wall -O2
LDARGS = -Wall -O2

Q = @

all: $(TARGET)

clean:
	$(Q)echo "    CLEAN"
	$(Q)$(RM) $(OBJECTS)

cleanall: clean
	$(Q)echo "    CLEANALL"
	$(Q)$(RM) $(TARGET)

$(TARGET): $(OBJECTS)
	$(Q)echo "    LD  $(TARGET)"
	$(Q)$(CC) $(LDARGS) -o $@ $(LIBS) $(OBJECTS)

%.o: %.c $(SENSITIVITY)
	$(Q)echo "    CC  $<"
	$(Q)$(CC) $(CCARGS) -o $@ -c $<

