#ifndef INA219_H
#define INA219_H

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_INA219.h>

extern bool ina219_setup(unsigned int SDA, unsigned int SCL);
extern float get_shunt_voltage_mV();
extern float get_bus_voltage_V();
extern float get_current_mA();
extern float get_power_mW();
extern float get_load_voltage_V();

extern float shunt_voltage;
extern float bus_voltage;
extern float current_mA;
extern float load_voltage;
extern float power_mW;

#endif