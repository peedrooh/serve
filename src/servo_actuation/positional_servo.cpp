#include "servo_actuation.h"

bool check_positional_servo_params(bool& servo_is_accurate, float& initial_servo_position, float& second_servo_position, int& expected_half_position, int& expected_full_position, float& actual_half_position, float& actual_full_position) {
    servo_is_accurate = false;
    
    if(!rotate_servo(0, 1000, 50)) return false;
    read_raw_angle();
    delay(200);
    initial_servo_position = degAngle;

    // send servo to 50%
    if(!rotate_servo(50, 1000, 50)) return false;
    read_raw_angle();
    delay(200);
    second_servo_position = degAngle;

    float angle_difference = abs(second_servo_position - initial_servo_position);

    if(angle_difference < 45+10 && angle_difference > 45-10 ) {
        expected_half_position = 45;
        expected_full_position = 90;
        actual_half_position = angle_difference;
    } else if(angle_difference < 90+10 && angle_difference > 90-10 ) {
        expected_half_position = 90;
        expected_full_position = 180;
        actual_half_position = angle_difference;
    } else if(angle_difference < 180+10 && angle_difference > 180-10 ) {
        expected_half_position = 180;
        expected_full_position = 360;
        actual_half_position = angle_difference;
    };


    // send servo to 100%
    if(!rotate_servo(105, 1000, 50)) return false;
    read_raw_angle();
    delay(200);
    second_servo_position = degAngle;
    angle_difference = abs(second_servo_position - initial_servo_position);
    actual_full_position = angle_difference;

    if(actual_half_position <= expected_half_position + 10 && actual_half_position >= expected_half_position - 10) {
        if(actual_full_position <= expected_full_position + 10 && actual_full_position >= expected_full_position - 10) servo_is_accurate = true;
    }

    return true;
}
