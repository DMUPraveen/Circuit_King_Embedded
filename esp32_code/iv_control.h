#ifndef IV_CONTROL_H
#define IV_CONTROL_H
#include <Wire.h>
#include <Adafruit_MCP4725.h>
#include "Arduino.h"
#include <INA3221.h>

// Parameters for DAC
Adafruit_MCP4725 DAC_01_VCCS;
Adafruit_MCP4725 DAC_02_VCVS;

#define LOGIC_LEVEL 3.3

// Resistor Range Control Pins
#define MAIN_RANGE_SELECTOR 26
#define LOW_RANGE_SELECTOR 27
#define HIGH_RANGE_SELECTOR 32

// current source working region - Normalized w.r.t. Max range
#define VCCS_UPPER_CLAMP 0.92
#define VCCS_LOWER_CLAMP 0.2

#define SETTLING_TIME 150 // ms
// INA3221 Sensor Parameters
#define R_SHUNT 100        // milli ohms
#define MAX_VOLTAGE 163800 // in uV

INA3221 INA3221_Sensor(INA3221_ADDR40_GND);

int32_t Measure_Shunt(ina3221_ch_t channel)
{
    int32_t voltage = INA3221_Sensor.getShuntVoltage(channel);
    if (voltage > MAX_VOLTAGE)
        voltage = voltage - 2 * MAX_VOLTAGE;
    return voltage;
}

// val 0 - 6.6 V
void set_Voltage(float val)
{
    DAC_02_VCVS.setVoltage((int)((val / 2) / LOGIC_LEVEL * 4095), false);
    // Serial.print((int)( (val/2) / LOGIC_LEVEL * 4095 ));
    // dacWrite( DAC_CH2 , (int)( (val/2) / LOGIC_LEVEL * 255 ) ) ;
    return;
}

void reset_VCCS()
{
    digitalWrite(MAIN_RANGE_SELECTOR, LOW);
    digitalWrite(LOW_RANGE_SELECTOR, LOW);
    digitalWrite(HIGH_RANGE_SELECTOR, LOW);
    delay(10);
    DAC_01_VCCS.setVoltage(4095, false);
    return;
}

// current value should be in mA
void set_Current(float value)
{
    // II = val;
    int val = (int)(value * 1000);
    // Serial.println(val);
    int current_range;

    switch (val)
    {
    case 0 ... 1000: // 0 mA - 1 mA
        // Serial.println("Case 01");
        digitalWrite(MAIN_RANGE_SELECTOR, LOW);
        digitalWrite(LOW_RANGE_SELECTOR, LOW);
        digitalWrite(HIGH_RANGE_SELECTOR, LOW);
        delay(SETTLING_TIME);
        current_range = 1000;
        break;

    case 1001 ... 10000: // 1 mA - 10 mA
        // Serial.println("Case 02");
        digitalWrite(MAIN_RANGE_SELECTOR, LOW);
        digitalWrite(LOW_RANGE_SELECTOR, HIGH);
        digitalWrite(HIGH_RANGE_SELECTOR, LOW);
        delay(SETTLING_TIME);
        current_range = 10000;
        break;

    case 10001 ... 100000: // 10 mA - 100 mA
        // Serial.println("Case 03");
        digitalWrite(MAIN_RANGE_SELECTOR, HIGH);
        digitalWrite(LOW_RANGE_SELECTOR, LOW);
        digitalWrite(HIGH_RANGE_SELECTOR, LOW);
        delay(SETTLING_TIME);
        current_range = 100000;
        break;

    case 100001 ... 1000000: // 100 mA - 1000 mA
        // Serial.println("Case 04");
        digitalWrite(MAIN_RANGE_SELECTOR, HIGH);
        digitalWrite(LOW_RANGE_SELECTOR, LOW);
        digitalWrite(HIGH_RANGE_SELECTOR, HIGH);
        delay(SETTLING_TIME);
        current_range = 1000000;
        break;
    }
    float current_level = (val * 1.0) / current_range;

    if (current_level < VCCS_LOWER_CLAMP)
    {
        current_level = VCCS_LOWER_CLAMP;
    }
    else if (current_level > VCCS_UPPER_CLAMP)
    {
        current_level = VCCS_UPPER_CLAMP;
    }
    int control_signal = (int)((current_level - VCCS_LOWER_CLAMP) * (1100 - 4095) / (VCCS_UPPER_CLAMP - VCCS_LOWER_CLAMP) + 4095);
    // Serial.println(control_signal);
    DAC_01_VCCS.setVoltage(control_signal, false);
    delay(10);
    return;
}

void read_values(float *buffer)
{
    buffer[0] = Measure_Shunt(INA3221_CH1) / 100.0;           // CH1 I
    Serial.println(buffer[0]);
    buffer[1] = INA3221_Sensor.getVoltage(INA3221_CH1) * 1.0; // CH1 V
    Serial.println(buffer[1]);
    buffer[2] = Measure_Shunt(INA3221_CH2) / 100.0;           // CH2 I
    Serial.println(buffer[2]);
    buffer[3] = INA3221_Sensor.getVoltage(INA3221_CH2) * 1.0; // CH2 V
    Serial.println(buffer[3]);
    buffer[4] = Measure_Shunt(INA3221_CH3) / 100.0;           // CH3 I
    Serial.println(buffer[4]);
    buffer[5] = INA3221_Sensor.getVoltage(INA3221_CH3) * 1.0; // CH3 V
    Serial.println(buffer[5]);
    return;
}

void setup_iv()
{
    DAC_01_VCCS.begin(0x60);
    DAC_02_VCVS.begin(0x61);
    Serial.println("DAC Initiated");

    INA3221_Sensor.begin();
    INA3221_Sensor.reset();
    INA3221_Sensor.setShuntRes(R_SHUNT, R_SHUNT, R_SHUNT);
    Serial.println("INA3221 Initiated");

    pinMode(MAIN_RANGE_SELECTOR, OUTPUT);
    pinMode(LOW_RANGE_SELECTOR, OUTPUT);
    pinMode(HIGH_RANGE_SELECTOR, OUTPUT);

    reset_VCCS();
}
#endif