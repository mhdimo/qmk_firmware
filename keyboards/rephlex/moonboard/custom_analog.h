#include <stdint.h>
#include <stdbool.h>
#include <hal.h>
#include <ch.h>

#pragma once

void adcStartAllConversions(void);

adcsample_t sampleBuffer1[2];
adcsample_t sampleBuffer2[2];
adcsample_t sampleBuffer4[2];

void initADCGroups(void);
