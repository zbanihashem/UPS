#ifndef POWER_CONTROL_H
#define POWER_CONTROL_H

class PowerControl {
public:
    void Begin();
    void Enable();
    void Shutdown();
    void StartCharging();
    void StopCharging();

private:
    int m_pin;
};

#endif