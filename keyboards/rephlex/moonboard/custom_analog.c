#include "custom_analog.h"
#include "gpio.h"
#include "print.h"
#include "multiplexer.h"

#define ADC_SAMPLING_RATE ADC_SMPR_SMP_1P5

/*static void adc1_callback(ADCDriver *adcp) {
    if (adcp->state != ADC_COMPLETE) {
        return;
    }
    adc_samples[0] = sampleBuffer1[0];
    adc_samples[1] = sampleBuffer1[1];
    adc1_complete = true;
}

static void adc2_callback(ADCDriver *adcp) {
    if (adcp->state != ADC_COMPLETE) {
        return;
    }
    adc_samples[2] = sampleBuffer2[0];
    adc_samples[3] = sampleBuffer2[1];
    adc2_complete = true;
}

static void adc4_callback(ADCDriver *adcp) {
    if (adcp->state != ADC_COMPLETE) {
        return;
    }
    adc_samples[4] = sampleBuffer4[0];
    adc_samples[5] = sampleBuffer4[1];
    adc4_complete = true;
}*/

static void adc_error_callback(ADCDriver *adcp, adcerror_t err) {
    (void)adcp;
    switch (err) {
        case ADC_ERR_DMAFAILURE:
            uprintf("ADC ERROR: DMA failure.\n");
            break;
        case ADC_ERR_OVERFLOW:
            uprintf("ADC ERROR: Overflow.\n");
            break;
        case ADC_ERR_AWD1:
            uprintf("ADC ERROR: Watchdog 1 triggered.\n");
            break;
        case ADC_ERR_AWD2:
            uprintf("ADC ERROR: Watchdog 2 triggered.\n");
            break;
        case ADC_ERR_AWD3:
            uprintf("ADC ERROR: Watchdog 3 triggered.\n");
            break;
        default:
            uprintf("ADC ERROR: Unknown\n");
            break;
    }
}

static const ADCConversionGroup adc1ConversionGroup = {
  .circular     = false,
  .num_channels = 2,
  //.end_cb       = adc1_callback,
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
    ADC_SQR1_SQ1_N(ADC_CHANNEL_IN3) | ADC_SQR1_SQ2_N(ADC_CHANNEL_IN4),
  }
};

static const ADCConversionGroup adc2ConversionGroup = {
  .circular     = false,
  .num_channels = 2,
  //.end_cb       = adc2_callback,
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
    ADC_SQR1_SQ1_N(ADC_CHANNEL_IN3) | ADC_SQR1_SQ2_N(ADC_CHANNEL_IN4),
  }
};

static const ADCConversionGroup adc4ConversionGroup = {
  .circular     = false,
  .num_channels = 2,
  //.end_cb       = adc4_callback,
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
    ADC_SQR1_SQ1_N(ADC_CHANNEL_IN3) | ADC_SQR1_SQ2_N(ADC_CHANNEL_IN4),
  }
};

void adc_init() {
    for (uint8_t i = 0; i < MUXES; i++) {
        palSetLineMode(mux_pins[i], PAL_MODE_INPUT_ANALOG);
    }
    // adc1_complete = false;
    // adc2_complete = false;
    // adc4_complete = false;
    adcStart(&ADCD1, NULL);
    adcStart(&ADCD2, NULL);
    adcStart(&ADCD4, NULL);
}

// void adc_start_conversion() {
//     adc1_complete = false;
//     adc2_complete = false;
//     adc4_complete = false;
//     adcStartConversion(&ADCD1, &adc1ConversionGroup, sampleBuffer1, 2);
//     adcStartConversion(&ADCD2, &adc2ConversionGroup, sampleBuffer2, 2);
//     adcStartConversion(&ADCD4, &adc4ConversionGroup, sampleBuffer4, 2);
// }

msg_t adcConvertAll(ADCDriver *adcp1, const ADCConversionGroup *grpp1, adcsample_t *samples1,
                    ADCDriver *adcp2, const ADCConversionGroup *grpp2, adcsample_t *samples2,
                    ADCDriver *adcp3, const ADCConversionGroup *grpp3, adcsample_t *samples3,
                    size_t depth) {
    msg_t msg;

    osalSysLock();
    osalDbgAssert(adcp1->thread == NULL || adcp2->thread == NULL || adcp3->thread == NULL, "already waiting");
    adcStartConversionI(adcp1, grpp1, samples1, depth);
    adcStartConversionI(adcp2, grpp2, samples2, depth);
    adcStartConversionI(adcp3, grpp3, samples3, depth);
    msg = osalThreadSuspendS(&adcp3->thread);
    osalSysUnlock();
    return msg;
}

void adcGetConversionAll(){
    adcConvertAll(&ADCD1, &adc1ConversionGroup, sampleBuffer1,
                  &ADCD2, &adc2ConversionGroup, sampleBuffer2,
                  &ADCD4, &adc4ConversionGroup, sampleBuffer4, 2);
}
