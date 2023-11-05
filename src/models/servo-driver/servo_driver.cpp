#include "servo_driver.h"

extern bool servo_driver_setup() {
    pinMode(OPTO_PIN, OUTPUT);
    return true;
}

extern void servo_driver(int frequency_hz, int duty_cicle_percent) {
    float period_ms = 1000.0 / frequency_hz;
    digitalWrite(OPTO_PIN, HIGH);
    delay(period_ms * duty_cicle_percent/100.0);
    digitalWrite(OPTO_PIN, LOW);
    delay(period_ms * (1 - (duty_cicle_percent/100.0)));
}