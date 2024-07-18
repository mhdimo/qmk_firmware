/* Copyright 2023 RephlexZero (@RephlexZero)
SPDX-License-Identifier: GPL-2.0-or-later */
#pragma once

#define HAL_USE_ADC TRUE
#define ADC_USE_WAIT TRUE

#define HAL_USE_I2C TRUE
#define HAL_USE_PWM TRUE

#define HAL_USE_DAC TRUE
#define HAL_USE_GPT TRUE

#include_next <halconf.h>
