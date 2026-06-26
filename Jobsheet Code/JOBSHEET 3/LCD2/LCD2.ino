// LCD2
#include <SPI.h>
#include <MFRC522v2.h>
#include <MFRC522DriverSPI.h>
#include <MFRC522DriverPinSimple.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>

#define TFT_CS   -1
#define TFT_DC    2
#define TFT_RST   4
Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);

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
const unsigned long DISPLAY_DURATION = 2000; // Jeda 2 detik sebelum teks hilang

bool cardTapped = false;
unsigned long tapTimestamp = 0;

// DAFTAR NAMA
struct Siswa {
  String uid;
  String nama;
};
Siswa daftarSiswa[] = {
  {"9690  6FB8", "BAHLUL"},
  {"991E7C12", "RELAY 1CH 5V"},
  {"058931B81D1300", "SIMPLICITY"},
  {"72A6FF0A", "PARAGON"},
  {"D287020B", "DIMAS"},
  {"040C559A687780", "DIMSUM"}
};
int jumlahSiswa = 6;

String formatUID(byte *uid, byte uidSize) {
  String s = "";
  for (byte i = 0; i < uidSize; i++) {
    if (uid[i] < 0x10) s += "0";
    s += String(uid[i], HEX);
  }
  s.toUpperCase();
  return s;
}

String CariNamaSiswa(String uid) {
  for (int i = 0; i < jumlahSiswa; i++) {
    if (daftarSiswa[i].uid == uid) {
      return daftarSiswa[i].nama;
    }
  }
  return "TIDAK DIKENAL";
}

void TampilkanDefaultLayar() {
  tft.fillScreen(ST77XX_BLACK);
  tft.drawRect(0, 0, 240, 240, ST77XX_BLUE);
  tft.drawRect(1, 1, 238, 238, ST77XX_BLUE);
 
  tft.setTextColor(ST77XX_YELLOW);
  tft.setTextSize(2);
  tft.setCursor(45, 50);
  tft.print("TRAINER RFID");
 
  tft.setTextColor(ST77XX_WHITE);
  tft.setTextSize(2);
  tft.setCursor(55, 110);
  tft.print("SILAKAN TAP");
  tft.setCursor(60, 140);
  tft.print("KARTU ANDA");
}

void TampilkanDataDiLCD(String nama, String uid, bool sukses) {
  tft.fillScreen(ST77XX_BLACK);
 
  if (sukses) {
    tft.fillRect(0, 0, 240, 50, ST77XX_GREEN);
    tft.setTextColor(ST77XX_BLACK);
    tft.setTextSize(2);
    tft.setCursor(60, 18);
    tft.print("BERHASIL!");
 
    tft.setTextColor(ST77XX_WHITE);
    tft.setTextSize(3);
    tft.setCursor(15, 80);
    tft.print(nama);
  } else {
    tft.fillRect(0, 0, 240, 50, ST77XX_RED);
    tft.setTextColor(ST77XX_WHITE);
    tft.setTextSize(2);
    tft.setCursor(45, 18);
    tft.print("KARTU ASING");
 
    tft.setTextColor(ST77XX_RED);
    tft.setTextSize(2);
    tft.setCursor(15, 80);
    tft.print("TIDAK DIKENAL");
  }
 
  tft.setTextColor(ST77XX_CYAN);
  tft.setTextSize(2);
  tft.setCursor(15, 160);
  tft.print("UID: " + uid);
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  tft.init(240, 240, SPI_MODE3);
  tft.setRotation(3);
  TampilkanDefaultLayar();

  hspi.begin(RFID_SCK_PIN, RFID_MISO_PIN, RFID_MOSI_PIN, RFID_SS_PIN);
  rfid.PCD_Init();
  delay(100);

  Serial.println("Silakan Dekatkan Kartu/Tag RFID Anda...");
}

void loop() {
  if (cardTapped && (millis() - tapTimestamp >= DISPLAY_DURATION)) {
    TampilkanDefaultLayar();
    cardTapped = false;
  }

  if (millis() - lastReadTime < READ_INTERVAL) return;
  lastReadTime = millis();

  if (!rfid.PICC_IsNewCardPresent()) return;
  if (!rfid.PICC_ReadCardSerial()) return;

  String uid = formatUID(rfid.uid.uidByte, rfid.uid.size);
  String nama = CariNamaSiswa(uid);

  if (nama == "TIDAK DIKENAL") {
    TampilkanDataDiLCD(nama, uid, false);
  } else {
    TampilkanDataDiLCD(nama, uid, true);
  }

  Serial.println("Terbaca -> Nama: " + nama + " | UID: " + uid);

  cardTapped = true;
  tapTimestamp = millis();

  rfid.PICC_HaltA();
  rfid.PCD_StopCrypto1();
