#!/usr/bin/python3
import os
import re
import sys
import doctest
import argparse

from collections import OrderedDict

# Midi numbers for each note and octave
#     C   C#  D   D#  E   F   F#  G   G#  A   A#  B  
# -1   0   1   2   3   4   5   6   7   8   9  10  11
#  0  12  13  14  15  16  17  18  19  20  21  22  23 
#  1  24  25  26  27  28  29  30  31  32  33  34  35 
#  2  36  37  38  39  40  41  42  43  44  45  46  47 
#  3  48  49  50  51  52  53  54  55  56  57  58  59 
#  4  60  61  62  63  64  65  66  67  68  69  70  71 
#  5  72  73  74  75  76  77  78  79  80  81  82  83 
#  6  84  85  86  87  88  89  90  91  92  93  94  95 
#  7  96  97  98  99  100 101 102 103 104 105 106 107
#  8  108 109 110 111 112 113 114 115 116 117 118 119
#  9  120 121 122 123 124 125 126 127                

def generate_lut():
    lut = OrderedDict()
    letter = ""
    for n in range(128):
        k = (n%12)
        l = n//12 - 1
        letter = { 0: "c", 1: "c#", 2: "d", 3: "d#", 4: "e", 5: "f", 6: "f#", 7: "g", 8: "g#", 9: "a", 10: "a#", 11: "b" }[k]
        equivalents = {
            "c#": "db",
            "d#": "eb",
            "f#": "gb",
            "g#": "ab",
            "a#": "bb",
        }
        key = "{:s}{:d}".format(letter, l)
        lut[key] = n
        if letter in equivalents.keys():
            key = "{:s}{:d}".format(equivalents[letter], l)
            lut[key] = n
    return lut

def lookup(s):
    """
    >>> lookup("a#2.wav")
    46

    >>> lookup("foo_a#2_bar.wav")
    46

    >>> lookup("foo_a#_2_bar.wav")
    46

    >>> lookup("bb5.wav")
    82

    >>> lookup("Bb5.wav")
    82

    >>> lookup("BB5.wav")
    82

    >>> lookup("fooC4_bar.wav")
    60

    >>> lookup("fooC-1_bar.wav")
    0

    >>> lookup("fooC_-1_bar.wav")
    0

    """
    re_sharp = re.compile(r"([A-Ga-g]#)[^\d\w]*?[_-]*?(-?\d)")
    re_flat = re.compile(r"([A-Ga-g][bB])[^\d\w]*?[_-]*?(-?\d)")
    re_pure = re.compile(r"([A-Ga-g])[^\d\w]*?[_-]*?(-?\d)")

    lut = generate_lut()

    for pattern in [re_sharp, re_flat, re_pure]:
        m = pattern.search(s)
        if m is not None:
            note = "{:s}{:s}".format(m.group(1), m.group(2)).lower()
            return lut[note]
    else:
        raise RuntimeError("Could not match note: {:s}".format(s))

if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("name", nargs="?", help="name to parse")
    parser.add_argument("-t", "--doctest", default=False, action="store_true", help="run doctest")
    parser.add_argument("-v", "--verbose", default=False, action="store_true", help="run doctest (verbose)")
    args = parser.parse_args()

    if args.doctest or args.verbose:
        doctest.testmod(verbose=args.verbose)
        sys.exit()

    if not args.name:
        name = input("Write a string to extract midi note from: ").strip()
    else:
        name = args.name

    print(lookup(name))
