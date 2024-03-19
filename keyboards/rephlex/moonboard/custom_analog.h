#include <stdbool.h>
#include <stdint.h>
#include "hal_lld.h"
#include "hal.h"
#include "ch.h"

// Buffers to hold ADC conversion results. Size depends on num_channels.
adcsample_t sample_buffer1[2];
adcsample_t sample_buffer2[2];
adcsample_t sample_buffer4[2];

static const ADCConversionGroup CG_ADC1;
static const ADCConversionGroup CG_ADC2;
static const ADCConversionGroup CG_ADC4;

void init_custom_analog(void);
void start_adc_conversions(void);
bool check_adc_conversion_complete(uint8_t ADC);

void ADC1_callback(ADCDriver *adcp);
void ADC2_callback(ADCDriver *adcp);
void ADC4_callback(ADCDriver *adcp);

// Define a union to hold the flags
union ConversionCompleteFlags {
    uint8_t value;
    struct {
        unsigned int adc_1 : 1;
        unsigned int adc_2 : 1;
        unsigned int adc_3 : 1;
        unsigned int adc_4 : 1;
        // Add more flags as needed
    } bits;
};
