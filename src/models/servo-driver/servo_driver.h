#ifndef SERVO_DRIVER_H
#define SERVO_DRIVER_H

#include <Arduino.h>

#define OPTO_PIN 17

extern bool servo_driver_setup();
extern void servo_driver(int frequency_hz, int duty_cicle_percent);
extern void rotate_servo(int position_percentage, int duration_ms, int frequency_hz);

#endif