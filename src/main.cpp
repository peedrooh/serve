#include <Arduino.h>
#include <Wire.h>

#include "models/ams5600/ams5600.h"
#include "models/leds/leds.h"
#include "models/rele/rele.h"
#include "models/button/button.h"

#define SDA_AMS 15
#define SCL_AMS 16

void setup() {
    Serial.begin(115200);
    pinMode(4, INPUT);
    pinMode(2, INPUT);
    pinMode(18, INPUT);

    // -------------------------------------------
    // AMS5600 SETUP
    // -------------------------------------------
    // ams5600_begin(SDA_AMS, SCL_AMS);
    // check_magnet_presence();
    // read_raw_angle();
    // startAngle = degAngle;

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
    bool state = get_button_state();
    Serial.println(state);
}
