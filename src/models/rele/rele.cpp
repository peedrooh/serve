#include "rele.h"

extern bool rele_setup() {
    pinMode(RELE_PIN, OUTPUT);
    return true;
}

extern void set_rele_state(bool state) {
    digitalWrite(RELE_PIN, state);
}