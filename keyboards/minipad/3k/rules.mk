# Path: keyboards/moonpad/2k/rules.mk

SRC += analog.c
SRC += matrix.c
SRC += lut.c
SRC += scanFunctions.c

CUSTOM_MATRIX = lite

LTO_ENABLE = yes
OPT = 3

