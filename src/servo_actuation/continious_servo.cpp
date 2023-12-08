#include "servo_actuation.h"

bool check_continuos_rotation_params(float& initial_servo_position, float& second_servo_position, float& max_velocity, int& lower_zero_vel_freq, int& upper_zero_vel_freq) {
    float max_servo_velocity_1;
    float max_servo_velocity_2;
    float max_servo_velocity_3;
    float max_servo_velocity_deg_s;
    delay(500);
    
    read_raw_angle();
    initial_servo_position = degAngle;
    if(!rotate_servo(100, 500, 50)) return false;
    read_raw_angle();
    second_servo_position = degAngle;
    max_servo_velocity_1 = abs(second_servo_position - initial_servo_position)/0.5;
    delay(1000);

    read_raw_angle();
    initial_servo_position = degAngle;
    if(!rotate_servo(100, 500, 50))  return false;
    read_raw_angle();
    second_servo_position = degAngle;
    max_servo_velocity_2 = abs(second_servo_position - initial_servo_position)/0.5;
    delay(1000);

    read_raw_angle();
    initial_servo_position = degAngle;
    if(!rotate_servo(100, 500, 50))  return false;
    read_raw_angle();
    second_servo_position = degAngle;
    max_servo_velocity_3 = abs(second_servo_position - initial_servo_position)/0.5;
    delay(1000);
    
    max_velocity = (max_servo_velocity_1+max_servo_velocity_2+max_servo_velocity_3)/3;

    // check zero duty cycle
    if(!rotate_servo(0, 200, 50)) return false;
    read_raw_angle();
    int i = 2;
    for(i; i < 100; i = i + 2) {
        read_raw_angle();
        initial_servo_position = degAngle;
        if(!rotate_servo(i, 200, 50)) return false;
        read_raw_angle();
        delay(200);
        second_servo_position = degAngle;

        if(initial_servo_position == second_servo_position) break;
    }
    lower_zero_vel_freq = i;

    for(i; i < 100; i = i + 2) {
        read_raw_angle();
        initial_servo_position = degAngle;
        if(!rotate_servo(i, 200, 50)) return false;
        read_raw_angle();
        delay(200);
        second_servo_position = degAngle;

        if(initial_servo_position != second_servo_position) break;
    }
    upper_zero_vel_freq = i;
    return true;
}
