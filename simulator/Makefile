.PHONY: all clean cleanall

SHARED = 1

ifeq ($(SHARED), 1)
TARGET = simulator.so
else
TARGET = simulator
endif

APP_DIR = ../app/
OBJ_DIR = ./_objects/

INCLUDES = -I$(APP_DIR)

# Project main
SOURCES = main.c

# Project files
SOURCES += $(APP_DIR)process.c
SOURCES += $(APP_DIR)samplebank.c
SOURCES += $(APP_DIR)voice.c
SOURCES += $(APP_DIR)polyfilter.c
SOURCES += $(APP_DIR)polyfilter_coeffs.c
SOURCES += $(APP_DIR)vcf.c
SOURCES += $(APP_DIR)adsr.c
SOURCES += $(APP_DIR)preset.c
SOURCES += $(APP_DIR)file_helpers.c
SOURCES += $(APP_DIR)hardware.c

# Hardware dependent
SOURCES += gpio.c
SOURCES += midi.c
SOURCES += dac.c

SENSITIVITY = $(wildcard *.h) $(wildcard $(APP_DIR)*.h) Makefile

OBJECTS = $(addprefix $(OBJ_DIR), $(notdir $(SOURCES:.c=.o)))

LIBS =

BIN_DIR = 
CC = $(BIN_DIR)gcc
LD = $(BIN_DIR)ld
RM = rm -rf

ifeq ($(SHARED), 1)
CCARGS = -Wall -O0 -g3 -fPIC
LDARGS = -Wall -O0 -g3 -shared
else
CCARGS = -Wall -O0 -g3
LDARGS = -Wall -O0 -g3
endif

Q = @

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(Q)echo "    LD  $(TARGET)"
	$(Q)$(CC) $(LDARGS) -o $@ $(LIBS) $(OBJECTS)

$(OBJ_DIR)%.o: %.c $(SENSITIVITY) | $(OBJ_DIR)
	$(Q)echo "    CC  $<"
	$(Q)$(CC) $(CCARGS) $(INCLUDES) -o $@ -c $<

$(OBJ_DIR)%.o: $(APP_DIR)%.c $(SENSITIVITY) | $(OBJ_DIR)
	$(Q)echo "    CC  $<"
	$(Q)$(CC) $(CCARGS) $(INCLUDES) -o $@ -c $<

$(OBJ_DIR):
	$(Q)echo "    MKDIR $@"
	$(Q)mkdir $@

clean:
	$(Q)echo "    CLEAN"
	$(Q)$(RM) $(OBJ_DIR)

cleanall: clean
	$(Q)echo "    CLEANALL"
	$(Q)$(RM) $(TARGET)

