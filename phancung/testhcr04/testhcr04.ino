#include <NewPing.h>

#define TRIGGER_PIN 27
#define ECHO_PIN 26
#define MAX_DISTANCE 200 // Maximum distance to measure in centimeters

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

void setup() {
  Serial.begin(9600); // Initialize serial communication
}

void loop() {
  delay(500); // Wait for a moment
  unsigned int distance = sonar.ping_cm(); // Measure distance in centimeters
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");
  delay(1500);
}