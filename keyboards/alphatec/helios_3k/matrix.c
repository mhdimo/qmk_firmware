/* Copyright 2023 RephlexZero (@RephlexZero)
SPDX-License-Identifier: GPL-2.0-or-later */
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "print.h"
#include "quantum.h"
#include "analog.h"
#include "lut.h"
#include "scanfunctions.h"

pin_t         matrix_pins[MATRIX_ROWS][MATRIX_COLS] = DIRECT_PINS;
analog_config g_config                              = {.mode = 1, .actuation_point = 32, .press_sensitivity = 32, .release_sensitivity = 32, .press_hysteresis = 0, .release_hysteresis = 5};
key_t         keys[MATRIX_ROWS][MATRIX_COLS]        = {0};

bool switch_type[MATRIX_ROWS][MATRIX_COLS] = SWITCH_TYPES;

void matrix_init_pins(void) {
    for (uint8_t row = 0; row < MATRIX_ROWS; row++) {
        for (uint8_t col = 0; col < MATRIX_COLS; col++) {
            pin_t pin = matrix_pins[row][col];
            if(pin != NO_PIN) {
                switch (switch_type[row][col]) {
                    case digital_switch:
                        setPinInputHigh(pin);
                        break;
                }
            }
        }
    }
}
void matrix_init_custom(void) {
    matrix_init_pins();
    generate_lut();
    get_sensor_offsets(distance_to_adc(0));
    wait_ms(100); // Let ADC reach steady state
    get_sensor_offsets(distance_to_adc(0));
}

enum analog_key_modes { dynamic_actuation = 0, continuous_dynamic_actuation, static_actuation, flashing };

matrix_row_t previous_matrix[MATRIX_ROWS];
bool         matrix_scan_custom(matrix_row_t current_matrix[]) {
    memcpy(previous_matrix, current_matrix, sizeof(previous_matrix));
    for (uint8_t current_row = 0; current_row < MATRIX_ROWS; current_row++) {
        for (uint8_t current_col = 0; current_col < MATRIX_COLS; current_col++) {
            pin_t pin = matrix_pins[current_row][current_col];
            if (pin == NO_PIN) {
                continue;
            }
            switch (switch_type[current_row][current_col]) {
                case analog_switch:
                    key_t *key = &keys[current_row][current_col];
                    key->value = lut[analogReadPin(pin) + key->offset];
                    key->value = MIN(key->value * CALIBRATION_RANGE / lut[1100 + key->offset], 255);

                    switch (g_config.mode) {
                        case dynamic_actuation:
                            matrix_read_cols_dynamic_actuation(&current_matrix[current_row], current_col, key);
                            break;
                        case continuous_dynamic_actuation:
                            matrix_read_cols_continuous_dynamic_actuation(&current_matrix[current_row], current_col, key);
                            break;
                        case static_actuation:
                            matrix_read_cols_static_actuation(&current_matrix[current_row], current_col, key);
                            break;
                        case flashing:
                        default:
                            bootloader_jump();
                            break;
                    }
                    break;
                case digital_switch:
                    if (readPin(pin)) { // Active low
                        deregister_key(&current_matrix[current_row], current_col);
                    } else {
                        register_key(&current_matrix[current_row], current_col);
                    }
                    break;
            }
        }
    }
    return memcmp(previous_matrix, current_matrix, sizeof(previous_matrix)) != 0;
}