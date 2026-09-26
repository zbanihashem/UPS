//#include "SerialUSB.h"
#include <sys/_stdint.h>
#include "FaultEngine.h"

Fault gasFault;
Fault currentFault;
Fault voltageFault;
Fault temperatureFault;
//SystemStates systemState;
FaultLevel FaultLevels;

const char* FaultStrings[] {
  "Low Battery!",       //FAULT_BAT_LOW
  "Critical Batery!",   //FAULT_BAT_CRITICAL
  "Over Current!",      //FAULT_OVERCURRENT
  "Over Temperature!",  //FAULT_TEMP_HIGH
  "Gas Fault!",         //FAULT_GAS_WARNING
  "Gas Danger",         //FAULT_GAS_DANGER
  "Mains Lost!"         //FAULT_MAINS_LOST  
};

uint8_t FaultEngine::Update(FaultEngine &engine, Sensors &faultData, const char* faultMessages[])
{
    this->EvaluateBattery(faultData);
    this->EvaluateOvercurrent(faultData);
    this->EvaluateTemperature(faultData);
    this->EvaluateGasLevel(faultData);
    this->EvaluateSystemFaults(faultData);
    this->UpdateSystemState(engine, faultData, faultMessages);
    return faultCount;
}

void FaultEngine::EvaluateBattery(Sensors &faultData)
{
    float v = faultData.GetBusVoltage();

    if (v < BAT_LOW_THRESHOLD)
    {
        {
            faults[FAULT_BAT_LOW].type = FAULT_BAT_LOW;
            faults[FAULT_BAT_LOW].active = true;
            if(v < BAT_DOWN_THRESHOLD)
                faults[FAULT_BAT_LOW].level = FL_CRITICAL;
            else
                faults[FAULT_BAT_LOW].level = FL_WARNING;            

        }
    }

    if (v > (BAT_DOWN_THRESHOLD + BAT_LOW_HYSTERESIS))
    {
        if (faults[FAULT_BAT_LOW].latched)
            faults[FAULT_BAT_LOW].latched = false;

        if(v > BAT_LOW_THRESHOLD + BAT_LOW_HYSTERESIS)
        {
            faults[FAULT_BAT_LOW].active = false;   
            faults[FAULT_BAT_LOW].level = FL_NORMAL;
        }
        else
            faults[FAULT_BAT_LOW].level = FL_WARNING;
    }
}

void FaultEngine::EvaluateOvercurrent(Sensors &faultData)
{
    if (faultData.GetCurrent() > CURRENT_LIMIT)
    {
        faults[FAULT_OVERCURRENT].type = FAULT_OVERCURRENT;
        faults[FAULT_OVERCURRENT].active = true;
        faults[FAULT_OVERCURRENT].level = FL_CRITICAL;
        faults[FAULT_OVERCURRENT].latched = true;
    }
}

void FaultEngine::EvaluateTemperature(Sensors &faultData)
{
    float t = faultData.GetTemperature();

    // Raise condition
    if (t > TEMP_MAX_THRESHOLD)
    {
        if (!faults[FAULT_TEMPERATURE].active)
        {
            faults[FAULT_TEMPERATURE].type = FAULT_TEMPERATURE;
            faults[FAULT_TEMPERATURE].active = true;
            faults[FAULT_TEMPERATURE].latched = true;
            faults[FAULT_TEMPERATURE].level = FL_DANGER;
        }
    }

    // Clear condition (with hysteresis)
    else if (t < (TEMP_MAX_THRESHOLD + TEMP_HYSTERESIS))
    {
        if (!faults[FAULT_TEMPERATURE].latched)
        {
            faults[FAULT_TEMPERATURE].active = false;
            faults[FAULT_TEMPERATURE].level = FL_NORMAL;
        }
    }
}

void FaultEngine::EvaluateGasLevel(Sensors &faultData)
{
    int16_t g = faultData.GetGasPPM();

    if (g > GAS_DANGER_THRESHOLD)
    {
        if (!faults[FAULT_GAS].active)
        {
            faults[FAULT_GAS].type = FAULT_GAS;
            faults[FAULT_GAS].active = true;
            faults[FAULT_GAS].latched = true;
            faults[FAULT_GAS].level = FL_DANGER;
        }
    }
    else if (g > GAS_WARNING_THRESHOLD)
    {
        if (!faults[FAULT_GAS].active)
        {
            faults[FAULT_GAS].type = FAULT_GAS;
            faults[FAULT_GAS].active = true;
            faults[FAULT_GAS].level = FL_WARNING;
        }
    }
    else if (g < GAS_WARNING_THRESHOLD + GAS_HYSTERESIS )
    {
        faults[FAULT_GAS].active = false;
        faults[FAULT_GAS].level = FL_NORMAL;
    }
}

void FaultEngine::EvaluateSystemFaults(Sensors &faultData)
{
  m_level = FL_NORMAL;
  m_latched = false;

  for(int i=0; i<4; i++)
  {
    if(faults[i].latched)
    {
        m_latched = true;
    }
    switch(faults[i].level)
    {
        case FL_DANGER:
            m_level = FL_DANGER;
            break;
        case FL_CRITICAL:
            if(m_level != FL_DANGER)
            {
                m_level = FL_CRITICAL; 
            }
            break;
        case FL_WARNING:
            if(m_level != FL_DANGER && m_level != FL_CRITICAL)
            {
                m_level = FL_WARNING;
            }
            break;
    } 
  }
}

void FaultEngine::UpdateSystemState(FaultEngine &engine, Sensors &faultData, const char* faultMessages[])
{
    bool critical = false;
    bool warning = false;
    bool isBattery = false;
    bool danger = false;
    faultCount = 0;
    

    for (int i = 0; i <= FAULT_MAINS_LOST; i++)
    {
        if (faults[i].active)
        {
            if (faults[i].level == FL_CRITICAL)
                critical = true;
            else if (faults[i].level == FL_WARNING)
                warning = true;
            else if (faults[i].level == FL_DANGER)
                danger = true;

            switch (faults[i].type)
            {
                case FAULT_BAT_LOW:
                    if(faults[i].level == FL_CRITICAL)
                    {
                        faultMessages[faultCount] = FaultStrings[1];
                    }
                    else
                        faultMessages[faultCount] = FaultStrings[0];
                    break;
                
                case FAULT_OVERCURRENT:
                    faultMessages[faultCount] = FaultStrings[2];
                    break;

                case FAULT_TEMPERATURE:
                    faultMessages[faultCount] = FaultStrings[3];
                    isBattery = true;
                    break;

                case FAULT_GAS:
                    if(faults[i].level == FL_DANGER)
                    {
                        faultMessages[faultCount] = FaultStrings[5];
                        isBattery = true;
                    }
                    else
                        faultMessages[faultCount] = FaultStrings[4];
                    break;
            }
            faultCount++;
        }
    }

    m_state = STATE_NORMAL;
    if (isBattery)
        m_state = (SystemStates)(m_state | STATE_STOP_CHARGING);
    if (critical || danger)
        m_state =  (SystemStates)(m_state | STATE_SHUTDOWN);
    else if (warning)
        m_state = (SystemStates)(m_state | STATE_WARNING);

}

void FaultEngine::ClearLatchedFaults(int type)
{
    faults[type].active = false;
    faults[type].latched = false;
    faults[type].level = FL_NORMAL;
}
