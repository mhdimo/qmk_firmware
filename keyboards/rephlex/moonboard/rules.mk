QUANTUM_LIB_SRC += analog.c
SRC += matrix.c lut.c scanfunctions.c multiplexer.c

MCU_LDSCRIPT = STM32F303xE

CUSTOM_MATRIX = lite

OPT = 2

AUDIO_DRIVER = dac_additive
