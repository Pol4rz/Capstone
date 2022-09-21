#include <WiFi.h>
#include <PubSubClient.h>
#include <Arduino.h>
  
/****************************************
 * Define Constants
 ****************************************/
#define VARIABLE_LABEL "sensor" // Assing the variable label
#define DEVICE_LABEL "esp32" // Assig the device label
 
#define SENSOR 34 // Set the A0 as SENSOR
 
char payload[100];
char topic[150];
// Space to store values to send
char str_sensor[10];
 
/****************************************
 * Auxiliar Functions
 ****************************************/
WiFiClient ubidots;
PubSubClient client(ubidots);
 
 
/****************************************
 * Main Functions
 ****************************************/
void setup() {
  Serial.begin(115200);
  // Assign the pin as INPUT 
  pinMode(SENSOR, INPUT);

}
 
void loop() {


  sprintf(topic, "%s%s", "/v1.6/devices/", DEVICE_LABEL);
  sprintf(payload, "%s", ""); // Cleans the payload
  sprintf(payload, "{\"%s\":", VARIABLE_LABEL); // Adds the variable label
  
  float sensor = analogRead(SENSOR); 
  
  /* 4 is mininum width, 2 is precision; float value is copied onto str_sensor*/
  dtostrf(sensor, 4, 2, str_sensor);
  
  sprintf(payload, "%s {\"value\": %s}}", payload, str_sensor); // Adds the value
  delay(500);
}