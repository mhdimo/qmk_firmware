#include <stdint.h>
#include <stdbool.h>
#include <hal.h>
#include <ch.h>

void adc_init(void);
// void adc_start_conversion(void);
//adcsample_t adc_samples[6];
//bool adc1_complete, adc2_complete, adc4_complete;
adcsample_t sampleBuffer1[2];
adcsample_t sampleBuffer2[2];
adcsample_t sampleBuffer4[2];
void adcGetConversionAll(void);
