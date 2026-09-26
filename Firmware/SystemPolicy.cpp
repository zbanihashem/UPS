#include "SystemPolicy.h"

void SystemPolicy::Apply(const FaultEngine &faults)
{
    SystemStates currentState = faults.State();

    if (currentState & STATE_SHUTDOWN) {
        m_power.Shutdown();
    } else {
        m_power.Enable();
    }
    if (currentState & STATE_STOP_CHARGING) {
        m_power.StopCharging();
    } else {
        m_power.StartCharging();
    }
}