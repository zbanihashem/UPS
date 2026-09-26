#include <sys/_stdint.h>
#ifndef DISPLAY_MANAGER_H
#define DISPLAY_MANAGER_H

#include <Arduino.h>
#include <U8g2lib.h>
#include "Sensors.h"
#include "FaultEngine.h"

extern U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2;

class DisplayManager {
public:
     void Update(uint8_t faultCount,
                Sensors &data,
                FaultEngine &faultEngine,
                const char* faultMessages[]);

    void Init();

private:
    void UpdateDisplay(Sensors &data);
    void ShowFaultScreen(uint8_t faultCount, const char* faultMessages[]);

    void UpdateIndicators(Sensors &data, FaultEngine &faultEngine);
    void UpdateBuzzer(FaultEngine &faultEngine);
};

#endif