/* Copyright 2023 RephlexZero (@RephlexZero)
SPDX-License-Identifier: GPL-2.0-or-later */
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "quantum.h"
#include "custom_analog.h"
#include "lut.h"
#include "multiplexer.h"
#include "scanfunctions.h"
#include <ch.h>
#include <hal.h>
#include "gpio.h"

// Use the external ADC manager instance
extern ADCManager adcManager;

analog_key_t keys[MATRIX_ROWS][MATRIX_COLS] = {0};
static uint16_t pressedAdcValue = {0};
static uint16_t restAdcValue = {0};

void matrix_init_custom(void) {
    gpio_set_pin_input_high(ENCODER_BUTTON_PIN);
    generate_lut();
    pressedAdcValue = distance_to_adc(255);
    restAdcValue = distance_to_adc(0);
    multiplexer_init();
    initADCGroups(&adcManager);
    get_sensor_offsets();
}

matrix_row_t previous_matrix[MATRIX_ROWS];
bool matrix_scan_custom(matrix_row_t current_matrix[]) {
    memcpy(previous_matrix, current_matrix, sizeof(previous_matrix));
    for (uint8_t channel = 0; channel < MUX_CHANNELS; channel++) {
        uint8_t channel_greycoded = (channel >> 1) ^ channel;
        select_mux(channel_greycoded);
        adcStartAllConversions(&adcManager);
        for (uint8_t mux = 0; mux < MUXES; mux++) {
            uint8_t current_row = mux_index[mux][channel_greycoded].row;
            uint8_t current_col = mux_index[mux][channel_greycoded].col;

            if (current_row == 255 && current_col == 255) continue; // NC mux pin

            analog_key_t *key = &keys[current_row][current_col];
            key->raw = getADCSample(&adcManager, mux);
            key->value = lut[MAX(MIN(key->raw + key->offset, 4095), 0)];
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
        }
    }
    bool encoder_button_pressed = gpio_read_pin(ENCODER_BUTTON_PIN);
    // Encoder button scan
    if (current_matrix[1] & (1 << 14)) {
        if (!encoder_button_pressed) {
            deregister_key(&current_matrix[1], 14);
        }
    } else {
        if (encoder_button_pressed) {
            register_key(&current_matrix[1], 14);
        }
    }
    return memcmp(previous_matrix, current_matrix, sizeof(previous_matrix)) != 0;
}
