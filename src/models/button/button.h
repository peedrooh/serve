#ifndef BUTTON_H
#define BUTTON_H

#include <Arduino.h>

#define BUTTON_PIN 18

extern bool button_setup();
extern bool get_button_state();

#endif