#include <sys/_stdint.h>
#ifndef SENSORS_H
#define SENSORS_H

#include <cstdint>
#include "Hardware.h"

class Sensors {
public:
    // Public read-only getters
    float GetBusVoltage() const { return busVoltage; }   // returns volts
    float GetShuntVoltage() const { return shuntVoltage; }   // returns volts
    float GetCurrent() const { return current; }   // returns amps
    int16_t GetGasPPM() const { return gasPPM; }
    float GetPower() const { return power; }
    int16_t GetTemperature() const { return temperature; }
    
    void UpdateFromINA(Sensors &ina);
    void UpdateTemperature(Sensors &temp);
    void UpdateGasLevel(Sensors &gasLevel);
    void ReadAll(Sensors &data);
    void Init();

private:
    float busVoltage= 0;
    float shuntVoltage= 0;
    float current = 0;
    float power= 0;
    int16_t gasPPM = 0;
    int16_t temperature = 0;
    
    void SetBusVoltageRaw(float v) { busVoltage = v; }
    void SetShuntVoltageRaw(float v) { shuntVoltage = v; }
    void SetCurrentRaw(float c) { current = c; }
    void SetGasRaw(int16_t g) { gasPPM = g; }
    void SetTemperature(int16_t t) { temperature = t; }
    void SetPower(float p) { power = p / 1000; }
};

#endif