#include "custom_analog.h"
#include "gpio.h"
#include "print.h"

#define ADC_SAMPLING_RATE ADC_SMPR_SMP_1P5

static ADCConfig   adcCfg = {};

static const ADCConversionGroup adcConversionGroup = {
    .circular     = FALSE,
    .num_channels = (uint16_t)(1),
    .cfgr = ADC_CFGR_RES_12BITS,
    .smpr = {ADC_SMPR1_SMP_AN3(ADC_SAMPLING_RATE) | ADC_SMPR1_SMP_AN4(ADC_SAMPLING_RATE)},
    .sqr = {ADC_SQR1_SQ1_N(ADC_CHANNEL_IN3)},
    .end_cb = NULL,
    .error_cb = NULL,
};

void adc_init() {
    uprintf("adc_init\n");
    palSetLineMode(A2, PAL_MODE_INPUT_ANALOG);
    adcStart(&ADCD1, &adcCfg);
}

void adc_start() {
    adcStartConversion(&ADCD1, &adcConversionGroup, sampleBuffer, 1);
}

bool is_adc_conversion_done(void) {
    switch (ADCD1.state) {
        case ADC_COMPLETE:
            return true;
        case ADC_ERROR:
            return true;
        case ADC_UNINIT:
            return false;
        case ADC_READY:
            return false;
        case ADC_ACTIVE:
            return false;
        default:
            return false;
    }
}
