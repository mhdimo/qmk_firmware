#include "config.h"
#include "custom_analog.h"

#define ADC_SAMPLING_RATE ADC_SMPR_SMP_1P5

adcsample_t sample_buffer[3][2];
adcsample_t *samples1 = sample_buffer[0];
adcsample_t *samples2 = sample_buffer[1];
adcsample_t *samples4 = sample_buffer[2];

const ADCConversionGroup CG_ADC1 = {
    .circular     = false,
    .num_channels = 2,
    .cfgr = ADC_CFGR_CONT | ADC_RESOLUTION,
    .smpr = {
        ADC_SMPR1_SMP_AN3(ADC_SAMPLING_RATE) |
        ADC_SMPR1_SMP_AN4(ADC_SAMPLING_RATE)
    },
    .sqr = {
        ADC_SQR1_SQ1_N(ADC_CHANNEL_IN3) | ADC_SQR1_SQ2_N(ADC_CHANNEL_IN4)
    }
};
const ADCConversionGroup CG_ADC2 = {
    .circular     = false,
    .num_channels = 2,
    .cfgr = ADC_CFGR_CONT | ADC_RESOLUTION,
    .smpr = {
        ADC_SMPR1_SMP_AN3(ADC_SAMPLING_RATE) |
        ADC_SMPR1_SMP_AN4(ADC_SAMPLING_RATE)
    },
    .sqr = {
        ADC_SQR1_SQ1_N(ADC_CHANNEL_IN3) | ADC_SQR1_SQ2_N(ADC_CHANNEL_IN4)
    }
};
const ADCConversionGroup CG_ADC4 = {
    .circular     = false,
    .num_channels = 2,
    .cfgr = ADC_CFGR_CONT | ADC_RESOLUTION,
    .smpr = {
        ADC_SMPR1_SMP_AN3(ADC_SAMPLING_RATE) |
        ADC_SMPR1_SMP_AN4(ADC_SAMPLING_RATE)
    },
    .sqr = {
        ADC_SQR1_SQ1_N(ADC_CHANNEL_IN3) | ADC_SQR1_SQ2_N(ADC_CHANNEL_IN4)
    }
};
