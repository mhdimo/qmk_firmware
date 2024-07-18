/* Copyright 2023 RephlexZero (@RephlexZero)
SPDX-License-Identifier: GPL-2.0-or-later */
#include "quantum.h"
#include "multiplexer.h"

const pin_t mux_pins[MUXES] = MUX_PINS;
const pin_t mux_selector_pins[MUX_SELECTOR_BITS] = MUX_SELECTOR_PINS;

void multiplexer_init(void) {
    for (uint8_t i = 0; i < MUX_SELECTOR_BITS; i++) {
        pin_t pin = mux_selector_pins[i];
        setPinOutput(pin);
    }
}

bool select_mux(uint8_t channel) {
    if (channel > MUX_CHANNELS) return 0;
    for (uint8_t i = 0; i < MUX_SELECTOR_BITS; i++) {
        writePin(mux_selector_pins[i], channel & (1 << i));
    }
    current_channel = channel;
    return 1;
}

const mux_t NC = {255,255}; // A coord with a Null pin (from JSON)
const mux_t mux_index[MUXES][MUX_CHANNELS] = {
    {{2,1},NC,{0,1},NC,{1,1},{1,0},{0,0},{2,0},{3,0},{4,0},NC,{5,0},NC,{5,1},{3,1},{4,1}},
    {{2,3},{2,4},{0,3},NC,{1,3},{1,2},{0,2},{2,2},NC,{3,2},NC,{5,2},NC,{4,2},{3,3},{4,3}},
    {{1,6},{3,6},{2,5},{4,7},{0,4},{0,5},{1,5},{1,4},{3,4},{4,4},NC,{5,3},NC,{4,5},{3,5},{4,6}},
    {{2,8},{2,9},{0,8},{1,9},{0,7},{0,6},{1,8},{2,7},{3,7},{4,8},{2,6},{4,9},{1,7},{3,8},{3,9},{4,10}},
    {{0,11},{1,12},{2,12},{2,11},{0,10},{0,9},{1,11},{2,10},{3,10},{5,4},{3,11},{5,5},{1,10},{4,11},{3,12},{5,6}},
    {{2,15},{0,15},{1,15},{0,14},{3,14},{2,14},{1,14},{0,13},{5,7},{2,13},{5,8},{1,13},{5,9},{0,12},{3,15},{4,12}}
};
