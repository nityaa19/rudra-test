#include <stdint.h>
#include <stdio.h>
#include <math.h>  // For rounding if needed

// Function to parse SBUS channel data from a buffer
uint16_t parse_channel(uint8_t buff[], int channel) {
    uint16_t mask = 0x7FF; // 11-bit mask
    uint16_t value = 0;

    // Calculate the channel value based on the SBUS frame
    switch (channel) {
        case 0:
            value = ((buff[1] | (buff[2] << 8)) & mask);
            break;
        case 1:
            value = ((buff[2] >> 3 | (buff[3] << 5)) & mask);
            break;
        // Add more cases if needed for other channels
        default:
            value = 0;
            break;
    }
    return value;
}

// Interpolation function to map SBUS channel value to Sabertooth command
uint8_t interpolate(uint16_t channelValue) {
    // Map SBUS channel value (0-2047) to Sabertooth command (0-255)
    int mappedValue = (int)round(channelValue / 2047.0 * 254 - 127);
    uint8_t sabertoothValue;

    // Convert to Sabertooth format
    if (mappedValue >= 0) {
        sabertoothValue = (uint8_t)mappedValue;
    } else {
        sabertoothValue = 128 - (uint8_t)abs(mappedValue);
    }

    return sabertoothValue;
}

// Function to send command to Sabertooth motor driver
void send_to_sabertooth(uint8_t value) {
    // Send data to Sabertooth via serial port
    // Replace with your specific serial communication function
    printf("Sending value to Sabertooth: %u\n", value);
    // Example: serial_write(value);  // Replace with actual function
}

// Setup serial communication (implementation depends on your platform)
void setup_serial() {
    // Initialize serial port for communication
    // Replace with your specific initialization code
    printf("Serial port initialized.\n");
    // Example: serial_init(9600);
}

// Function to read SBUS data (implementation depends on your platform)
void read_sbus(uint8_t buff[]) {
    // Read SBUS data into buffer
    // Replace with your specific read function
    // Example: serial_read(buff, 23);
    printf("SBUS data read into buffer.\n");
}

int main() {
    // Initialize serial communication
    setup_serial();

    // Buffer for SBUS data (23 bytes)
    uint8_t sbusBuffer[23];

    while (1) {
        // Read SBUS data
        read_sbus(sbusBuffer);

        // Parse channel 0 (adjust as needed)
        uint16_t channelValue = parse_channel(sbusBuffer, 0);

        // Interpolate and format for Sabertooth
        uint8_t sabertoothValue = interpolate(channelValue);

        // Send command to Sabertooth Motor Driver
        send_to_sabertooth(sabertoothValue);

        // Optional: Add a delay to prevent flooding
        // Replace with appropriate delay function for your platform
        // Example: delay(50);
    }

    return 0;
}
