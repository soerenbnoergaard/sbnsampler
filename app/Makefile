.PHONY: all clean cleanall

SIM = 0
ifeq ($(SIM), 1)
	HAL_DIR = ./sim/
	TARGET = sbnsamplersim
else
	HAL_DIR = ./hw/
	TARGET = sbnsampler
endif

OBJ_DIR = ./__objects__/

INCLUDES = -I.

# Project main
SOURCES = main.c

# Project files
SOURCES += vco.c
SOURCES += vca.c
SOURCES += vcf.c
SOURCES += voice.c
SOURCES += ctrl.c
SOURCES += polyfilter_coeffs.c
SOURCES += adsr.c
SOURCES += panel.c

SENSITIVITY = $(wildcard *.h) Makefile
OBJECTS = $(addprefix $(OBJ_DIR), $(notdir $(SOURCES:.c=.o)))

RM = rm -rf

# Hardware-dependent info
include $(HAL_DIR)rules.mk

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
	$(Q)$(CC) $(CCARGS) $(INCLUDES) -o $@ -c $<

$(OBJ_DIR)%.o: $(HAL_DIR)%.c $(SENSITIVITY) | $(OBJ_DIR)
	$(Q)echo "    CC  $<"
	$(Q)$(CC) $(CCARGS) $(INCLUDES) -o $@ -c $<

$(OBJ_DIR):
	$(Q)echo "    MKDIR $@"
	$(Q)mkdir $(OBJ_DIR)
