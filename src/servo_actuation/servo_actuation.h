#ifndef SERVO_ACTUATION_H
#define SERVO_ACTUATION_H

#include <Arduino.h>
#include "../models/ams5600/ams5600.h"
#include "../models/servo-driver/servo_driver.h"

extern bool check_servo_type(bool& is_positional);
bool check_continuos_rotation_params(float& initial_servo_position, float& second_servo_position, float& max_velocity, int& lower_zero_vel_freq, int& upper_zero_vel_freq);
bool check_positional_servo_params(bool& servo_is_accurate, float& initial_servo_position, float& second_servo_position, int& expected_half_position, int& expected_full_position, float& actual_half_position, float& actual_full_position);

#endif