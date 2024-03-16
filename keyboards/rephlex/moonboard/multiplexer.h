/* Copyright 2023 RephlexZero (@RephlexZero)
SPDX-License-Identifier: GPL-2.0-or-later */
#include <stdint.h>

uint8_t current_channel;

void multiplexer_init(void);

bool select_mux(uint8_t channel);

typedef struct {
    uint8_t row;
    uint8_t col;
} mux_t;

extern const pin_t mux_pins[MUXES];
extern const uint8_t mux_adcs[MUXES];
extern const pin_t mux_selector_pins[MUX_SELECTOR_BITS];
extern const mux_t mux_index[MUXES][MUX_CHANNELS];
extern const mux_t NC;
