// RFID1
#include <SPI.h>
#include <MFRC522v2.h>
#include <MFRC522DriverSPI.h>
#include <MFRC522DriverPinSimple.h>

#define RFID_SCK_PIN  14
#define RFID_MISO_PIN 12
#define RFID_MOSI_PIN 13
#define RFID_SS_PIN    5
#define RFID_RST_PIN  22

SPIClass hspi(HSPI);
MFRC522DriverPinSimple rfidSsPin(RFID_SS_PIN);
MFRC522DriverSPI rfidDriver{rfidSsPin, hspi};
MFRC522 rfid{rfidDriver};

unsigned long lastReadTime = 0;
const unsigned long READ_INTERVAL = 200;

String formatUID(byte *uid, byte uidSize) {
  String s = "";
  for (byte i = 0; i < uidSize; i++) {
    if (uid[i] < 0x10) s += "0";
    s += String(uid[i], HEX);
  }
  s.toUpperCase();
  return s;
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  hspi.begin(RFID_SCK_PIN, RFID_MISO_PIN, RFID_MOSI_PIN, RFID_SS_PIN);
  rfid.PCD_Init();
  delay(100);

  Serial.println("Dekatkan Kartu/Tag RFID Anda...");
}

void loop() {
  if (millis() - lastReadTime < READ_INTERVAL) return;
  lastReadTime = millis();

  if (!rfid.PICC_IsNewCardPresent()) return;
  if (!rfid.PICC_ReadCardSerial()) return;

  String uid = formatUID(rfid.uid.uidByte, rfid.uid.size);
 
  Serial.print("Terbaca, UID: ");
  Serial.println(uid);

  rfid.PICC_HaltA();
  rfid.PCD_StopCrypto1();
}
