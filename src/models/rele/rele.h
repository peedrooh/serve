#ifndef RELE_H
#define RELE_H

#include <Arduino.h>
#define RELE_PIN 1

extern bool rele_setup();
extern void set_rele_state(bool state);

#endif