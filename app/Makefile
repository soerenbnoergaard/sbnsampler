.PHONY: all clean cleanall

TARGET = sbnsampler

SOURCES = main.c \
	  dac.c \
	  midi.c \
	  samplebank.c \
	  file_helpers.c

OBJECTS = $(SOURCES:.c=.o)

LIBS = -lasound

BIN_DIR = ../buildroot/output/host/usr/bin
CC = $(BIN_DIR)/arm-linux-gcc
LD = $(BIN_DIR)/arm-linux-ld
RM = rm -rf

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
	$(Q)$(CC) -o $@ $(LIBS) $(OBJECTS)

%.o: %.c
	$(Q)echo "    CC  $<"
	$(Q)$(CC) -o $@ -c $<

