  // MAARINA — Phase 1: SOS button -> voice call via A7670E
  // Board: Waveshare ESP32-S3-A7670E-4G
  // TODO before committing: move SOS_NUMBER into config.h — do not push to GitHub with a real number hardcoded.

  #include <HardwareSerial.h>

  // ---- Pins ----
  // Confirm MODEM_RX / MODEM_TX against your WIRING_GUIDE.md — these are the
  // common Waveshare A7670E UART pins, but double-check before flashing.
  #define MODEM_RX 18   // A7670E TXD -> ESP32 RX
  #define MODEM_TX 17   // A7670E RXD -> ESP32 TX
  #define SOS_BUTTON_PIN 3   // IO3, wired to GND with INPUT_PULLUP
  #define LED_PIN 38

  HardwareSerial SerialAT(1);  // UART1 for the modem

  // ---- Config (move to config.h) ----
  const char* SOS_NUMBER = "+919930824773";  // <-- replace with your test number

  bool modemReady = false;

  // Sends one AT command, prints whatever comes back for `timeout` ms
  void sendAT(const String &cmd, unsigned long timeout = 3000) {
    SerialAT.println(cmd);
    unsigned long start = millis();
    while (millis() - start < timeout) {
      while (SerialAT.available()) {
        Serial.write(SerialAT.read());
      }
    }
  }

  // Tries "AT" up to 3 times, waiting for "OK" — this is the autobaud loop
  // that fixed modem bring-up earlier.
  bool autobaudSync() {
    for (int i = 0; i < 3; i++) {
      SerialAT.println("AT");
      delay(500);
      if (SerialAT.available()) {
        String resp = SerialAT.readString();
        Serial.println(resp);
        if (resp.indexOf("OK") != -1) return true;
      }
    }
    return false;
  }

  void dialNumber(const char* number) {
    Serial.print("Dialing: ");
    Serial.println(number);
    String cmd = "ATD" + String(number) + ";";
    sendAT(cmd, 5000);   // voice call setup takes a few seconds to respond
  }

  void setup() {
    Serial.begin(115200);
    pinMode(SOS_BUTTON_PIN, INPUT_PULLUP);
    pinMode(LED_PIN, OUTPUT);

    // Power on the A7670E modem
pinMode(4, OUTPUT);
digitalWrite(4, LOW);
delay(100);
digitalWrite(4, HIGH);
delay(2000);
digitalWrite(4, LOW);

    Serial.println("Booting modem — waiting 15s...");
    delay(15000);

    SerialAT.begin(115200, SERIAL_8N1, MODEM_RX, MODEM_TX);

    modemReady = autobaudSync();
    if (modemReady) {
      Serial.println("Modem responding.");
      sendAT("AT+CPIN?");   // SIM unlocked?
      sendAT("AT+CSQ");     // signal strength
    } else {
      Serial.println("Modem NOT responding — check power/wiring before going further.");
    }
  }

  void loop() {
    if (modemReady && digitalRead(SOS_BUTTON_PIN) == LOW) {
      digitalWrite(LED_PIN, HIGH);
      dialNumber(SOS_NUMBER);
      delay(20000);           // cooldown so it doesn't redial itself in a loop
      digitalWrite(LED_PIN, LOW);
    }
  }
