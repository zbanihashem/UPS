#include <sys/_stdint.h>
#ifndef FAULT_ENGINE_H
#define FAULT_ENGINE_H

// ======================= Safety Thresholds =============================

#define BAT_LOW_THRESHOLD      12.0f//11.7f
#define BAT_LOW_HYSTERESIS     0.3f

#define BAT_DOWN_THRESHOLD     11.5f//11.4f

#define TEMP_MAX_THRESHOLD     45.0f
#define TEMP_HYSTERESIS        2.0f

#define GAS_WARNING_THRESHOLD  120
#define GAS_DANGER_THRESHOLD   250
#define GAS_HYSTERESIS         10
#define CURRENT_LIMIT          5

#include "Sensors.h"

enum FaultLevel {
    FL_NORMAL,
    FL_WARNING,
    FL_CRITICAL,
    FL_DANGER
};
extern FaultLevel FaultLevels;

typedef enum {
  FAULT_BAT_LOW,
  FAULT_OVERCURRENT,
  FAULT_TEMPERATURE,
  FAULT_GAS,
  FAULT_MAINS_LOST,
  FAULT_NONE  
} FaultTypes;

typedef struct {
  FaultTypes type;
  FaultLevel level;
  bool active;
  bool latched;
} FaultStatus;

struct Fault {
  FaultLevel level = FL_NORMAL;
  bool latched = false;
  uint8_t dangerStart = 0;
  uint8_t long lastLevel = 0;
  uint8_t repeatCount = 0;
};

typedef enum {
  STATE_NORMAL          =0,
  STATE_WARNING         =1,
  STATE_SHUTDOWN        =2,
  STATE_STOP_CHARGING   =4
} SystemStates;

extern FaultLevel systemLevel;
extern bool systemLatched;
extern Fault gasFault;
extern Fault currentFault;
extern Fault voltageFault;
extern Fault temperatureFault;

class FaultEngine
{
private:
    FaultLevel m_level = FL_NORMAL;
    bool m_latched = false;
    uint8_t faultCount = 0;
    FaultTypes m_type = FAULT_NONE;
    SystemStates m_state;

public:
    uint8_t Update(FaultEngine &engine, Sensors &faultData, const char* faultMessages[]);
    FaultStatus faults[FAULT_MAINS_LOST + 1];

    FaultLevel Level() const { return m_level; }
    SystemStates State() const { return m_state; }
    bool Latched() const { return m_latched; }
    FaultTypes types;
    FaultTypes Type() const { return types; }

    void EvaluateBattery(Sensors &data);
    void EvaluateOvercurrent(Sensors &data);
    void EvaluateTemperature(Sensors &data);
    void EvaluateGasLevel(Sensors &data);
    void EvaluateSystemFaults(Sensors &data);
    void UpdateSystemState(FaultEngine &engine, Sensors &data, const char* faultMessages[]);
    void ClearLatchedFaults(int type);
};

#endif