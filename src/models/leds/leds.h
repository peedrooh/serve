#ifndef LEDS_H
#define LEDS_H

#include <Arduino.h>

#define LED_RUNNING 5
#define LED_WIFI 6
#define LED_SERVO 7

extern bool leds_setup();
extern void set_wifi_led_state(bool state);
extern void set_running_led_state(bool state);
extern void set_servo_led_state(bool state);

#endif