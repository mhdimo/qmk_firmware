#include "custom_analog.h"
#include "gpio.h"
#include "print.h"

#define ADC_SAMPLING_RATE ADC_SMPR_SMP_1P5
static adcsample_t sampleBuffer[1];

static void adc_callback(ADCDriver *adcp) {
    if (adcp->state != ADC_COMPLETE) {
        return;
    }
    adc1_sample = sampleBuffer[0];
    sample_complete = true;
}

static void adc_error_callback(ADCDriver *adcp, adcerror_t err) {
    (void)adcp;
    uprintf("ADC error: %ld\n", err);
}

static const ADCConversionGroup adcConversionGroup = {
  .circular     = false,
  .num_channels = 1,
  .end_cb       = adc_callback,
  .error_cb     = adc_error_callback,
  .cfgr         = ADC_CFGR_CONT,
  .tr1          = ADC_TR_DISABLED,
  .tr2          = ADC_TR_DISABLED,
  .tr3          = ADC_TR_DISABLED,
  .awd2cr       = 0U,
  .awd3cr       = 0U,
  .smpr         = {
    ADC_SMPR1_SMP_AN3(ADC_SMPR_SMP_1P5) | ADC_SMPR1_SMP_AN4(ADC_SMPR_SMP_1P5),
  },
  .sqr          = {
    ADC_SQR1_SQ1_N(ADC_CHANNEL_IN3)
  }
};

void adc_init() {
    palSetLineMode(A2, PAL_MODE_INPUT_ANALOG);
    adcStart(&ADCD1, NULL);
}

void adc_start_conversion() {
    sample_complete = false;
    adcStartConversion(&ADCD1, &adcConversionGroup, sampleBuffer, 1);
}
