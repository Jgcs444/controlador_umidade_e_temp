#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <string.h>
#define pin1 0

const char *WIFI_SSID = "CLARO_2GBABDAB";
const char *WIFI_PASSWORD = "E7BABDAB";
 
const char *MQTT_HOST = "mqtt.eclipseprojects.io";
const int MQTT_PORT = 1883;
const char *TOPIC = "lab/temp";
const char *TOPIC2 = "lab/humid";
 
WiFiClient nodemcuClient01;
PubSubClient client(nodemcuClient01);
 
void callback(char* topic, byte* payload, unsigned int length)
{
    payload[length] = '\0';
    int value = String((char*) payload).toInt();

    if (strcmp (topic, TOPIC)== 0){
      Serial.println("TOPIC 1");
      Serial.println(topic);
      Serial.println(value);
      Serial.println("\n");
      func_temp(value);
    }
    else if (strcmp (topic, TOPIC2)== 0){
      Serial.println("TOPIC 2");
      Serial.println(topic);
      Serial.println(value);
      Serial.println("\n");
    }
    
}

void func_temp (int temp){
  if(temp > 15){
    digitalWrite(pin1, LOW);
    Serial.println(temp);
  }else{
    digitalWrite(pin1, HIGH);
  }
    }  
      
 void reconnect_MQTT(){
  while (!client.connected()) {
    client.connect("IoT_client");
    Serial.println("Reconnecting MQTT...");
  }
 }
  
void setup()
{
     pinMode(pin1, OUTPUT);
     
    Serial.begin(9600);
 
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
 
    Serial.println("Connected to Wi-Fi");
 
    client.setServer(MQTT_HOST, MQTT_PORT);
    client.setCallback(callback);
 
    while (!client.connected()) {
        reconnect_MQTT();
    }
 Serial.println("bla");
    client.subscribe(TOPIC);
    client.subscribe(TOPIC2);
}
 
void loop(){
    client.loop();
}
