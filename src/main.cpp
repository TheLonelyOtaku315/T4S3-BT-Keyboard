#include <Arduino.h>

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("===================");
  Serial.println("T4-S3 AMOLED Ready!");
  Serial.println("===================");
}

void loop() {
  Serial.println("Running...");
  delay(2000);
}