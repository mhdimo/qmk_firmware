/* Copyright 2023 RephlexZero (@RephlexZero)
SPDX-License-Identifier: GPL-2.0-or-later */
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include "arius.h"
#include "quantum.h"
#include "eeprom.h"
#include "scanfunctions.h"
#include "print.h"
#include "multiplexer.h"
#include "lut.h"

analog_config g_config = {.mode = static_actuation, .actuation_point = 48, .press_sensitivity = 32, .release_sensitivity = 32, .press_hysteresis = 0, .release_hysteresis = 5};

#ifdef BOOTMAGIC_ENABLE
void bootmagic_scan(void) {
//     matrix_scan();
// 
//     uint16_t threshold = distance_to_adc(CALIBRATION_RANGE / 2);
//     uint16_t raw_value = keys[BOOTMAGIC_ROW][BOOTMAGIC_COLUMN].raw;
// 
//     if ((lut_b > 0 && raw_value > threshold) || (lut_b < 0 && raw_value < threshold)) {
//         bootloader_jump();
//     }
}
 #endif


#ifdef DEFERRED_EXEC_ENABLE

#    ifdef DEBUG_ENABLE
deferred_token debug_token;

bool debug_print(void) {
    static char rowBuffer[MATRIX_COLS * 8]; // 8: for 7 characters (" null  " or " 12345  ") + '\0'
    static uint8_t currentRow = 0;
    char *bufferPtr = rowBuffer;

    for (uint8_t col = 0; col < MATRIX_COLS; col++) {
        analog_key_t *key = &keys[currentRow][col];
        if (key->raw) {
            bufferPtr += snprintf(bufferPtr, sizeof(rowBuffer) - (bufferPtr - rowBuffer), "%5d  ", key->value);
        } else {
            bufferPtr += snprintf(bufferPtr, sizeof(rowBuffer) - (bufferPtr - rowBuffer), " null   ");
        }
    }

    uprintf("%s\n", rowBuffer);
    currentRow++;

    if (currentRow >= MATRIX_ROWS) {
        currentRow = 0;
        uprintf("\n");
        return false;
    }

    return true;
}

uint32_t debug_print_callback(uint32_t trigger_time, void *cb_arg) {
    debug_print();
    return 100; // Assuming this is in milliseconds
}
#endif

deferred_token idle_recalibrate_token;
bool           process_record_kb(uint16_t keycode, keyrecord_t *record) {
    extend_deferred_exec(idle_recalibrate_token, 300000);
    return true;
}

uint32_t idle_recalibrate_callback(uint32_t trigger_time, void *cb_arg) {
    get_sensor_offsets();
    return 10000;
}
#endif

void values_load(void) {
    eeconfig_read_kb_datablock(&g_config);
}

void values_save(void) {
    eeconfig_update_kb_datablock(&g_config);
}

void eeconfig_init_kb() {
    values_save();
}

void keyboard_post_init_kb(void) {
#ifdef DEFERRED_EXEC_ENABLE
#    ifdef DEBUG_ENABLE
    debug_token = defer_exec(1000, debug_print_callback, NULL);
#    endif
    idle_recalibrate_token = defer_exec(300000, idle_recalibrate_callback, NULL);
#endif
    values_load();
}

#ifdef VIA_ENABLE
void via_custom_value_command_kb(uint8_t *data, uint8_t length) {
    /* data = [ command_id, channel_id, value_id, value_data ] */
    uint8_t *command_id        = &(data[0]);
    uint8_t *channel_id        = &(data[1]);
    uint8_t *value_id_and_data = &(data[2]);

    if (*channel_id == id_custom_channel) {
        switch (*command_id) {
            case id_custom_set_value: {
                via_config_set_value(value_id_and_data);
                break;
            }
            case id_custom_get_value: {
                via_config_get_value(value_id_and_data);
                break;
            }
            case id_custom_save: {
                values_save();
                break;
            }
            default: {
                /* Unhandled message */
                *command_id = id_unhandled;
                break;
            }
        }
        return;
    }

    /* Return the unhandled state */
    *command_id = id_unhandled;

    /* DO NOT call raw_hid_send(data,length) here, let caller do this */
}

enum via_dynamic_actuation {
    id_mode = 1,
    id_actuation_point,
    id_press_sensitivity,
    id_release_sensitivity,
    id_press_hysteresis,
    id_release_hysteresis,
};

void via_config_set_value(uint8_t *data) {
    /* data = [ value_id, value_data ] */
    uint8_t *value_id   = &(data[0]);
    uint8_t *value_data = &(data[1]);

    switch (*value_id) {
        case id_mode:
            g_config.mode = *value_data;
            break;
        case id_actuation_point:
            g_config.actuation_point = *value_data * 255 / 40;
            break;
        case id_press_sensitivity:
            g_config.press_sensitivity = *value_data * 255 / 40;
            break;
        case id_release_sensitivity:
            g_config.release_sensitivity = *value_data * 255 / 40;
            break;
        case id_press_hysteresis:
            g_config.press_hysteresis = *value_data * 255 / 40;
            break;
        case id_release_hysteresis:
            g_config.release_hysteresis = *value_data * 255 / 40;
            break;
    }
}

void via_config_get_value(uint8_t *data) {
    uint8_t *value_id   = &(data[0]);
    uint8_t *value_data = &(data[1]);

    switch (*value_id) {
        case id_mode:
            *value_data = g_config.mode;
            break;
        case id_actuation_point:
            *value_data = g_config.actuation_point * 40 / 255;
            break;
        case id_press_sensitivity:
            *value_data = g_config.press_sensitivity * 40 / 255;
            break;
        case id_release_sensitivity:
            *value_data = g_config.release_sensitivity * 40 / 255;
            break;
        case id_press_hysteresis:
            *value_data = g_config.press_hysteresis * 40 / 255;
            break;
        case id_release_hysteresis:
            *value_data = g_config.release_hysteresis * 40 / 255;
            break;
    }
}
#endif

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    // If console is enabled, it will print the matrix position and status of each key pressed
#ifdef CONSOLE_ENABLE
    uprintf("KL: kc: 0x%04X, col: %2u, row: %2u, pressed: %u, time: %5u, int: %u, count: %u\n", keycode, record->event.key.col, record->event.key.row, record->event.pressed, record->event.time, record->tap.interrupted, record->tap.count);
#endif
    return true;
}
