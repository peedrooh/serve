#include <Arduino.h>
#include <Wire.h>
#include <WiFi.h>

#include <Adafruit_INA219.h>

#include "models/ams5600/ams5600.h"
#include "models/leds/leds.h"
#include "models/rele/rele.h"
#include "models/button/button.h"
#include "models/servo-driver/servo_driver.h"
#include "models/ina219/ina219.h"

#define SDA_AMS 15
#define SCL_AMS 16

#define SDA_INA 42
#define SCL_INA 2


void setup() {
    Serial.begin(115200);
    // -------------------------------------------
    // AMS5600 SETUP
    // -------------------------------------------
    ams5600_begin(SDA_AMS, SCL_AMS);
    check_magnet_presence();
    read_raw_angle();
    startAngle = degAngle;

    // -------------------------------------------
    // LED'S SETUP
    // -------------------------------------------
    if (!leds_setup()) Serial.println("Failed to setup led's.");

    // -------------------------------------------
    // RELE SETUP
    // -------------------------------------------
    if(!rele_setup()) Serial.println("Failed to setup the rele.");

    // -------------------------------------------
    // BUTTON SETUP
    // -------------------------------------------
    if (!button_setup()) Serial.println("Failed to setup button.");
    
    // -------------------------------------------
    // SERVO DRIVER SETUP
    // -------------------------------------------
    if (!servo_driver_setup()) Serial.println("Failed to setup servo driver.");

    // -------------------------------------------
    // INA219 SETUP
    // -------------------------------------------
    if(!ina219_setup(SDA_INA, SCL_INA)) Serial.println("Failed to setup ina219.");
}


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


bool check_positional_servo_params(bool& servo_is_accurate, float& initial_servo_position, float& second_servo_position, int& expected_half_position, int& expected_full_position, float& actual_half_position, float& actual_full_position) {
    servo_is_accurate = false;
    
    if(!rotate_servo(0, 1000, 50)) return false;
    read_raw_angle();
    delay(200);
    initial_servo_position = degAngle;

    // send servo to 50%
    if(rotate_servo(50, 1000, 50)) return false;
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
    if(!rotate_servo(100, 1000, 50)) return false;
    read_raw_angle();
    delay(200);
    second_servo_position = degAngle;
    angle_difference = abs(second_servo_position - initial_servo_position);
    actual_full_position = angle_difference;

    return true;
}


void loop() {
    // TODO0: wait's a click to start program
    set_servo_led_state(1);
    set_running_led_state(0);
    set_wifi_led_state(0);

    bool is_positional_servo = false;

    bool button_state = get_button_state();
    if (!button_state) return;
    Serial.println("oi");
    
    set_rele_state(0);
    set_running_led_state(1);

    delay(200);
    float current = get_current_mA();
    
    if(get_current_mA() > 500.0) {
        set_rele_state(1);
        return;
    }

    // TODO1: check if is positional or continuos rotation servo
    float initial_servo_position = degAngle;
    if(!check_servo_type(is_positional_servo))return;

    // TODO2: if is continuos rotation check max velocity and zero velocity
    float second_servo_position = degAngle;
    float max_servo_velocity_deg_s;
    int lower_zero_vel_freq = 0;
    int upper_zero_vel_freq = 0;

    if (!is_positional_servo) {
        if(!check_continuos_rotation_params(initial_servo_position, second_servo_position, max_servo_velocity_deg_s, lower_zero_vel_freq, upper_zero_vel_freq))return;
    }

    // TODO3: if is positional check if it goes to write positions
    bool servo_is_accurate = false;
    int expected_half_position = 0;
    int expected_full_position = 0;
    float actual_half_position = 0;
    float actual_full_position = 0;

    if(is_positional_servo) {
        if(check_positional_servo_params(servo_is_accurate, initial_servo_position, second_servo_position, expected_half_position, expected_full_position, actual_half_position, actual_full_position))return;
    }

    // TODO4: send all to a webserver


}