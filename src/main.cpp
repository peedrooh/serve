#include <Arduino.h>
#include <Wire.h>

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

void loop() {
    // // -------------------------------------------
    // // AMS5600 USAGE
    // // -------------------------------------------
    // read_raw_angle(); // get a relative position (an angle from 0 to 360 deg)
    // correct_angle(); // get the absolute position since statup
    // check_quadrant(); // check current quadrant
    
    // // -------------------------------------------
    // // TURN LED'S AND RELE ON AND OFF
    // // -------------------------------------------
    // set_running_led_state(1);
    // set_servo_led_state(1);
    // set_wifi_led_state(1);
    // set_rele_state(1);
    // delay(1000);
    // set_running_led_state(0);
    // set_servo_led_state(0);
    // set_wifi_led_state(0);
    // set_rele_state(0);
    // delay(1000);
    // // -------------------------------

    // // -------------------------------------------
    // // BUTTON USAGE
    // // -------------------------------------------
    // bool state = get_button_state();
    // Serial.println(state);

    // // -------------------------------------------
    // // OPTO USAGE
    // // -------------------------------------------
    // servo_driver(50, 10);

    // // -------------------------------------------
    // // INA219 USAGE
    // // -------------------------------------------
    // get_shunt_voltage_mV();
    // get_bus_voltage_V();
    // get_current_mA();
    // get_power_mW();
    // get_load_voltage_V();
    
    // Serial.print("Bus Voltage:   "); Serial.print(bus_voltage); Serial.println(" V");
    // Serial.print("Shunt Voltage: "); Serial.print(shunt_voltage); Serial.println(" mV");
    // Serial.print("Load Voltage:  "); Serial.print(load_voltage); Serial.println(" V");
    // Serial.print("Current:       "); Serial.print(current_mA); Serial.println(" mA");
    // Serial.print("Power:         "); Serial.print(power_mW); Serial.println(" mW");
    // Serial.println("");

    // delay(2000);

    // -------------------------------------------
    // MAIN LOGIC
    // -------------------------------------------
    bool is_running = false;
    unsigned long time_in_millis = millis();
    unsigned long prev_time_millis = millis();

    if(get_current_mA() > 1000.0) {
        set_rele_state(0);
        set_running_led_state(0);
        return;
    }

    if(!is_running && get_button_state()) {
        is_running = true;
        delay(500);
        return;
    }

    // Initial encoder position
    read_raw_angle();
    float initial_servo_position = degAngle;

    // DEAD ZONE CHECK
    int lower_limit_freq = 0;
    int upper_limit_freq = 100;
    bool is_lower_limit = true;
    bool angle_toggle = false;

    for(int freq = 2; freq < 100; freq = freq + 2) {
        prev_time_millis = millis();
        time_in_millis = millis();
        initial_servo_position = degAngle;
        while(time_in_millis - prev_time_millis < (1000/freq) * 10) {
            if (angle_toggle) {
                angle_toggle = !angle_toggle;
                servo_driver(freq, 60);
            }
            else {
                angle_toggle = !angle_toggle;
                servo_driver(freq, 50);
            }
            time_in_millis = millis();
        }
        read_raw_angle();

        if(abs(initial_servo_position - degAngle) > 5  && is_lower_limit) {
            lower_limit_freq = freq;
            is_lower_limit = false;
        }

        if(!is_lower_limit && abs(initial_servo_position - degAngle) < 1) {
            upper_limit_freq = freq;
            freq = 100;
            break;
        }
        // Serial.println(freq);
        delay(100);
    }
    // Serial.println(lower_limit_freq);
    // Serial.println(upper_limit_freq);


    // SERVO POSITION VERIFICATION
    bool servo_is_accurate = false;
    servo_driver(50, 0);
    delay(1000);
    initial_servo_position = degAngle;

    servo_driver(50, 25);
    delay(1000);
    float delta_servo_angle = abs(degAngle - initial_servo_position);
    if(delta_servo_angle >= 87.0 && delta_servo_angle <= 93.0) {
        servo_is_accurate = true;
    } else if (delta_servo_angle >= 42.0 && delta_servo_angle <= 48.0) {
        servo_is_accurate = true;
    } else if (delta_servo_angle >= 20.0 && delta_servo_angle <= 26.0) {
        servo_is_accurate = true;
    } else {
        servo_is_accurate = false;
    }

    servo_driver(50, 50);
    delay(1000);
    float delta_servo_angle = abs(degAngle - initial_servo_position);
    if(delta_servo_angle >= 177.0 && delta_servo_angle <= 183.0) {
        servo_is_accurate = true;
    } else if (delta_servo_angle >= 87.0 && delta_servo_angle <= 93.0) {
        servo_is_accurate = true;
    } else if (delta_servo_angle >= 42.0  && delta_servo_angle <= 48.0) {
        servo_is_accurate = true;
    } else {
        servo_is_accurate = false;
    }

    Serial.println(servo_is_accurate);
    // TODO1: improve servo_drive, so that is not a delay but it waits the servo goes to write position or 
    // maximun time and checks if current is not too high
    // TODO2: send data to webserver that display if servo is good or not
}