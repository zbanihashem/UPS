#include "SerialUSB.h"
#include "Sensors.h"

void Sensors::UpdateFromINA(Sensors &ina)
{
    INAData data;
    HW::ReadINA(data);

    ina.SetBusVoltageRaw(data.bus_v);
    ina.SetShuntVoltageRaw(data.sunt_mv);
    ina.SetCurrentRaw(data.current);
    ina.SetPower(data.power);    
}

void Sensors::UpdateTemperature(Sensors &temp)
{
    temp.SetTemperature(HW::ReadTemperature());
}

void Sensors::UpdateGasLevel(Sensors &gasLevel)
{
    gasLevel.SetGasRaw(HW::ReadGas());
}

void Sensors::ReadAll(Sensors &data)
{
    UpdateFromINA(data);
    UpdateGasLevel(data);
    UpdateTemperature(data);
}

void Sensors::Init()
{
    HW::init();
}
