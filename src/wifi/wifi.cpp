#include "wifi.h"

uint8_t target_esp_mac [6] = {0x24 , 0x0a , 0x44 , 0x9a , 0x38 , 0x28};

void wifi_setup(){
  
    WiFi.mode(WIFI_STA);
    esp_wifi_set_mac(WIFI_IF_STA, target_esp_mac);
    
    #ifdef UTFPR
    WiFi.begin(EAP_SSID , WPA2_AUTH_PEAP, EAP_IDENTITY , EAP_USERNAME, EAP_PASSWORD);
    #else
    WiFi.begin(EAP_SSID , EAP_PASSWORD);
    #endif

    Serial.println(EAP_SSID) ;
    Serial.print( "Trying to connect") ;
    while(WiFi.status()!= WL_CONNECTED){
        Serial.print(".") ;
        delay(400);
    }
    Serial.println("");
    Serial.print("Connected to local Wifi as IP ");
    Serial.println(WiFi.localIP());
  
}

void reconnect_wifi(void* parameter) {
    for(;;) {
        vTaskDelay(2000);
        if(WiFi.status() != WL_CONNECTED) {
            Serial.print( "Trying to connect") ;
            while(WiFi.status()!= WL_CONNECTED){
                Serial.print(".") ;
                vTaskDelay(400);
            }
            Serial.println("");
            Serial.print("Connected to local Wifi as IP ");
            Serial.println(WiFi.localIP());
        }
    }
}