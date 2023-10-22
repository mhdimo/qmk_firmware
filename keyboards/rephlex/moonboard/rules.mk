# Path: keyboards/rephlex/moonboard/rules.mk
QUANTUM_LIB_SRC += analog.c
SRC += matrix.c lut.c scanfunctions.c multiplexer.c

CUSTOM_MATRIX = lite

OPT = 1

WEAR_LEVELING_DRIVER = embedded_flash
ENCODER_MAP_ENABLE = yes