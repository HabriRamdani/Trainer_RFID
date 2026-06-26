// LCD1
#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>
#include <SPI.h>

#define TFT_CS   -1
#define TFT_DC    2
#define TFT_RST   4
Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);

// Input Teks
const String TEKS_BARIS_1 = "isi disini...";
const String TEKS_BARIS_2 = "isi disini...";
const String TEKS_BARIS_3 = "isi disini...";

void setup() {
  Serial.begin(115200);
  delay(1000);

  tft.init(240, 240, SPI_MODE3);
  tft.setRotation(3); // Rotasi layar LCD
 
  tft.fillScreen(ST77XX_BLACK);
  tft.drawRect(0, 0, 240, 240, ST77XX_BLUE);
  tft.drawRect(1, 1, 238, 238, ST77XX_BLUE);
 
  // Teks Baris 1
  tft.setTextColor(ST77XX_YELLOW); // Warna Teks
  tft.setTextSize(2); // Ukuran Font
  tft.setCursor(10, 20); // Koordinat Kursor
  tft.println(TEKS_BARIS_1); // Menampilkan teks sesuai dengan Baris 1
 
  // Teks Baris 2
  tft.setTextColor(ST77XX_WHITE);
  tft.setTextSize(2);
  tft.setCursor(20, 30);
  tft.println(TEKS_BARIS_2);
 
  // Teks Baris 3
  tft.setTextColor(ST77XX_CYAN);
  tft.setTextSize(2);
  tft.setCursor(30, 40);
  tft.println(TEKS_BARIS_3);
}

void loop() {
}
