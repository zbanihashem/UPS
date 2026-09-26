#include "Sensors.h"
#include "FaultEngine.h"
#include "PowerControl.h"
#include "SystemPolicy.h"
#include "DisplayManager.h"

Sensors sensorData;
FaultEngine faultEngine;
PowerControl power;
SystemPolicy policy(power);
DisplayManager display;
const char* faultMessages[5];
uint8_t faultCount = 0;

void setup()
{
   display.Init();
   sensorData.Init();
   //power.Begin(); // example pin
}

void loop()
{
    
    sensorData.ReadAll(sensorData);

    faultCount = faultEngine.Update(faultEngine, sensorData, faultMessages);

    policy.Apply(faultEngine);
    
    display.Update(faultCount, sensorData, faultEngine, faultMessages);

    delay(500);
}