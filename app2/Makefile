.PHONY: all clean cleanall

TARGET = sbnsampler
OBJ_DIR = ./__objects__/

# Project main
SOURCES = main.c

# Project files
SOURCES += vco.c
SOURCES += voice.c

# Hardware dependent

SENSITIVITY = $(wildcard *.h) Makefile
OBJECTS = $(addprefix $(OBJ_DIR), $(notdir $(SOURCES:.c=.o)))

LIBS = 

BIN_DIR = 
CC = $(BIN_DIR)gcc
LD = $(BIN_DIR)ld
RM = rm -rf

CCARGS = -Wall -O0 -g3
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

$(OBJ_DIR):
	$(Q)echo "    MKDIR $@"
	$(Q)mkdir $(OBJ_DIR)
