#include <Arduino.h>


#ifndef LED_RUNNING
  #define LED_RUNNING 5
#endif

#ifndef LED_WIFI
  #define LED_WIFI 6
#endif

#ifndef LED_SERVO
  #define LED_SERVO 7
#endif

bool leds_setup() {
  pinMode(LED_RUNNING, OUTPUT);
  pinMode(LED_WIFI, OUTPUT);
  pinMode(LED_SERVO, OUTPUT);

  return true;
}

void setup() {
  Serial.begin(115200);
  if (!leds_setup()) {
    Serial.println("Failed to setup led's.");
  }
}

void loop() {
  
  digitalWrite(LED_WIFI, HIGH);
  digitalWrite(LED_RUNNING, HIGH);
  digitalWrite(LED_SERVO, HIGH);

  delay(1000);

  digitalWrite(LED_WIFI, LOW);
  digitalWrite(LED_RUNNING, LOW);
  digitalWrite(LED_SERVO, LOW);

  delay(1000);
}
