int PHOTO_SWITCH_PIN = 3;

int photoSwitchVal = 0;
// Value is high when the sensor is blocked
long lastSwitchToHigh = 0;
// Value is low when the sensor is not blocked
long lastSwitchToLow = 0;

void setup() {
  Serial.begin(9600);
  pinMode(PHOTO_SWITCH_PIN, INPUT_PULLUP);
}

void loop() {
  if (digitalRead(PHOTO_SWITCH_PIN) == HIGH) {
    if (photoSwitchVal == LOW) {
      // Change detected, LOW to HIGH
      photoSwitchVal = HIGH;
      long now = millis();
      long interval = now - lastSwitchToLow;
      Serial.print("Time since last passage: ");
      Serial.println(interval);
      lastSwitchToHigh = now;
    }
  } else {
    if (photoSwitchVal == HIGH) {
      // Change detected, HIGH to LOW
      photoSwitchVal = LOW;
      long now = millis();
      long interval = now - lastSwitchToHigh;
      Serial.print("Time blocked: ");
      Serial.println(interval);
      lastSwitchToLow = now;
    }
  }
}