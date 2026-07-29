// MAARINA Phase 1 — Button Detection
// Press SOS button → Serial Monitor confirms it. For Martha.

#define BUTTON_PIN 3  // IO3 on the board

void setup() {
  Serial.begin(115200);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  Serial.println("MAARINA ready. Press the SOS button.");
}

void loop() {
  if (digitalRead(BUTTON_PIN) == LOW) {
    Serial.println("BUTTON PRESSED — SOS triggered!");
    delay(500); // prevents multiple triggers per press
  }
}