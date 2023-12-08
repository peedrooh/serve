#include "servo_actuation.h"

bool check_servo_type(bool& is_positional) {
    read_raw_angle();
    // send to 50 percent, if servo doesn't stop it is a continuos rotation servo
    unsigned long start_time = millis();
    unsigned long current_time = millis();
    while(current_time - start_time < 1000) {
        if(!rotate_servo(50, 10, 50)) return false;
        current_time = millis();
    }
    read_raw_angle();
    float first_position = degAngle;
    if(!rotate_servo(50, 100, 50)) return false;
    read_raw_angle();
    
    if (degAngle >= first_position - 5 && degAngle <= first_position + 5) {
        is_positional = true;
    } else {
        is_positional = false;
    }
    return true;
}
