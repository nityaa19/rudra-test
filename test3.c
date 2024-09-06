#include <stdint.h>
#include <stdio.h>
#include <stdlib.h> // For exit()

// Function prototypes
FILE* open_file(const char* filename, const char* mode);
void close_file(FILE* file);
void read_SBUS(uint8_t* buffer, size_t size, FILE* file);
uint16_t* parse_buffer(uint8_t* buff);
int interpolation(uint16_t channel);
void write_to_SB(FILE* file, const char* format, int value);

int main(int argc, char** argv) {
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <SBUS_port> <Sabertooth_port>\n", argv[0]);
        return 1;
    }

    char *port_name_1 = argv[1]; // SBUS 
    char *port_name_2 = argv[2]; // Sabertooth1

    FILE *sbus; 
    FILE *sabertooth;

    // to store sbus packets
    uint8_t sbus_packet[25]; // Ensure the buffer size matches the expected packet size

    // to store value of individual RC channel
    uint16_t *channel;

    // PWM value after interpolation 
    int pwm;

    // Opening serial port for communication with Sabertooth and SBUS
    sbus = open_file(port_name_1, "rb");
    sabertooth = open_file(port_name_2, "w+");

    if (sbus == NULL || sabertooth == NULL) {
        perror("Error opening file");
        exit(1);
    }

    // Read data from RC transmitter using SBUS
    read_SBUS(sbus_packet, sizeof(sbus_packet), sbus);

    // Parse SBUS packet
    channel = parse_buffer(sbus_packet);

    // Get PWM range for Sabertooth 1
    pwm = interpolation(channel[0]);

    // Write data to Sabertooth 1
    write_to_SB(sabertooth, "%d", pwm);

    // Closing all serial ports
    close_file(sbus);
    close_file(sabertooth);

    return 0;
}

// Placeholder implementations for missing functions

FILE* open_file(const char* filename, const char* mode) {
    return fopen(filename, mode);
}

void close_file(FILE* file) {
    if (file) fclose(file);
}

void read_SBUS(uint8_t* buffer, size_t size, FILE* file) {
    fread(buffer, 1, size, file);
}

uint16_t* parse_buffer(uint8_t* buff) {
    static uint16_t channel[16];
    uint16_t mask = 0x7FF;

    // Parsing channels from the buffer
    channel[0]  = ((buff[1] | (buff[2] << 8)) & mask);
    channel[1]  = ((buff[2] >> 3 | (buff[3] << 5)) & mask);
    channel[2]  = ((buff[3] >> 6 | (buff[4] << 2) | (buff[5] << 10)) & mask);
    channel[3]  = ((buff[5] >> 1 | (buff[6] << 7)) & mask);
    channel[4]  = ((buff[6] >> 4 | (buff[7] << 4)) & mask);
    channel[5]  = ((buff[7] >> 7 | (buff[8] << 1) | (buff[9] << 9)) & mask);
    channel[6]  = ((buff[9] >> 2 | (buff[10] << 6)) & mask);
    channel[7]  = ((buff[10] >> 5 | (buff[11] << 3)) & mask);
    channel[8]  = ((buff[12] | (buff[13] << 8)) & mask);
    channel[9]  = ((buff[13] >> 3 | (buff[14] << 5)) & mask);
    channel[10] = ((buff[14] >> 6 | (buff[15] << 2) | (buff[16] << 10)) & mask);
    channel[11] = ((buff[16] >> 1 | (buff[17] << 7)) & mask);
    channel[12] = ((buff[17] >> 4 | (buff[18] << 4)) & mask);
    channel[13] = ((buff[18] >> 7 | (buff[19] << 1) | (buff[20] << 9)) & mask);
    channel[14] = ((buff[20] >> 2 | (buff[21] << 6)) & mask);
    channel[15] = ((buff[21] >> 5 | (buff[22] << 3)) & mask);

    return channel;
}

int interpolation(uint16_t channel) {
    // Example interpolation: map 0-2047 to 0-255
    return (int)round(channel * 255.0 / 2047.0);
}

void write_to_SB(FILE* file, const char* format, int value) {
    fprintf(file, format, value);
}
