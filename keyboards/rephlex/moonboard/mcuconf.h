/* Copyright 2023 RephlexZero (@RephlexZero)
SPDX-License-Identifier: GPL-2.0-or-later */
#pragma once

#include_next <mcuconf.h>

#undef STM32_ADC_USE_ADC1
#define STM32_ADC_USE_ADC1          TRUE

#undef STM32_ADC_USE_ADC2
#define STM32_ADC_USE_ADC2          TRUE

#undef STM32_ADC_USE_ADC3
#define STM32_ADC_USE_ADC3          TRUE

#undef STM32_ADC_USE_ADC4
#define STM32_ADC_USE_ADC4          TRUE

#undef STM32_I2C_USE_I2C1
#define STM32_I2C_USE_I2C1          TRUE

#undef STM32_PWM_USE_ADVANCED
#define STM32_PWM_USE_ADVANCED      TRUE

#undef STM32_PWM_USE_TIM1
#define STM32_PWM_USE_TIM1          TRUE

#undef STM32_DAC_USE_DAC1_CH1
#define STM32_DAC_USE_DAC1_CH1      TRUE
#undef STM32_DAC_USE_DAC1_CH2
#define STM32_DAC_USE_DAC1_CH2      TRUE
#undef STM32_GPT_USE_TIM6
#define STM32_GPT_USE_TIM6          TRUE

#define STM32_HSE_ENABLED           TRUE
#undef  STM32_PREDIV_VALUE
#define STM32_PREDIV_VALUE          2
#undef  STM32_PLLMUL_VALUE
#define STM32_PLLMUL_VALUE          9
#undef  STM32_USBPRE
#define STM32_USBPRE                STM32_USBPRE_DIV1P5
#undef  STM32_I2C1SW
#define STM32_I2C1SW                STM32_I2C1SW_SYSCLK
#undef  STM32_PPRE1
#define STM32_PPRE1                 STM32_PPRE1_DIV2
#undef  STM32_PPRE2
#define STM32_PPRE2                 STM32_PPRE2_DIV1

#define STM32_NO_INIT               FALSE
#define STM32_PVD_ENABLE            FALSE
#define STM32_PLS                   STM32_PLS_LEV0
#define STM32_SW                    STM32_SW_PLL
#define STM32_PLLSRC                STM32_PLLSRC_HSE
