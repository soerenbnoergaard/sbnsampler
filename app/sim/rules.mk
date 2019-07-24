SOURCES += dac.c
SOURCES += midi.c
SOURCES += gpio.c

LIBS = 
BIN_DIR = 
CC = $(BIN_DIR)gcc
LD = $(BIN_DIR)ld
CCARGS = -Wall -O0 -g3 -pg
LDARGS = -Wall -O0 -g3 -pg

