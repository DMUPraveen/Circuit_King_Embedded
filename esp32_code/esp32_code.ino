#include "wave_gen.h"
#include "arbitrary_wave_command.h"
#include "sine_wave_command.h"
#include "Command_codes.h"
#define RXD2 16
#define TXD2 17
void serial_cleanup()
{
    while (Serial2.available())
    {
        Serial2.read();
    }
}
void setup()
{
    Serial2.begin(115200, SERIAL_8N1, RXD2, TXD2);
    Serial.begin(115200);
    auto sig = Signal_Generator();
    while (1)
    {
        while (!Serial2.available())
        {
            /*
            Busy wait while no data is available
            */
        }
        uint8_t command = Serial2.read(); // read the command code
        switch (command)
        {
        case SINE_WAVE_COMMAND:
            /* code */
            run_sine_gen_command(&sig);
            Serial.println("Sine wave OK");
            break;
        case ARBITRARY_WAVE_COMMAND:
            run_arbitrary_wave_command(&sig);
            Serial.println("Arbitrary wave OK");
            break;

        default:
            // incorrect command code run a serial cleanup
            serial_cleanup();
            break;
        }
    }
}

void loop()
{
}
