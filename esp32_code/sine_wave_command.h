#ifndef SINE_WAVE_COMMAND_H
#define SINE_WAVE_COMMAND_H
#include "wave_gen.h"
void run_sine_gen_command(Signal_Generator *sig)
{
    uint8_t byte_freq[4] = {0};
    uint8_t atten = 0;
    int freq = 0;
    Serial.readBytes(byte_freq, 4);
    freq += ((int)(byte_freq[0]));
    freq += (((int)(byte_freq[1])) << 8);
    freq += (((int)(byte_freq[2])) << 16);
    freq += (((int)(byte_freq[3])) << 24);
    atten = Serial.read();
    sig->create_continuous_cosine_wave(freq, atten);
}
#endif