#include <sys/_types.h>
#include "Hardware.h"
#include <Wire.h>
#include <INA226_WE.h>
#include <OneWire.h>
#include <DallasTemperature.h>


// ======================= Sensors =======================================
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
INA226_WE ina226 = INA226_WE(I2C_ADDRESS);

const float SHUNT_RESISTOR = 0.06;
unsigned long LastTempRequestTime = 0;


float HW::ReadGas()
{
  return analogRead(MQ2_ADC_PIN);
}

float HW::ReadTemperature()
{ 
  if(millis() - LastTempRequestTime < 750)
    return sensors.getTempCByIndex(0);

  sensors.requestTemperatures();
  LastTempRequestTime = millis();
  return sensors.getTempCByIndex(0);
}

void HW::init()
{
    Wire.setSDA(I2C_SDA_PIN);
    Wire.setSCL(I2C_SCL_PIN);
    Wire.begin();

    delay(2000);

    ina226.init();

    ina226.setAverage(INA226_AVERAGE_64);
    ina226.setConversionTime(INA226_CONV_TIME_1100);
    ina226.setMeasureMode(INA226_CONTINUOUS);
    ina226.setResistorRange(0.006, 4);

    sensors.begin();

    pinMode(CHARGE_PIN, OUTPUT);
    pinMode(MAIN_PIN, OUTPUT);
    pinMode(LOAD_PIN, OUTPUT);

    pinMode(BUZZER_PIN, OUTPUT);

    pinMode(LED_MAINS, OUTPUT);
    pinMode(LED_BATTERY, OUTPUT);
    pinMode(LED_CHARGE, OUTPUT);
    pinMode(LED_LOAD, OUTPUT);
    pinMode(LED_FAULT, OUTPUT);
    pinMode(LED_DANGER, OUTPUT);
    pinMode(MAINS_SENSE, INPUT);

    digitalWrite(CHARGE_PIN, LOW);
    digitalWrite(MAIN_PIN, HIGH);
    digitalWrite(LOAD_PIN, LOW);

    digitalWrite(BUZZER_PIN, LOW);

    digitalWrite(LED_MAINS, LOW);
    digitalWrite(LED_BATTERY, LOW);
    digitalWrite(LED_CHARGE, LOW);
    digitalWrite(LED_LOAD, LOW);
    digitalWrite(LED_FAULT, LOW);
    digitalWrite(LED_DANGER, LOW);    
}

void HW::ReadINA(INAData& data)
{
    //INAData data;
    data.bus_v = ina226.getBusVoltage_V();
    data.current = ina226.getCurrent_A();
    data.sunt_mv = ina226.getShuntVoltage_mV()/1000;
    data.power = ina226.getBusPower();
}

void HW::ControlLoad(bool loadState)
{
  if(loadState)
    digitalWrite(LOAD_PIN, HIGH);
  else
    digitalWrite(LOAD_PIN, LOW);

   //digitalWrite(LOAD_PIN, loadState ? HIGH : LOW);

    Serial.print("GP15 command=");
    Serial.print(loadState);
    Serial.print(" read=");
    Serial.println(digitalRead(LOAD_PIN));
}

void HW::ControlCharge(bool chargeState)
{
  if(chargeState)
    digitalWrite(CHARGE_PIN, HIGH);
  else
    digitalWrite(CHARGE_PIN, LOW);
}

void HW::SetBuzzer(bool on)
{
    //digitalWrite(BUZZER_PIN, on ? HIGH : LOW);
}

void HW::SetMainsLED(bool state)
{
    digitalWrite(LED_MAINS, state ? HIGH : LOW);
}

void HW::SetBatteryLED(bool state)
{
    digitalWrite(LED_BATTERY, state ? HIGH : LOW);
}

void HW::SetChargeLED(bool state)
{
    digitalWrite(LED_CHARGE, state ? HIGH : LOW);
}

void HW::SetLoadLED(bool state)
{
    digitalWrite(LED_LOAD, state ? HIGH : LOW);
}

void HW::SetFaultLED(bool state)
{
    digitalWrite(LED_FAULT, state ? HIGH : LOW);
}

void HW::SetDangerLED(bool state)
{
    digitalWrite(LED_DANGER, state ? HIGH : LOW);
}

void HW::ControlMain(bool state)
{
    digitalWrite(MAIN_PIN, state ? HIGH : LOW);  
}

bool HW::IsMainsPresent()
{
    return digitalRead(MAINS_SENSE);
}

