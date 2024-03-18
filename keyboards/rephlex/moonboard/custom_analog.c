#include "config.h"
#include "custom_analog.h"

#define ADC_SAMPLING_RATE ADC_SMPR_SMP_1P5

adcsample_t sample_buffer[3][2];
adcsample_t *samples1 = sample_buffer[0];
adcsample_t *samples2 = sample_buffer[1];
adcsample_t *samples4 = sample_buffer[2];
static union ConversionCompleteFlags conversion_complete_flags = {0};

static const ADCConversionGroup CG_ADC1 = {
    .circular     = false,
    .num_channels = 2,
    .end_cb = ADC1_callback,
    .cfgr = ADC_CFGR_CONT | ADC_RESOLUTION,
    .smpr = {
        ADC_SMPR1_SMP_AN3(ADC_SAMPLING_RATE) |
        ADC_SMPR1_SMP_AN4(ADC_SAMPLING_RATE)
    },
    .sqr = {
        ADC_SQR1_SQ1_N(ADC_CHANNEL_IN3) | ADC_SQR1_SQ2_N(ADC_CHANNEL_IN4)
    }
};
static const ADCConversionGroup CG_ADC2 = {
    .circular     = false,
    .num_channels = 2,
    .end_cb = ADC2_callback,
    .cfgr = ADC_CFGR_CONT | ADC_RESOLUTION,
    .smpr = {
        ADC_SMPR1_SMP_AN3(ADC_SAMPLING_RATE) |
        ADC_SMPR1_SMP_AN4(ADC_SAMPLING_RATE)
    },
    .sqr = {
        ADC_SQR1_SQ1_N(ADC_CHANNEL_IN3) | ADC_SQR1_SQ2_N(ADC_CHANNEL_IN4)
    }
};
static const ADCConversionGroup CG_ADC4 = {
    .circular     = false,
    .num_channels = 2,
    .end_cb = ADC4_callback,
    .cfgr = ADC_CFGR_CONT | ADC_RESOLUTION,
    .smpr = {
        ADC_SMPR1_SMP_AN3(ADC_SAMPLING_RATE) |
        ADC_SMPR1_SMP_AN4(ADC_SAMPLING_RATE)
    },
    .sqr = {
        ADC_SQR1_SQ1_N(ADC_CHANNEL_IN3) | ADC_SQR1_SQ2_N(ADC_CHANNEL_IN4)
    }
};

void ADC1_callback(ADCDriver *adcp) {
    if(adcIsBufferComplete(adcp)) {
        conversion_complete_flags.bits.adc_1 = 1;
    }
}

void ADC2_callback(ADCDriver *adcp) {
    if(adcIsBufferComplete(adcp)) {
        conversion_complete_flags.bits.adc_2 = 1;
    }
}

void ADC4_callback(ADCDriver *adcp) {
    if(adcIsBufferComplete(adcp)) {
        conversion_complete_flags.bits.adc_4 = 1;
    }
}

void init_custom_analog(void) {
    palSetPadMode(GPIOA, 2, PAL_MODE_INPUT_ANALOG);
    palSetPadMode(GPIOA, 3, PAL_MODE_INPUT_ANALOG);
    palSetPadMode(GPIOA, 6, PAL_MODE_INPUT_ANALOG);
    palSetPadMode(GPIOA, 7, PAL_MODE_INPUT_ANALOG);
    palSetPadMode(GPIOB, 12, PAL_MODE_INPUT_ANALOG);
    palSetPadMode(GPIOB, 14, PAL_MODE_INPUT_ANALOG);
    adcStart(&ADCD1, NULL);
    adcStart(&ADCD2, NULL);
    adcStart(&ADCD4, NULL);
}

void start_adc_conversions(void) {
    adcStartConversion(&ADCD1, &CG_ADC1, samples1, 2);
    adcStartConversion(&ADCD2, &CG_ADC2, samples2, 2);
    adcStartConversion(&ADCD4, &CG_ADC4, samples4, 2);
}

bool check_adc_conversion_complete(uint8_t ADC) {
    uint8_t flags = conversion_complete_flags.value;
    conversion_complete_flags.value &= ~(1 << (ADC - 1));
    return flags & (1 << (ADC - 1));
}
