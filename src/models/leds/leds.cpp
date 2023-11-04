#include "leds.h"

extern bool leds_setup() {
    pinMode(LED_RUNNING, OUTPUT);
    pinMode(LED_WIFI, OUTPUT);
    pinMode(LED_SERVO, OUTPUT);

    return true;
}

extern void set_wifi_led_state(bool state) {
    digitalWrite(LED_WIFI, state);
}

extern void set_running_led_state(bool state) {
    digitalWrite(LED_RUNNING, state);
}

extern void set_servo_led_state(bool state) {
    digitalWrite(LED_SERVO, state);
}