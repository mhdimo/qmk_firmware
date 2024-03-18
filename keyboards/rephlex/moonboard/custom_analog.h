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

extern const ADCConversionGroup CG_ADC1;
extern const ADCConversionGroup CG_ADC2;
extern const ADCConversionGroup CG_ADC4;
