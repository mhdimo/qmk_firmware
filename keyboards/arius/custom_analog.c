/* Copyright 2023 RephlexZero (@RephlexZero)
SPDX-License-Identifier: GPL-2.0-or-later */
#include "custom_analog.h"
#include "print.h"
#include "multiplexer.h"

// Define the global ADC manager instance
ADCManager adcManager;

static void adcCompleteCallback(ADCDriver *adcp) {
    (void)adcp; // Unused parameter
    osalSysLockFromISR();
    adcManager.completedConversions++;
    if (adcManager.completedConversions == 3) {
        osalThreadResumeI(&adcManager.waitingThread, MSG_OK); // Resume the waiting thread
        adcManager.waitingThread = NULL; // Reset the waitingThread reference
    }
    osalSysUnlockFromISR();
}

void adcErrorCallback(ADCDriver *adcp, adcerror_t err) {
    (void)adcp; // Unused parameter
    osalSysLockFromISR();
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
            uprintf("ADC ERROR: Unknown error.\n");
            break;
    }
    osalSysUnlockFromISR();
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
        ADC_SMPR1_SMP_AN3(ADC_SAMPLING_TIME) | ADC_SMPR1_SMP_AN4(ADC_SAMPLING_TIME),
    },
    .sqr          = {
        ADC_SQR1_SQ1_N(ADC_CHANNEL_IN3) | ADC_SQR1_SQ2_N(ADC_CHANNEL_IN4),
    }
};

void initADCGroups(ADCManager *adcManager) {
    adcManager->completedConversions = 0;
    adcManager->waitingThread = NULL;
    for (uint8_t i = 0; i < MUXES; i++) {
        palSetLineMode(mux_pins[i], PAL_MODE_INPUT_ANALOG);
    }
    adcStart(&ADCD1, NULL); // Start ADC1
    adcStart(&ADCD2, NULL); // Start ADC2
    adcStart(&ADCD4, NULL); // Start ADC4
}

msg_t adcStartAllConversions(ADCManager *adcManager) {
    msg_t msg;
    osalSysLock();
    adcManager->completedConversions = 0;

    // Save the current thread's pointer to resume later
    adcManager->waitingThread = chThdGetSelfX();

    adcStartConversionI(&ADCD1, &adcConversionGroup, adcManager->sampleBuffer1, 1);
    adcStartConversionI(&ADCD2, &adcConversionGroup, adcManager->sampleBuffer2, 1);
    adcStartConversionI(&ADCD4, &adcConversionGroup, adcManager->sampleBuffer4, 1);

    // Suspend the current thread until all conversions are complete
    msg = osalThreadSuspendS(&adcManager->waitingThread);
    osalSysUnlock();
    return msg;
}

adcsample_t getADCSample(const ADCManager *adcManager, uint8_t muxIndex) {
    switch (muxIndex) {
        case 0:
            return adcManager->sampleBuffer1[0];
        case 1:
            return adcManager->sampleBuffer1[1];
        case 2:
            return adcManager->sampleBuffer2[0];
        case 3:
            return adcManager->sampleBuffer2[1];
        case 4:
            return adcManager->sampleBuffer4[1]; // SWAPPED!!!
        case 5:
            return adcManager->sampleBuffer4[0];
        default:
            return 0; // Invalid index
    }
}
