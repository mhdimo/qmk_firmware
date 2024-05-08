/* Copyright 2023 RephlexZero (@RephlexZero)
SPDX-License-Identifier: GPL-2.0-or-later */
#include <stdint.h>
#include <stdbool.h>
#include <hal.h>
#include <ch.h>

#pragma once

msg_t adcStartAllConversions(void);

adcsample_t sampleBuffer1[2];
adcsample_t sampleBuffer2[2];
adcsample_t sampleBuffer4[2];

void initADCGroups(void);
