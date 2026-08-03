#include <HardwareSerial.h>

HardwareSerial ModemSerial(1);

static const int MODEM_RX = 17;
static const int MODEM_TX = 18;
static const uint32_t MODEM_BAUD = 115200;

void sendAT(const char *command) {
  Serial.print("Send: ");
  Serial.println(command);
  ModemSerial.print(command);
  ModemSerial.print("\r\n");
}

void setup() {
  Serial.begin(115200);
  delay(2000);
  ModemSerial.begin(MODEM_BAUD, SERIAL_8N1, MODEM_RX, MODEM_TX);

  Serial.println("A7670E UART test");
  Serial.println("Waiting for modem...");
  delay(5000);

  sendAT("AT");
  delay(1000);
  sendAT("ATE1");
  delay(1000);
  sendAT("AT+CPIN?");
  delay(1000);
  sendAT("AT+CSQ");
}

void loop() {
  while (ModemSerial.available()) {
    Serial.write(ModemSerial.read());
  }
  while (Serial.available()) {
    ModemSerial.write(Serial.read());
  }
}