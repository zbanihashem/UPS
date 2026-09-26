#include "PowerControl.h"
#include "Hardware.h"

void PowerControl::Begin()
{

}

void PowerControl::Enable()
{
    HW::ControlMain(true);
    HW::ControlLoad(true);
}

void PowerControl::Shutdown()
{
    HW::ControlLoad(false);
    HW::ControlCharge(false);
    HW::ControlMain(false);
}

void PowerControl::StopCharging()
{
    HW::ControlCharge(false);
}

void PowerControl::StartCharging()
{
    HW::ControlCharge(true);
} 
