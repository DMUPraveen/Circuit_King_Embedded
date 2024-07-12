
#ifndef WAVEGEN_H
#define WAVEGEN_H
#include "driver/dac_cosine.h"

#include "driver/dac_continuous.h"
const uint8_t attenuation_array[] = {DAC_COSINE_ATTEN_DB_0, DAC_COSINE_ATTEN_DB_6, DAC_COSINE_ATTEN_DB_12, DAC_COSINE_ATTEN_DB_18};

bool started_cont = false;
dac_continuous_handle_t cont_handle_global;
uint8_t global_buffer[2048];
void arbitrary_wave_stop();

struct Signal_Generator
{
    dac_cosine_handle_t chan0_handle;
    dac_cosine_config_t cos0_cfg;
    bool started_cosine = false;

    bool create_continuous_cosine_wave(int freq_hz, uint8_t attenuation)
    {
        arbitrary_wave_stop();
        stop_continuous_cosine_wave();
        if (freq_hz < 130 || freq_hz > 500000)
        {
            return false;
        }
        if (attenuation > 3)
        {
            return false;
        }
        cos0_cfg.chan_id = DAC_CHAN_0;
        cos0_cfg.freq_hz = freq_hz;
        cos0_cfg.clk_src = DAC_COSINE_CLK_SRC_DEFAULT;
        cos0_cfg.atten = (dac_cosine_atten_t)attenuation_array[attenuation];
        cos0_cfg.phase = DAC_COSINE_PHASE_0;
        cos0_cfg.offset = 0;
        dac_cosine_new_channel(&cos0_cfg, &chan0_handle);
        delay(10);
        dac_cosine_start(chan0_handle);
        delay(10);
        started_cosine = true;
        return true;
    }

    bool stop_continuous_cosine_wave()
    {
        if (!started_cosine)
        {
            return true;
        }
        started_cosine = false;
        dac_cosine_stop(chan0_handle);
        delay(10);
        dac_cosine_del_channel(chan0_handle);
        delay(10);
        return false;
    }
};

void arbitrary_wave_start(uint8_t *buffer, size_t buf_size, Signal_Generator *sig, uint32_t freq)
{
    arbitrary_wave_stop();
    sig->stop_continuous_cosine_wave();
    if (freq < 20000)
    {
        return;
    }
    dac_continuous_handle_t cont_handle;
    dac_continuous_config_t cont_cfg = {
        .chan_mask = DAC_CHANNEL_MASK_ALL,
        .desc_num = 8,
        .buf_size = 2048,
        .freq_hz = freq,
        .offset = 0,
        .clk_src = DAC_DIGI_CLK_SRC_DEFAULT, // If the frequency is out of range, try 'DAC_DIGI_CLK_SRC_APLL'
        /* Assume the data in buffer is 'A B C D E F'
         * DAC_CHANNEL_MODE_SIMUL:
         *      - channel 0: A B C D E F
         *      - channel 1: A B C D E F
         * DAC_CHANNEL_MODE_ALTER:
         *      - channel 0: A C E
         *      - channel 1: B D F
         */
        .chan_mode = DAC_CHANNEL_MODE_SIMUL,
    };
    /* Allocate continuous channel */
    dac_continuous_new_channels(&cont_cfg, &cont_handle);
    /* Enable the channels in the group */
    dac_continuous_enable(cont_handle);
    dac_continuous_write_cyclically(cont_handle, (uint8_t *)buffer, buf_size, NULL);
    cont_handle_global = cont_handle;
    started_cont = true;
}

void arbitrary_wave_stop()
{
    if (!started_cont)
    {
        return;
    }
    started_cont = false;
    dac_continuous_disable(cont_handle_global);
    dac_continuous_del_channels(cont_handle_global);
}
#endif