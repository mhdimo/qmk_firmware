/* Copyright 2023 RephlexZero (@RephlexZero)
SPDX-License-Identifier: GPL-2.0-or-later */
#pragma once

#define MATRIX_ROWS 6
#define MATRIX_COLS 18

#define MUXES 6
#define MUX_PINS { A2, A3, A5, A7, B12, B14 }
#define MUX_SELECTOR_BITS 4
#define MUX_SELECTOR_PINS { B5, B4, B3, A15 }

#define MUX_CHANNELS ( 1 << MUX_SELECTOR_BITS )

#define EECONFIG_KB_DATA_SIZE 6

#define DEBUG_ENABLE
#define DEBUG_MATRIX_SCAN_RATE

#define ADC_RESOLUTION      ADC_CFGR_RES_12BITS
#define ADC_SAMPLING_TIME  ADC_SMPR_SMP_1P5
#define ADC_RESOLUTION_MAX  1 << 12

#define CALIBRATION_RANGE 255


