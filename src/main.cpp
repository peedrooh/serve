#include <Arduino.h>
#include <Wire.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include "models/ams5600/ams5600.h"
#include "models/leds/leds.h"
#include "models/rele/rele.h"
#include "models/button/button.h"
#include "models/servo-driver/servo_driver.h"
#include "models/ina219/ina219.h"
#include "servo_actuation/servo_actuation.h"
#include "wifi/wifi.h"
#include "mqtt/mqtt.h"

#define SDA_AMS 15
#define SCL_AMS 16

#define SDA_INA 42
#define SCL_INA 2

void servo_test_routine();
bool finished_running;

bool is_positional_servo = false;

float max_servo_velocity_deg_s;
int lower_zero_vel_freq;
int upper_zero_vel_freq;

bool servo_is_accurate;
int expected_half_position;
int expected_full_position;
float actual_half_position;
float actual_full_position;

void setup() {
    Serial.begin(115200);

    ams5600_begin(SDA_AMS, SCL_AMS);
    check_magnet_presence();
    read_raw_angle();
    startAngle = degAngle;

    if (!leds_setup()) Serial.println("Failed to setup led's.");
    if(!rele_setup()) Serial.println("Failed to setup the rele.");
    if (!button_setup()) Serial.println("Failed to setup button.");
    if (!servo_driver_setup()) Serial.println("Failed to setup servo driver.");
    if(!ina219_setup(SDA_INA, SCL_INA)) Serial.println("Failed to setup ina219.");
    
    set_rele_state(1);
    set_running_led_state(0);
    set_servo_led_state(0);
    set_wifi_led_state(0);

    delay(2000);
    wifi_setup();
    set_wifi_led_state(1);
    mqtt_setup();
    set_servo_led_state(1);

    xTaskCreate(
        reconnect_wifi,
        "WiFi Reconnector",
        30000,
        NULL,
        4,
        NULL
    );

    xTaskCreate(
        reconnect_mqtt_broker,
        "MQTT Reconnector",
        30000,
        NULL,
        3,
        NULL
    );

    xTaskCreate(
        publish_mqtt_msg,
        "MQTT Publisher",
        30000,
        NULL,
        2,
        NULL
    );

    // xTaskCreate(
    //     servo_test_routine,
    //     "Serve",
    //     30000,
    //     NULL,
    //     4,
    //     NULL
    // );
    
}


void loop() {
    servo_test_routine();
}

void publish_mqtt_msg(void* parameter) {
    for(;;){
        vTaskDelay(MQTT_MESSAGE_RATE);
        if(finished_running && is_positional_servo) {
            Serial.print("Meia pos. esperada: ");
            Serial.println(expected_half_position);
            Serial.print("Meia pos. medida: ");
            Serial.println(actual_half_position);
            Serial.print("Pos. Completa esperada: ");
            Serial.println(expected_full_position);
            Serial.print("Pos. Completa medida: ");
            Serial.println(actual_full_position);
            MQTT.publish(SERVO_TYPE_TOPIC, String(is_positional_servo).c_str());
            MQTT.publish(PS_SERVO_IS_ACCURATE, String(servo_is_accurate).c_str());
            MQTT.publish(PS_SERVO_EXP_HALF_POS, String(expected_half_position).c_str());
            MQTT.publish(PS_SERVO_ACT_HALF_POS, String(actual_half_position).c_str());
            MQTT.publish(PS_SERVO_EXP_FULL_POS, String(expected_full_position).c_str());
            MQTT.publish(PS_SERVO_ACT_FULL_POS, String(actual_full_position).c_str());            
            finished_running = false;
        }
        if(finished_running && !is_positional_servo) {
            Serial.print("Velocidade maxima(graus/s): ");
            Serial.println(max_servo_velocity_deg_s);
            Serial.print("Menor req. de vel. zero: ");
            Serial.println(lower_zero_vel_freq);
            Serial.print("Maior req. de vel. zero: ");
            Serial.println(upper_zero_vel_freq);
            MQTT.publish(SERVO_TYPE_TOPIC, String(is_positional_servo).c_str());
            MQTT.publish(CS_VELOCITY_TOPIC, String(max_servo_velocity_deg_s).c_str());
            MQTT.publish(CS_LOWER_ZERO_VEL_TOPIC, String(lower_zero_vel_freq).c_str());
            MQTT.publish(CS_UPPER_ZERO_VEL_TOPIC, String(upper_zero_vel_freq).c_str());
            finished_running = false;
        }
        MQTT.loop();
  }
}

void servo_test_routine() {
    vTaskDelay(100);
    for(;;) {
        // TODO0: wait's a click to start program
        set_running_led_state(0);
        set_rele_state(1);


        bool button_state = get_button_state();
        if (!button_state) return;
        finished_running = false;
            
        set_rele_state(0);
        set_running_led_state(1);

        // TODO1: check if is positional or continuos rotation servo
        float initial_servo_position = degAngle;
        if(!check_servo_type(is_positional_servo))return;

        // TODO2: if is continuos rotation check max velocity and zero velocity
        float second_servo_position = degAngle;
        max_servo_velocity_deg_s;
        lower_zero_vel_freq = 0;
        upper_zero_vel_freq = 0;

        if (!is_positional_servo) {
            if(!check_continuos_rotation_params(initial_servo_position, second_servo_position, max_servo_velocity_deg_s, lower_zero_vel_freq, upper_zero_vel_freq)) return;
        }

        // TODO3: if is positional check if it goes to write positions
        servo_is_accurate = false;
        expected_half_position = 0;
        expected_full_position = 0;
        actual_half_position = 0;
        actual_full_position = 0;

        if(is_positional_servo) {
            if(!check_positional_servo_params(servo_is_accurate, initial_servo_position, second_servo_position, expected_half_position, expected_full_position, actual_half_position, actual_full_position))return;
        }
        finished_running = true;
    }
}