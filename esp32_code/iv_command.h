#include <stdint.h>
#ifndef IV_COMMAND_H
#define IV_COMMAND_H
#include "iv_control.h"
void make_connections(uint8_t connections)
{
}
void set_voltage_and_current(float voltage, float current)
{
    set_Voltage(voltage);
    set_Current(current);
    // delay(100);
}

void run_iv_command()
{
    uint8_t connections = Serial2.read();
    Serial.print("connections done");
    float voltage;
    float current;
    Serial2.readBytes((uint8_t *)(&voltage), 4);
    Serial.print("voltage done");
    Serial2.readBytes((uint8_t *)(&current), 4);
    Serial.print("current done");
    set_voltage_and_current(voltage, current);
    float return_values[6] = {0, 1, 2, 3, 4, 5};
    read_values(return_values);
    // uint8_t dummy_buffer[24] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23};

    Serial2.write((uint8_t *)return_values, 6 * 4);
}
#endif