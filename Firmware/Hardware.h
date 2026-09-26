#ifndef HARDWARE_H
#define HARDWARE_H

#include <stdint.h>
#include <Arduino.h>
// ======================= I/O Configuration =============================
#define I2C_ADDRESS   0x40

#define MAINS_SENSE   3

#define I2C_SDA_PIN   4
#define I2C_SCL_PIN   5

#define LED_MAINS     6
#define LED_FAULT     7
#define LED_LOAD      8
#define LED_CHARGE    9
#define LED_BATTERY   10
#define LED_DANGER    11

#define MAIN_PIN      12
#define CHARGE_PIN    13
#define ONE_WIRE_BUS  14
#define LOAD_PIN      15

#define BUZZER_PIN    17

#define MQ2_ADC_PIN   26


struct INAData
{
    float bus_v;
    float sunt_mv;
    float current;
    float power;
};

namespace HW {

    
    void init();

    // --- Voltage / Current (INA, ADC, etc.) ---
    void ReadINA(INAData&); 
    
    // --- Gas sensor ---
    float ReadGas();

    // --- Temperature (OneWire) ---
    float ReadTemperature();

    void SetBuzzer(bool buzzerState);

    // --- Outputs ---
    void SetMainsLED(bool state);
    void SetBatteryLED(bool state);
    void SetChargeLED(bool state);    
    void SetLoadLED(bool state);
    void SetFaultLED(bool state);
    void SetDangerLED(bool state);

    void ControlMain(bool mainState);
    void ControlLoad(bool loadState);
    void ControlCharge(bool chargeState);
    bool IsMainsPresent();
    

}

#endif