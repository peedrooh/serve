#include "servo_driver.h"
#include "../ina219/ina219.h"
#include "../rele/rele.h"

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

extern bool rotate_servo(int position_percentage, int duration_ms, int frequency_hz) {
    float period_ms = 1000.0 / frequency_hz;
    unsigned long start_time = millis();
    unsigned long current_time = millis();
    while(current_time - start_time < duration_ms) {
        digitalWrite(OPTO_PIN, HIGH);
        delayMicroseconds( (1000.0*position_percentage / 100) + 1000.0);
        digitalWrite(OPTO_PIN, LOW);
        delay(period_ms - ((1.0 * (position_percentage/100)) + 1));
        current_time = millis();
        if(get_current_mA() > 1500.0) {
            set_rele_state(1);
            return false;
        }
    }
    return true;
}