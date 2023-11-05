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


    // // -------------------------------------------
    // // AMS5600 SETUP
    // // -------------------------------------------
    // ams5600_begin(SDA_AMS, SCL_AMS);
    // check_magnet_presence();
    // read_raw_angle();
    // startAngle = degAngle;

    // // -------------------------------------------
    // // LED'S SETUP
    // // -------------------------------------------
    // if (!leds_setup()) Serial.println("Failed to setup led's.");


    // // -------------------------------------------
    // // RELE SETUP
    // // -------------------------------------------
    // if(!rele_setup()) Serial.println("Failed to setup the rele.");

    // // -------------------------------------------
    // // BUTTON SETUP
    // // -------------------------------------------
    // if (!button_setup()) Serial.println("Failed to setup button.");
    
    // // -------------------------------------------
    // // SERVO DRIVER SETUP
    // // -------------------------------------------
    // if (!servo_driver_setup()) Serial.println("Failed to setup servo driver.");

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

    // -------------------------------------------
    // INA219 USAGE
    // -------------------------------------------
    get_shunt_voltage_mV();
    get_bus_voltage_V();
    get_current_mA();
    get_power_mW();
    get_load_voltage_V();
    
    Serial.print("Bus Voltage:   "); Serial.print(bus_voltage); Serial.println(" V");
    Serial.print("Shunt Voltage: "); Serial.print(shunt_voltage); Serial.println(" mV");
    Serial.print("Load Voltage:  "); Serial.print(load_voltage); Serial.println(" V");
    Serial.print("Current:       "); Serial.print(current_mA); Serial.println(" mA");
    Serial.print("Power:         "); Serial.print(power_mW); Serial.println(" mW");
    Serial.println("");

    delay(2000);
}