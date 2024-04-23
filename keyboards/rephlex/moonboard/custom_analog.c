/* Copyright 2023 RephlexZero (@RephlexZero)
SPDX-License-Identifier: GPL-2.0-or-later */
#include "custom_analog.h"
#include "print.h"
#include "multiplexer.h"

#define ADC_SAMPLING_RATE ADC_SMPR_SMP_2P5

static binary_semaphore_t adcSemaphore;
static int completed_conversions = 0;

void adcCompleteCallback(ADCDriver *adcp) {
    osalSysLockFromISR();
    completed_conversions++;
    if (completed_conversions == 3) {
        chBSemSignalI(&adcSemaphore);
    }
    osalSysUnlockFromISR();
}

void adcErrorCallback(ADCDriver *adcp, adcerror_t err) {
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

static const ADCConversionGroup adcConversionGroup = {
  .circular     = false,
  .num_channels = 2U,
  .end_cb       = adcCompleteCallback,
  .error_cb     = adcErrorCallback,
  .cfgr         = ADC_RESOLUTION,
  .tr1          = ADC_TR_DISABLED,
  .tr2          = ADC_TR_DISABLED,
  .tr3          = ADC_TR_DISABLED,
  .awd2cr       = 0U,
  .awd3cr       = 0U,
  .smpr         = {
    ADC_SMPR1_SMP_AN3(ADC_SAMPLING_RATE) | ADC_SMPR1_SMP_AN4(ADC_SAMPLING_RATE),
  },
  .sqr          = {
    ADC_SQR1_SQ1_N(ADC_CHANNEL_IN3) | ADC_SQR1_SQ2_N(ADC_CHANNEL_IN4),
  }
};

void initADCGroups() {
    for (uint8_t i = 0; i < MUXES; i++) {
        palSetLineMode(mux_pins[i], PAL_MODE_INPUT_ANALOG);
    }
    adcStart(&ADCD1, NULL);
    adcStart(&ADCD2, NULL);
    adcStart(&ADCD4, NULL);
}

void adcStartAllConversions() {
    chBSemObjectInit(&adcSemaphore, true);
    completed_conversions = 0;

    adcStartConversion(&ADCD1, &adcConversionGroup, sampleBuffer1, 1);
    adcStartConversion(&ADCD2, &adcConversionGroup, sampleBuffer2, 1);
    adcStartConversion(&ADCD4, &adcConversionGroup, sampleBuffer4, 1);

    chBSemWait(&adcSemaphore); // Wait here until all conversions signal completion
};

