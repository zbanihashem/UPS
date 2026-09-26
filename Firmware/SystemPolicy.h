#ifndef SYSTEMPOLICY_H
#define SYSTEMPOLICY_H

#include "FaultEngine.h"
#include "PowerControl.h"

class SystemPolicy {
public:
    SystemPolicy(PowerControl &power) : m_power(power) {}

    void Apply(const FaultEngine &faults);
   // void DisplayFaults(FaultEngine &faults);

private:
    PowerControl &m_power;     
};

#endif