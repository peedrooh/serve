#include "ina219.h"

TwoWire _wire_ina = TwoWire(1);
Adafruit_INA219 _ina219;

float shunt_voltage = 0;
float bus_voltage = 0;
float current_mA = 0;
float load_voltage = 0;
float power_mW = 0;

extern bool ina219_setup(unsigned int SDA, unsigned int SCL) {
    if (!_wire_ina.begin(SDA, SCL)) {
        return false;
    }

    return _ina219.begin(&_wire_ina);
}

extern float get_shunt_voltage_mV() {
    shunt_voltage = _ina219.getShuntVoltage_mV();
    return  shunt_voltage;
}

extern float get_bus_voltage_V() {
    bus_voltage = _ina219.getBusVoltage_V();
    return bus_voltage;
}
extern float get_current_mA() {
    current_mA = _ina219.getCurrent_mA();
    return current_mA;
}

extern float get_power_mW() {
    power_mW = _ina219.getPower_mW();
    return power_mW;
}

extern float get_load_voltage_V() {
    bus_voltage = get_bus_voltage_V();
    shunt_voltage = get_shunt_voltage_mV();
    load_voltage = bus_voltage + (shunt_voltage / 1000);
    return load_voltage;
}