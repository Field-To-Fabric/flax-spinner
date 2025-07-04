/*
 Example using the SparkFun HX711 breakout board with a scale
 By: Nathan Seidle
 SparkFun Electronics
 Date: November 19th, 2014
 License: This code is public domain but you buy me a beer if you use this and we meet someday (Beerware license).

 This example demonstrates basic scale output. See the calibration sketch to get the calibration_factor for your
 specific load cell setup.

 This example code uses bogde's excellent library:"https://github.com/bogde/HX711"
 bogde's library is released under a GNU GENERAL PUBLIC LICENSE

 The HX711 does one thing well: read load cells. The breakout board is compatible with any wheat-stone bridge
 based load cell which should allow a user to measure everything from a few grams to tens of tons.
 Arduino pin 2 -> HX711 CLK
 3 -> DAT
 5V -> VCC
 GND -> GND

 The HX711 board can be powered from 2.7V to 5V so the Arduino 5V power should be fine.

*/

#include "HX711.h"

#define calibration_factor -406.0 //This value is obtained using the SparkFun_HX711_Calibration sketch

#define LOADCELL_DOUT_PIN  3
#define LOADCELL_SCK_PIN  4

HX711 scale;

float minChange = 0.5f;
long minChangeMs = 700;
float lastReading;
long lastReadingMillis = 0;
long totalStoppage = 0;
long pauseDetectionTimer = 0;
boolean pauseTimerActivated = false;


void setup() {
  Serial.begin(9600);

  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  scale.set_scale(calibration_factor); //This value is obtained by using the SparkFun_HX711_Calibration sketch
  scale.tare(); //Assuming there is no weight on the scale at start up, reset the scale to 0
}

void loop() {
  long currentMillis = millis();
  Serial.print(currentMillis);
  Serial.print(",");
  float reading = scale.get_units(5);
  Serial.print(reading, 1); //scale.get_units() returns a float
  Serial.println();
  lastReading = reading;
  lastReadingMs = currentMillis;
  handleSerialInput();
}

void handleSerialInput() {
  if(Serial.available()) {
    char temp = Serial.read();
    if (temp == 't') {
      scale.tare();
    }
  } 
}

// Pause detection algorithm
// Sample readings every X milliseconds. If more than 3 samples in a row return values within a range than we have a pause.
// Once one reading is out of the pause range then start again.
