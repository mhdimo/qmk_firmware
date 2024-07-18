/* Copyright 2023 RephlexZero (@RephlexZero)
SPDX-License-Identifier: GPL-2.0-or-later */
#include <math.h>
#include "scanfunctions.h"
#include "util.h"

// const double lut_a = 16654600.6755; // Lekker
// const double lut_b = -0.00955994866577;
// const double lut_c = -1278.75103145;
// const double lut_d = 16652478.4163;

// const double lut_a = 0.200347177016; // Latenpow
// const double lut_b = 0.00955994866154;
// const double lut_c = 6.01110636956;
// const double lut_d = 1966.74076381;

/* Equation parameters for the sensor-magnet linearity mapping */
const double lut_a = 16654600.6755; // Lekker
const double lut_b = -0.00955994866577;
const double lut_c = -1278.75103145;
const double lut_d = 16652478.4163;

uint16_t distance_to_adc(uint8_t distance) {

    double intermediate = lut_a * exp(lut_b * distance + lut_c) + lut_d;
    uint16_t adc = (uint16_t) MAX(0, MIN(intermediate, 4095));
    return adc;
}

uint8_t adc_to_distance(uint16_t adc) {
    if (adc <= lut_d) {
        return 0;
    }
    double intermediate = (log((adc - lut_d) / lut_a) - lut_c) / lut_b;
    uint8_t distance = (uint8_t) MAX(0, MIN(intermediate, 255));
    return distance;
}

uint8_t lut[ADC_RESOLUTION_MAX] = {0};

void generate_lut(void) {
    for (uint16_t i = 0; i < ADC_RESOLUTION_MAX; i++) {
        lut[i] = adc_to_distance(i);
    }
}
