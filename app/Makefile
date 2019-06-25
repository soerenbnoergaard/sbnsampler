.PHONY: all clean cleanall

TARGET = sbnsampler

SOURCES = main.c \
	  dac.c \
	  midi.c \
	  samplebank.c \
	  voice.c \
	  polyfilter.c \
	  polyfilter_coeffs.c \
	  vcf.c \
	  adsr.c \
	  gpio.c \
	  midi_stanford.c \
	  preset.c \
	  file_helpers.c

SENSITIVITY = $(wildcard *.h)

OBJECTS = $(SOURCES:.c=.o)

LIBS = -lasound

BIN_DIR = ../buildroot/output/host/usr/bin
CC = $(BIN_DIR)/arm-linux-gcc
LD = $(BIN_DIR)/arm-linux-ld
RM = rm -rf

CCARGS = -Wall -Werror -O2
LDARGS = -Wall -Werror -O2

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

