/* Copyright 2023 RephlexZero (@RephlexZero)
SPDX-License-Identifier: GPL-2.0-or-later */
#pragma once

#include_next <board.h>

#define STM32_HSE_BYPASS
#undef STM32_HSECLK
#define STM32_HSECLK 16000000U
//#undef STM32F303xC
//#define STM32F303xC
//#define STM32_NO_INIT                       FALSE
//#define STM32_PVD_ENABLE                    FALSE
//#define STM32_PLS                           STM32_PLS_LEV0
//#define STM32_HSI_ENABLED                   TRUE
//#define STM32_LSI_ENABLED                   TRUE
//#undef  STM32_HSE_ENABLED
//#define STM32_HSE_ENABLED                   TRUE
//#define STM32_LSE_ENABLED                   FALSE
//#undef  STM32_SW
//#define STM32_SW                            STM32_SW_PLL
//#undef  STM32_PLLSRC
//#define STM32_PLLSRC                        STM32_PLLSRC_HSE

/*#undef  STM32_HPRE
#define STM32_HPRE                          STM32_HPRE_DIV1
#undef  STM32_PPRE1
#define STM32_PPRE1                         STM32_PPRE1_DIV2
#undef  STM32_PPRE2
#define STM32_PPRE2                         STM32_PPRE2_DIV1*/
//#define STM32_MCOSEL                        STM32_MCOSEL_NOCLOCK
//#define STM32_ADC12PRES                     STM32_ADC12PRES_DIV1
//#define STM32_ADC34PRES                     STM32_ADC34PRES_DIV1
//#define STM32_USART1SW                      STM32_USART1SW_PCLK
//#define STM32_USART2SW                      STM32_USART2SW_PCLK
//#define STM32_USART3SW                      STM32_USART3SW_PCLK
//#define STM32_UART4SW                       STM32_UART4SW_PCLK
//#define STM32_UART5SW                       STM32_UART5SW_PCLK
//#undef  STM32_I2C1SW
//#define STM32_I2C1SW                        STM32_I2C1SW_SYSCLK
//#undef  STM32_I2C2SW
//#define STM32_I2C2SW                        STM32_I2C2SW_SYSCLK
//#define STM32_TIM1SW                        STM32_TIM1SW_PCLK2
//#define STM32_TIM8SW                        STM32_TIM8SW_PCLK2
//#define STM32_RTCSEL                        STM32_RTCSEL_LSI
//#define STM32_USB_CLOCK_REQUIRED            TRUE
//#define STM32_USBPRE                        STM32_USBPRE_DIV1
