# Smart UPS V1

A Raspberry Pi Pico based smart UPS designed for reliable backup power, battery protection, fault monitoring, and system status indication.

The project includes custom PCBs, embedded firmware, sensor monitoring, automatic mains/battery switching, and protection logic.

## V1 Goals

The primary goal of V1 is to build and operate a stable UPS prototype under real load for an extended test period.

V1 is intentionally focused on reliability rather than adding new features. The system is planned to operate for approximately four months so that hardware and firmware issues can be identified before designing V2.

## System Architecture

The system is divided into several functional sections:

- Control Board — Raspberry Pi Pico, RTC, OLED, buzzer and interfaces
- Power Board — power-path control, load switching and power monitoring
- Battery Board — battery temperature and gas monitoring
- LED Board — six system status indicators
- Firmware — fault detection, system policy, power control and display logic

## Firmware Architecture

The firmware is divided into independent modules:

- `Hardware` — low-level GPIO and hardware access
- `Sensors` — sensor acquisition
- `FaultEngine` — fault detection and fault levels
- `SystemPolicy` — system decisions based on detected faults
- `PowerControl` — load, charger and system-power control
- `DisplayManager` — OLED and LED status indication

Main firmware entry point:

`Firmware/UPSPhase1.ino`

## Current V1 Status

The core UPS power-control system is operational.

Verified functions include:

- Automatic startup when mains power is available
- Automatic transition from mains to battery
- Automatic transition back to mains
- Pico self-power hold while operating from battery
- Load disconnect on critical battery condition
- Charger inhibit control
- Complete control-system shutdown on critical battery condition
- Automatic restart when mains power returns
- Mains presence detection
- Battery voltage and current monitoring
- Battery temperature monitoring
- MQ-2 gas monitoring
- OLED status display
- RTC operation
- Buzzer control
- Six front-panel status LEDs

## Status LEDs

The front panel provides six indicators:

- Green — Mains
- Red — Fault
- White — Load
- Blue — Charge Enabled
- Yellow — Battery Operation
- Red — Critical / Danger

The Charge LED represents charger permission from the controller. It does not indicate measured charging current.

## Known V1 Limitations

V1 contains several hardware compromises discovered during integration testing.

These include:

- Passive diode OR power-path architecture
- Relay-based high-side load switching
- Charger module parasitic battery consumption
- Hardware modifications made during integration testing

These issues are being documented for correction in V2.

The current charger isolation issue will be addressed in V1 before extended operation.

## Repository Structure

- `Firmware/` — Raspberry Pi Pico firmware
- `Hardware/` — KiCad schematics, PCB designs and hardware documentation
- `Simulations/` — circuit simulations
- `README.md` — project overview and current status

## Development Strategy

V1 will be operated under real load for approximately four months.

During this period:

- faults and unexpected behavior will be recorded
- hardware modifications will be documented
- firmware changes will be version controlled
- design improvements will be collected for V2

V2 will incorporate the lessons learned from extended V1 operation.