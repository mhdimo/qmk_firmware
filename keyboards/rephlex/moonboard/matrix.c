/* Copyright 2023 RephlexZero (@RephlexZero)
SPDX-License-Identifier: GPL-2.0-or-later */
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "lut.h"
#include "multiplexer.h"
#include "scanfunctions.h"
#include "custom_analog.h"

analog_key_t keys[MATRIX_ROWS][MATRIX_COLS] = {0};
uint32_t     matrix_size;
matrix_row_t previous_matrix[MATRIX_ROWS];
static void update_keypress(matrix_row_t *current_row, uint8_t current_col, analog_key_t *key);

void matrix_init_custom(void) {
    init_custom_analog();
    generate_lut();
    multiplexer_init();
    get_sensor_offsets();
    matrix_size = sizeof(previous_matrix);
    wait_ms(100); // Let ADC reach steady state
    get_sensor_offsets();
}

bool matrix_scan_custom(matrix_row_t current_matrix[]) {
    memcpy(previous_matrix, current_matrix, matrix_size);
    for (uint8_t channel = 0; channel < MUX_CHANNELS; channel++) {
        uint8_t channel_greycoded = (channel >> 1) ^ channel;
        select_mux(channel_greycoded);

        start_adc_conversions();
        analog_key_t *key;
        uint8_t       current_row;
        uint8_t       current_col;
        while (check_adc_conversion_complete(1)) {
        }
        // TODO: Translate the sample to ADC pin
        // TODO: Fix logic error of sample indexing
        for (uint8_t sample = 0; sample < 2; sample++) {
            current_row = mux_index[sample][channel_greycoded].row;
            current_col = mux_index[sample][channel_greycoded].col;
            if (current_row == 255 || current_col == 255) continue;     // NC mux pin
            key         = &keys[current_row][current_col];

            key->raw   = samples1[sample];
            key->value = lut[key->raw + key->offset];
            key->value = MIN(key->value * CALIBRATION_RANGE / lut[1100 + key->offset], 255);
            update_keypress(&current_matrix[current_row], current_col, key);
        }

        while (check_adc_conversion_complete(2)) {
        }

        for (uint8_t sample = 2; sample < 4; sample++) {
            current_row = mux_index[sample][channel_greycoded].row;
            current_col = mux_index[sample][channel_greycoded].col;
            if (current_row == 255 || current_col == 255) continue;     // NC mux pin
            key         = &keys[current_row][current_col];

            key->raw   = samples2[sample-2];
            key->value = lut[key->raw + key->offset];
            key->value = MIN(key->value * CALIBRATION_RANGE / lut[1100 + key->offset], 255);
            update_keypress(&current_matrix[current_row], current_col, key);
        }

        while (check_adc_conversion_complete(4)) {
        }

        for (uint8_t sample = 4; sample < 6; sample++) {
            current_row = mux_index[sample][channel_greycoded].row;
            current_col = mux_index[sample][channel_greycoded].col;
            if (current_row == 255 || current_col == 255) continue;     // NC mux pin
            key         = &keys[current_row][current_col];

            key->raw   = samples4[sample-4];
            key->value = lut[key->raw + key->offset];
            key->value = MIN(key->value * CALIBRATION_RANGE / lut[1100 + key->offset], 255);
            update_keypress(&current_matrix[current_row], current_col, key);
        }


    }
    return memcmp(previous_matrix, current_matrix, matrix_size) != 0;
}

static void update_keypress(matrix_row_t *current_row, uint8_t current_col, analog_key_t *key) {
    switch (g_config.mode) {
        case dynamic_actuation:
            matrix_read_cols_dynamic_actuation(current_row, current_col, key);
            break;
        case continuous_dynamic_actuation:
            matrix_read_cols_continuous_dynamic_actuation(current_row, current_col, key);
            break;
        case static_actuation:
            matrix_read_cols_static_actuation(current_row, current_col, key);
            break;
        case flashing:
        default:
            bootloader_jump();
            break;
    }
}
