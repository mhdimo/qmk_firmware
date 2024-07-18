#QUANTUM_LIB_SRC += analog.c
SRC += matrix.c lut.c scanfunctions.c multiplexer.c custom_analog.c

MCU_LDSCRIPT = STM32F303xB

CUSTOM_MATRIX = lite

OPT = 2

AUDIO_DRIVER = dac_basic

WEAR_LEVELING_DRIVER = embedded_flash

