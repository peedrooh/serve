#include "mqtt.h"

const char* BROKER_MQTT = "broker.hivemq.com"; // use if local broker doesn't work's
// const char* BROKER_MQTT = "192.168.0.123";
int BROKER_PORT = 1883;
WiFiClient espClient;
PubSubClient MQTT(espClient);
const int MQTT_MESSAGE_RATE = 1000;

void mqtt_setup() {
    MQTT.setKeepAlive(16960);
    MQTT.setServer(BROKER_MQTT, BROKER_PORT); 

    while (!MQTT.connected()) {
        if (MQTT.connect(MQTT_ID)) {
            Serial.println("Connected to MQTT broker");
        } else {
            Serial.print("Failed, rc=");
            Serial.print(MQTT.state());
            Serial.println(" Retrying MQTT connection...");
        }
    }
}

void reconnect_mqtt_broker(void* parameter) {
    for(;;) {
        vTaskDelay(4000);
        while(!MQTT.connected()) {
            String mqtt_id = MQTT_ID;
            mqtt_id += String(random(0xffff), HEX);

            if(MQTT.connect(mqtt_id.c_str())) {
                set_servo_led_state(1);
                Serial.print("Conectado no broker MQTT com o id ");
                Serial.println(mqtt_id);
            } else {
                set_servo_led_state(0);
                Serial.print("Failed, rc=");
                Serial.print(MQTT.state());
                Serial.println(" Retrying MQTT connection...");
            }
        }
    }
    
}