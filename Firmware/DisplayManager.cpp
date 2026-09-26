#include <sys/_stdint.h>
#include "DisplayManager.h"
#include "Hardware.h"

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(
  U8G2_R0,
  U8X8_PIN_NONE
);

void DisplayManager::Init()
{
    u8g2.begin();
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_6x12_tf);
}

void DisplayManager::Update(uint8_t faultCount,
                            Sensors &data,
                            FaultEngine &faultEngine,
                            const char* faults[])
{
    if (faultCount == 0)
        UpdateDisplay(data);
    else
        ShowFaultScreen(faultCount, faults);

    UpdateIndicators(data, faultEngine);
    UpdateBuzzer(faultEngine);
}

void DisplayManager::UpdateDisplay(Sensors &data)
{
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_6x12_tf);

  u8g2.setCursor(5, 12);
  u8g2.print("Temp: ");
  u8g2.print(data .GetTemperature());
  u8g2.print(" C");

  u8g2.setCursor(5, 24);
  u8g2.print("Gas: ");
  u8g2.print(data.GetGasPPM());

  u8g2.setCursor(5, 36);
  u8g2.print("Volt: ");
  u8g2.print(data.GetBusVoltage());
  u8g2.print(" V");

  u8g2.setCursor(5, 48);
  u8g2.print("Current: ");
  u8g2.print(data.GetCurrent());
  u8g2.print(" A");

  u8g2.setCursor(5, 60);
  u8g2.print("Power: ");
  u8g2.print(data.GetPower());
  u8g2.print(" W");

  u8g2.sendBuffer();
}

void DisplayManager::ShowFaultScreen(uint8_t faultCount, const char* faultMessages[])
{
  int row = 12;

  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_6x12_tf);
  for(int i=0; i<faultCount; i++)
  {
    u8g2.setCursor(5, row);
    u8g2.print(faultMessages[i]);
    row += 12;  
  }  
  u8g2.sendBuffer();
}

void DisplayManager::UpdateIndicators(Sensors &data, FaultEngine &faultEngine)
{
    // Fault
    HW::SetFaultLED(faultEngine.Level() != FL_NORMAL);

    // Danger
    HW::SetDangerLED(
        faultEngine.Level() == FL_CRITICAL ||
        faultEngine.Level() == FL_DANGER
    );

    // Load
    HW::SetLoadLED(
    !(faultEngine.State() & STATE_SHUTDOWN));

    // Charge
    HW::SetChargeLED(
    !(faultEngine.State() & STATE_STOP_CHARGING));

    // Mains
    bool mainsPresent = HW::IsMainsPresent();
    HW::SetMainsLED(mainsPresent);
    HW::SetBatteryLED(!mainsPresent);

    // Charge
    HW::SetChargeLED(
    HW::IsMainsPresent() &&
    !(faultEngine.State() & STATE_STOP_CHARGING)
);

}

void DisplayManager::UpdateBuzzer(FaultEngine &faultEngine)
{
    static unsigned long lastToggle = 0;
    static bool buzzerState = false;

    switch (faultEngine.Level())
    {
        case FL_NORMAL:
            HW::SetBuzzer(false);
            buzzerState = false;
            break;

        case FL_WARNING:
            if (millis() - lastToggle >= 1000)
            {
                lastToggle = millis();
                buzzerState = !buzzerState;
                HW::SetBuzzer(buzzerState);
            }
            break;

        case FL_CRITICAL:
            if (millis() - lastToggle >= 300)
            {
                lastToggle = millis();
                buzzerState = !buzzerState;
                HW::SetBuzzer(buzzerState);
            }
            break;

        case FL_DANGER:
            HW::SetBuzzer(true);
            buzzerState = true;
            break;
    }
}
