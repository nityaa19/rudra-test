#include <stdint.h>
#include <stdio.h>
#include <math.h>  // For rounding if needed

// Function prototypes
uint16_t parse_channel(uint8_t buff[], int channel);
uint8_t interpolate(uint16_t channelValue);
void send_to_sabertooth(uint8_t value);
void setup_serial();
void read_sbus(uint8_t buff[]);
