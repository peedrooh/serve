#ifndef MQTT_H
#define MQTT_H

#include <Arduino.h>
#include <PubSubClient.h>
#include <WiFi.h>

#include "../models/leds/leds.h"

#define SERVO_TYPE_TOPIC "serve/type"

#define CS_VELOCITY_TOPIC "serve/continious_servo/velocity"
#define CS_LOWER_ZERO_VEL_TOPIC "serve/continious_servo/lower_zero_vel"
#define CS_UPPER_ZERO_VEL_TOPIC "serve/continious_servo/upper_zero_vel"

#define PS_SERVO_IS_ACCURATE "serve/positional_servo/servo_is_accurate"
#define PS_SERVO_EXP_HALF_POS "serve/positional_servo/exp_half_pos"
#define PS_SERVO_ACT_HALF_POS "serve/positional_servo/act_half_pos"
#define PS_SERVO_EXP_FULL_POS "serve/positional_servo/exp_full_pos"
#define PS_SERVO_ACT_FULL_POS "serve/positional_servo/act_full_pos"

#define MQTT_ID  "serve"

extern const char* BROKER_MQTT;
extern WiFiClient espClient;
extern PubSubClient MQTT;
extern const int MQTT_MESSAGE_RATE;

extern void mqtt_setup();
extern void publish_mqtt_msg(void* parameter);
extern void reconnect_mqtt_broker(void* parameter);

#endif