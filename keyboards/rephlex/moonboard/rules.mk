# Path: keyboards/rephlex/moonboard/rules.mk
QUANTUM_LIB_SRC += analog.c
SRC += matrix.c lut.c scanfunctions.c

CUSTOM_MATRIX = lite

OPT = 1

EEPROM_DRIVER = wear_leveling
WEAR_LEVELING_DRIVER = embedded_flash
