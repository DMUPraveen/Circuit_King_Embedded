#ifndef IV_COMMAND_H
#define IV_COMMAND_H
void make_connections(uint8_t connections)
{
}
void set_voltage_and_current(float voltage, float current)
{
}

void read_values(float *return_values)
{
}
void run_iv_command()
{
    uint8_t connections = Serial2.read();
    float voltage;
    float current;
    Serial.readBytes((uint8_t *)(&voltage), 4);
    Serial.readBytes((uint8_t *)(&current), 4);
    set_voltage_and_current(voltage, current);
    float return_values[6] = {0, 1, 2, 3, 4, 5};
    read_values(return_values);
    Serial2.write((uint8_t *)return_values, 6 * 4);
}
#endif