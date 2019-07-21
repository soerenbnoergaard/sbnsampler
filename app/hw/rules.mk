SOURCES += dac.c
SOURCES += midi.c
SOURCES += gpio.c

LIBS = -lasound
BIN_DIR = ../buildroot/output/host/usr/bin/
CC = $(BIN_DIR)arm-linux-gcc
LD = $(BIN_DIR)arm-linux-ld
CCARGS = -Wall -O2
LDARGS = -Wall -O2

