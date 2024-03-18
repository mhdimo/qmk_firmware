#include <stdbool.h>
#include <stdint.h>
#include "hal_lld.h"
#include "hal.h"
#include "ch.h"

// Buffer to hold ADC conversion results. Size depends on num_channels.
extern adcsample_t sample_buffer[3][2]; // Adjust the size based on num_channels in CG_ADC1
// Create pointers for each ADC conversion group
extern adcsample_t *samples1;
extern adcsample_t *samples2;
extern adcsample_t *samples4;

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
