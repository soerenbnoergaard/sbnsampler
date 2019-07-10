#!/bin/bash

# Convert all files matching *_.wav to a correct-format *.wav file
ALLFILES=`ls *_.wav`

for infile in $ALLFILES; do
    outfile=${infile%_.*}".wav"
    ../../scripts/convert_wav.sh $infile $outfile
done
