.PHONY: all clean cleanall

TARGET = sbnsampler
SOURCES = main.c
OBJECTS = $(SOURCES:.c=.o)

BUILDROOT_TOPDIR = ../buildroot/
CC = $(BUILDROOT_TOPDIR)/output/host/usr/bin/arm-linux-gcc
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
	$(Q)echo "    LD $(TARGET)"
	$(Q)$(CC) -o $@ $(OBJECTS)

%.o: %.c
	$(Q)echo "    CC $<"
	$(Q)$(CC) -o $@ -c $<

