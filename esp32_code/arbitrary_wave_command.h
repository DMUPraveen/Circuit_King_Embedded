#ifndef ARBITRARY_WAVE_COMMAND_H
#define ARBITRARY_WAVE_COMMAND_H
#include "wave_gen.h"
#define ARBITRARY_WAVE_DATA_SIZE = 2048;
void run_arbitrary_wave_command(Signal_Generator *sig)
{
    // uint8_t command = Serial.read();
    uint8_t byte_freq[4] = {0};
    uint8_t byte_size[4] = {0};
    int freq = 0;
    int size = 0;
    Serial2.readBytes(byte_freq, 4);
    freq += ((int)(byte_freq[0]));
    freq += (((int)(byte_freq[1])) << 8);
    freq += (((int)(byte_freq[2])) << 16);
    freq += (((int)(byte_freq[3])) << 24);
    Serial2.readBytes(byte_size, 4);
    size += ((int)(byte_size[0]));
    size += (((int)(byte_size[1])) << 8);
    size += (((int)(byte_size[2])) << 16);
    size += (((int)(byte_size[3])) << 24);
    Serial2.readBytes(global_buffer, 2048);
    // Serial.print("freq");
    // Serial.print(freq);
    // Serial.println();
    // Serial.print("size");
    // Serial.print(size);
    // Serial.println();
    arbitrary_wave_start(global_buffer, size, sig, freq);
}
#endif