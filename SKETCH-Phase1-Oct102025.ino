#include <Arduino.h>
#include <U8g2lib.h>
#include <Wire.h>
#include <INA226_WE.h>
#include <OneWire.h>
#include <DallasTemperature.h>

// ======================= I/O Configuration =============================
#define I2C_ADDRESS 0x40
#define ONE_WIRE_BUS 14

// ======================= Safety Thresholds =============================

const float MIN_BATTERY_VOLTAGE = 11.5;
const float MAX_BATTERY_TEMP = 45.0;
const float SHUNT_RESISTOR = 0.06;

// ======================= Sensors =======================================
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

// ======================= Display ( SPI SSD1325 ) =======================
INA226_WE ina226 = INA226_WE(I2C_ADDRESS);
U8G2_SSD1325_NHD_128X64_F_4W_HW_SPI u8g2(
  U8G2_R0,
  17, // cs
  16, // dc
  20  // reset
);

// ======================= Variables =====================================
float busVoltage = 0.0;
float shuntVoltage = 0.0 ;
float current_mA = 0.0;
float power_mW = 0.0;
float temperatureC = 0.0;
bool tempWarning = false;
bool voltageWarning = false;

// ======================= Warning Messages ==============================

const String voltageMessage = "LOW VOLTAGE!";
const String tempMessage = "HIGH TEMPERATURE!";
const String gasMessage = "GAS WARNING!";

void setup(void) {
  
  Wire.begin();
  delay(2000);
  sensors.begin();

  u8g2.begin();
  u8g2.clearBuffer();

  // Title text
  u8g2.setFont(u8g2_font_6x10_tr);
  u8g2.drawStr(20, 12, "Battery Status");

  if (!ina226.init()) {
    u8g2.drawStr(1, 15, "INA226 not found!");
    while (1);
  }

  ina226.setAverage(INA226_AVERAGE_16);
  ina226.setConversionTime(INA226_CONV_TIME_1100);
  ina226.setMeasureMode(INA226_CONTINUOUS);
  ina226.setResistorRange(0.1, 3.2);

  u8g2.drawStr(1, 20, "INA226 initialized successfully!");
  u8g2.sendBuffer();  
}


void loop(void) {

  busVoltage   = ina226.getBusVoltage_V();
  shuntVoltage = ina226.getShuntVoltage_mV();
  current_mA   = ina226.getCurrent_mA();
  power_mW     = busVoltage * (current_mA / 1000.0) * 1000.0;
  char buf[32];

// DS18B20 reading
  sensors.requestTemperatures();
  temperatureC = sensors.getTempCByIndex(0);

// Sfety Check
tempWarning = false;
voltageWarning = false;

if(busVoltage < MIN_BATTERY_VOLTAGE){
  voltageWarning = true;
}

else if (temperatureC > MAX_BATTERY_TEMP){
  tempWarning = true;
}
  u8g2.clearBuffer();
  u8g2.setCursor(0, 10);  u8g2.print("Bus: ");  u8g2.print(busVoltage, 2);  u8g2.print(" V");
  u8g2.setCursor(0, 22);  u8g2.print("Shunt: ");  u8g2.print(shuntVoltage, 2);  u8g2.print(" mV");
  u8g2.setCursor(0, 34);  u8g2.print("Current: ");  u8g2.print(current_mA, 2);  u8g2.print(" mA");
  u8g2.setCursor(0, 46);  u8g2.print("Temp: ");  u8g2.print(temperatureC, 2);  u8g2.print(" C");

  /*
  snprintf(buf, sizeof(buf), "Bus: %.1f V", busVoltage); u8g2.drawStr(0, 10, buf);  
  snprintf(buf, sizeof(buf), "Shunt: %.1f mV", shuntVoltage); u8g2.drawStr(0, 22, buf);  
  snprintf(buf, sizeof(buf), "Current: %.1f mA", current_mA); u8g2.drawStr(0, 34, buf);  
  //snprintf(buf, sizeof(buf), "Power: %.1f mW", power_mW); u8g2.drawStr(0, 46, buf);  
  snprintf(buf, sizeof(buf), "Temperature: %.1f C", tempC); u8g2.drawStr(0, 46, buf);
  */

  if(tempWarning) {
    u8g2.setCursor(0, 60);
    u8g2.print("WARN: ");
    u8g2.print(tempMessage);
  } 
  if(voltageWarning) {
    u8g2.setCursor(0, 60);
    u8g2.print("WARN: ");
    u8g2.print(voltageMessage);
  }
  
  u8g2.sendBuffer();
  delay(1000);  
}
