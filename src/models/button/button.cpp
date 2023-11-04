#include "button.h"

extern bool button_setup() {
    pinMode(BUTTON_PIN, INPUT);
    return true;
}

extern bool get_button_state() {
    // The button is normally closed
    bool state = digitalRead(BUTTON_PIN);
    delay(50);

    return !state;
}