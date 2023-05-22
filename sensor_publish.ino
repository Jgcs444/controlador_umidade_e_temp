
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "DHT.h"

//DHT Area
#define DHTPIN 0 // set the pin that DHT is connected to
#define DHTTYPE DHT11 // set the DHT version
DHT dht(DHTPIN, DHTTYPE); // declares the DHT object
int t,h;

//Wifi area
const char* ssid = "CLARO_2GBABDAB"; // Set here SSID of your wifi network
const char* password = "E7BABDAB"; // Password of your wifi network
const char* mqtt_broker = "mqtt.eclipseprojects.io"; // Change for your MQTT Broker prefered
WiFiClient nodemcuClient01;
PubSubClient client(nodemcuClient01);

void setup() {
  Serial.begin(9600);
  dht.begin(); // initialize dht
  Serial.println("\n\n***************************************");
  Serial.println(" Initializing IoT Termometer...");
  wifi_connect();
  client.setServer(mqtt_broker,1883); // Set your MQTT server and port
}

void loop() {
  if (!client.connected()) {
    reconnect_MQTT();
  }
  DHT_readings();
  publish_MQTT();
  delay(5000);
}

// Function establishes connection with wifi network
void wifi_connect(){
  delay(10);
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

//Seeks to re-establish the connection with the MQTT broker
void reconnect_MQTT(){
  while (!client.connected()) {
    client.connect("IoT_Termometer01");
    Serial.println("Reconnecting MQTT...");
  }
  publish_MQTT();
}

// Function that takes dht readings and show results
void DHT_readings(){
  // Reading temperature or humidity takes about 250 milliseconds!
  h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  //float f = dht.readTemperature(true);

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

}

// Function publish readings in MQTT topic
void publish_MQTT(){
  // The true parameter persist information in topic
  client.publish("lab/temp", String(t).c_str()); // CHANGE THE TOPIC
  client.publish("lab/humid", String(h).c_str()); // CHANGE THE TOPIC

  // A series of prints to show readings
  Serial.println("\n---------------------------------------");
  Serial.println("Temperature and Humidity Readings: \n");
  Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.print(F("%  Temperature: "));
  Serial.print(t);
  Serial.println(F("°C "));

  // Shows publication success in the MQTT topic blinking internal led
}
