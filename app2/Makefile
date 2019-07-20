.PHONY: all clean cleanall

TARGET = sbnsampler
OBJ_DIR = ./__objects__/
HAL_DIR = ./sim/

INCLUDES = -I.

# Project main
SOURCES = main.c

# Project files
SOURCES += vco.c
SOURCES += voice.c
SOURCES += ctrl.c

# Hardware dependent
SOURCES += $(HAL_DIR)dac.c
SOURCES += $(HAL_DIR)midi.c

SENSITIVITY = $(wildcard *.h) Makefile
OBJECTS = $(addprefix $(OBJ_DIR), $(notdir $(SOURCES:.c=.o)))

LIBS = 

BIN_DIR = 
CC = $(BIN_DIR)gcc
LD = $(BIN_DIR)ld
RM = rm -rf

CCARGS = -Wall -O0 -g3 $(INCLUDES)
LDARGS = -Wall -O0 -g3

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

$(OBJ_DIR)%.o: %.c $(SENSITIVITY) | $(OBJ_DIR)
	$(Q)echo "    CC  $<"
	$(Q)$(CC) $(CCARGS) -o $@ -c $<

$(OBJ_DIR)%.o: $(HAL_DIR)%.c $(SENSITIVITY) | $(OBJ_DIR)
	$(Q)echo "    CC  $<"
	$(Q)$(CC) $(CCARGS) -o $@ -c $<

$(OBJ_DIR):
	$(Q)echo "    MKDIR $@"
	$(Q)mkdir $(OBJ_DIR)
