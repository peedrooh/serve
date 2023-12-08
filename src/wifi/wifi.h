#ifndef WIFI_H
#define WIFI_H

#include <Arduino.h>
#include <WiFi.h>
#include <esp_wifi.h>

#include "../models/leds/leds.h"

// #define UTFPR
#ifdef UTFPR
    #define EAP_SSID "UTFPR-ALUNO"
    #define EAP_IDENTITY "a2139146"
    #define EAP_USERNAME "a2139146"
    #define EAP_PASSWORD "xxx"
#else
    #define EAP_SSID "Wanderley"
    #define EAP_PASSWORD "crm15210"
#endif

extern void wifi_setup();
extern void reconnect_wifi(void* parameter);

#endif