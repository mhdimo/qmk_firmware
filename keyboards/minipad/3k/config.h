/* Copyright 2023 RephlexZero (@RephlexZero)
SPDX-License-Identifier: GPL-2.0-or-later */
#pragma once

#ifdef VIA_ENABLE
#define VIA_EEPROM_CUSTOM_CONFIG_SIZE 6
#define DYNAMIC_KEYMAP_LAYER_COUNT 1
#define VIA_FIRMWARE_VERSION 1
#endif

#define DEBOUNCE 0
/*#define DEBUG_MATRIX*/
#define DEBUG_MATRIX_SCAN_RATE

#define ADC_RESOLUTION 12

#define CALIBRATION_RANGE 255

#define MATRIX_ROWS 1
#define MATRIX_COLS 3
#define MATRIX_PINS {{ GP28, GP27, GP26 }}

#define TAPPING_TERM 135