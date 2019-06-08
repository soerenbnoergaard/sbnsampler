#!/bin/bash

INPUT_FILE=$1
OUTPUT_FILE=$2


if [ "$INPUT_FILE" == "" ]; then
    echo "No input file given."
elif [ "$OUTPUT_FILE" == "" ]; then
    echo "No output file given."
elif [ -f "$OUTPUT_FILE" ]; then
    echo "Output file aleady exists"
else
    sox $INPUT_FILE -r 44100 -c 1 -e signed -b 16 -L $OUTPUT_FILE
fi

