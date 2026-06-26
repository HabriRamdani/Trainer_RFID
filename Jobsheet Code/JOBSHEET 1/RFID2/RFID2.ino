// RFID2
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

// Daftar Nama
struct Siswa {
  String uid;
  String nama;
};

Siswa daftarSiswa[] = {
  {"96906FB8", "BAHLUL"},
  {"991E7C12", "RELAY 1CH 5V"},
  {"058931B81D1300", "SIMPLICITY"},
  {"72A6FF0A", "PARAGON"},
  {"D287020B", "DIMAS"},
  {"040C559A687780", "DIMSUM"}
};
int jumlahSiswa = 6;

String CariNamaSiswa(String uid) {
  for (int i = 0; i < jumlahSiswa; i++) {
    if (daftarSiswa[i].uid == uid) {
      return daftarSiswa[i].nama;
    }
  }
  return "TIDAK DIKENAL";
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  hspi.begin(RFID_SCK_PIN, RFID_MISO_PIN, RFID_MOSI_PIN, RFID_SS_PIN);
  rfid.PCD_Init();
  delay(100);

  Serial.println("Silakan Dekatkan Kartu/Tag RFID Anda");
}

void loop() {
  if (millis() - lastReadTime < READ_INTERVAL) return;
  lastReadTime = millis();

  if (!rfid.PICC_IsNewCardPresent()) return;
  if (!rfid.PICC_ReadCardSerial()) return;

  String uid = formatUID(rfid.uid.uidByte, rfid.uid.size);
  String nama = CariNamaSiswa(uid);
 
  Serial.print("Terbaca -> Nama: " + nama + " | UID: ");
  Serial.println(uid);

  rfid.PICC_HaltA();
  rfid.PCD_StopCrypto1();
}
