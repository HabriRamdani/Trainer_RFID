#include <WiFi.h>
#include <WebServer.h>
#include <SPI.h>
#include <MFRC522v2.h>
#include <MFRC522DriverSPI.h>
#include <MFRC522DriverPinSimple.h>
#include <HTTPClient.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

// KONFIGURASI TAMPILAN TABEL
const String MODE_TAMPILAN = "range";       // 'full', atau 'range'
int START_MONTH = 0;                       // 0=Jan, 1=Feb, ... 11=Des
const int END_MONTH = 5;                   // 0=Jan, 1=Feb, ... 11=Des
const String JUDUL_TABEL = "TRAINER RFID"; // Judul tabel & nama file
const int TAHUN = 2026;                    // Tahun
 
// KONFIGURASI WIFI
const char* ssid     = "KOST CERIA";
const char* password = "jangangalausaja";
const char* googleScriptURL = "https://script.google.com/macros/s/AKfycbzV5iuNVPIL-zDo0IxPz3qMwQp2BPmIMBHRkcNc4lYz8JyZPbQdmNeVrBbzaMihHvHd3A/exec";
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 7 * 3600, 60000);

#define RFID_SCK_PIN  14
#define RFID_MISO_PIN 12
#define RFID_MOSI_PIN 13
#define RFID_SS_PIN    5
#define RFID_RST_PIN  22

// DATABASE NAMA
struct Siswa {
  String uid;
  String nama;
};
Siswa daftarSiswa[] = {
  {"96906FB8", "BAHLUL"},
  {"991E7C12", "RELAY 1CH 5VOLT MINI"},
  {"058931B81D1300", "SIMPLICITY"},
  {"72A6FF0A", "PARAGON"},
  {"D287020B", "DIMAS"},
  {"040C559A687780", "DIMSUM"}
};
int jumlahSiswa = 6;

SPIClass hspi(HSPI);
MFRC522DriverPinSimple rfidSsPin(RFID_SS_PIN);
MFRC522DriverSPI rfidDriver{rfidSsPin, hspi};
MFRC522 rfid{rfidDriver};

WebServer server(80);

unsigned long lastReadTime = 0;
const unsigned long READ_INTERVAL = 200;
const unsigned long DISPLAY_DURATION = 2000;
bool cardTapped = false;
unsigned long tapTimestamp = 0;
bool cardBeingProcessed = false;

String cachedData = "[]";
unsigned long lastCacheUpdate = 0;
const unsigned long CACHE_INTERVAL = 60000;

String lastTapHistory[2] = {"", ""};
String lastTapTime[2] = {"", ""};

bool isSending = false;

String getModeConfigJSON() {
  String config = "{";
  config += "\"mode\":\"" + MODE_TAMPILAN + "\",";
  config += "\"startMonth\":" + String(START_MONTH) + ",";
  config += "\"endMonth\":" + String(END_MONTH) + ",";
  config += "\"judul\":\"" + JUDUL_TABEL + "\",";
  config += "\"tahun\":" + String(TAHUN);
  config += "}";
  return config;
}

String AmbilHalamanWeb();
String formatUID(byte *uid, byte uidSize);
String CariNamaSiswa(String uid);
void kirimKeGoogleSheets(String nama, String uid, String tgl, String jam);
String urlencode(String str);
String ambilDataDariGoogleSheets();
void updateCacheData();
void updateLastTaps(String nama, String waktu);

// URLENCODE
String urlencode(String str) {
  String encoded = "";
  char c;
  for (int i = 0; i < str.length(); i++) {
    c = str.charAt(i);
    if (c == ' ') {
      encoded += "%20";
    } else if (isalnum(c)) {
      encoded += c;
    } else {
      char hex[4];
      sprintf(hex, "%%%02X", c);
      encoded += hex;
    }
  }
  return encoded;
}

// TIME
String getCurrentTime() {
  if (WiFi.status() == WL_CONNECTED && timeClient.isTimeSet()) {
    return timeClient.getFormattedTime();
  }
  unsigned long s = millis() / 1000;
  char buf[9];
  sprintf(buf, "%02lu:%02lu:%02lu", (s / 3600) % 24, (s / 60) % 60, s % 60);
  return String(buf);
}

String getCurrentDate() {
  if (WiFi.status() == WL_CONNECTED && timeClient.isTimeSet()) {
    time_t epochTime = timeClient.getEpochTime();
    struct tm *ptm = localtime(&epochTime);
    char buf[11];
    sprintf(buf, "%02d/%02d/%04d", ptm->tm_mday, ptm->tm_mon + 1, ptm->tm_year + 1900);
    return String(buf);
  }
  unsigned long days = millis() / (1000UL * 60 * 60 * 24);
  int day = (days % 30) + 1;
  int month = (days / 30) % 12 + 1;
  int year = 2026;
  char buf[11];
  sprintf(buf, "%02d/%02d/%04d", day, month, year);
  return String(buf);
}

String getCurrentWeek() {
  if (WiFi.status() == WL_CONNECTED && timeClient.isTimeSet()) {
    time_t epochTime = timeClient.getEpochTime();
    struct tm *ptm = localtime(&epochTime);
    int dayOfMonth = ptm->tm_mday;
    return String((dayOfMonth - 1) / 7 + 1);
  }
  unsigned long days = millis() / (1000UL * 60 * 60 * 24);
  return String((days / 7) + 1);
}

// KIRIM DATA KE GOOGLE SHEETS
void kirimKeGoogleSheets(String nama, String uid, String tgl, String jam) {
  if (WiFi.status() != WL_CONNECTED) {
    return;
  }
  
  if (isSending) {
    return;
  }
  isSending = true;
  
  HTTPClient http;
  String url = String(googleScriptURL) + 
               "?nama=" + urlencode(nama) +
               "&uid=" + urlencode(uid) +
               "&tanggal=" + urlencode(tgl) +
               "&jam=" + urlencode(jam);
  
  http.begin(url);
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  http.setTimeout(1000); 
  
  int httpCode = http.GET();
  
  if (httpCode == 200 || httpCode == 302 || httpCode == -11) {
    Serial.println("[GS] Data terkirim!");
  }
  
  http.end();
  isSending = false;
}

// AMBIL DATA DARI GOOGLE SHEETS
String ambilDataDariGoogleSheets() {
  if (WiFi.status() != WL_CONNECTED) {
    return "[]";
  }
  
  HTTPClient http;
  String url = String(googleScriptURL) + 
               "?action=getData" +
               "&startMonth=" + String(START_MONTH) +
               "&endMonth=" + String(END_MONTH) +
               "&tahun=" + String(TAHUN);
  
  http.begin(url);
  http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
  http.addHeader("Content-Type", "application/json");
  http.setTimeout(5000);
  
  int httpCode = http.GET();
  String response = "[]";
  
  if (httpCode == 200) {
    response = http.getString();
  } 
  http.end();
  
  return response;
}

void updateCacheData() {
  if (WiFi.status() == WL_CONNECTED) {
    String data = ambilDataDariGoogleSheets();
    if (data != "[]" && data.length() > 5) {
      cachedData = data;
      lastCacheUpdate = millis();
    }
  }
}

// LAST TAPS
void updateLastTaps(String nama, String waktu) {
  lastTapHistory[1] = lastTapHistory[0];
  lastTapTime[1] = lastTapTime[0];
  lastTapHistory[0] = nama;
  lastTapTime[0] = waktu;
}

// RFID
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

// ROUTING SERVER
void handleRoot() {
  server.send(200, "text/html", AmbilHalamanWeb());
}

void handleGetConfig() {
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.sendHeader("Cache-Control", "no-cache");
  server.send(200, "application/json", getModeConfigJSON());
}

void handleGetData() {
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.sendHeader("Cache-Control", "no-cache");
  
  String validCache = cachedData;
  if (validCache == "" || validCache == "[]" || validCache.length() < 3) {
    validCache = "[]";
  }
  
  String json = "{\"status_alat\":\"standby\",\"last_taps\":[";
  
  int tapCount = 0;
  for (int i = 0; i < 2; i++) {
    if (lastTapHistory[i] != "") {
      if (tapCount > 0) json += ",";
      json += "{\"nama\":\"" + lastTapHistory[i] + "\",\"waktu\":\"" + lastTapTime[i] + "\"}";
      tapCount++;
    }
  }
  json += "],";
  json += "\"log_absensi\":" + validCache;
  json += "}";
  
  server.send(200, "application/json", json);
}

void handlePostTime() {
  server.send(200, "text/plain", "OK");
}

void handleClearData() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String url = String(googleScriptURL) + "?action=clearData";
    http.begin(url);
    http.GET();
    http.end();
  }
  
  server.send(200, "text/plain", "OK");
  delay(1000);
  ESP.restart(); 
}

void handleNotFound() {
  server.send(404, "text/plain", "Not Found");
}

// SETUP
void setup() {
  Serial.begin(115200);
  delay(1000);

  hspi.begin(RFID_SCK_PIN, RFID_MISO_PIN, RFID_MOSI_PIN, RFID_SS_PIN);
  rfid.PCD_Init();
  delay(100);

  Serial.print("Connecting to WiFi...");
  WiFi.begin(ssid, password);
  int timeout = 0;
  while (WiFi.status() != WL_CONNECTED && timeout < 30) {
    delay(500);
    timeout++;
  }
  Serial.println();
  
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("WiFi Connected! IP: " + WiFi.localIP().toString());
    
    timeClient.begin();
    for (int i = 0; i < 10; i++) {
      timeClient.update();
      if (timeClient.isTimeSet()) break;
      delay(500);
    }
    
    updateCacheData();
    
    Serial.println("Mode: " + JUDUL_TABEL);
    Serial.println("Bulan: " + String(START_MONTH+1) + " - " + String(END_MONTH+1));
    Serial.println("IP: " + WiFi.localIP().toString());
  } else {
    Serial.println("WiFi GAGAL! Data TIDAK BISA tersimpan!");
  }

  server.on("/", handleRoot);
  server.on("/getconfig", handleGetConfig);
  server.on("/getdata", handleGetData);
  server.on("/posttime", handlePostTime);
  server.on("/clear", handleClearData);
  server.onNotFound(handleNotFound);
  server.begin();
  
  Serial.println("Web server ready!");
}

// LOOP
void loop() {
  server.handleClient();

  if (WiFi.status() == WL_CONNECTED && millis() % 3600000 < 1000) {
    timeClient.update();
  }

  if (WiFi.status() == WL_CONNECTED && millis() - lastCacheUpdate > CACHE_INTERVAL) {
    updateCacheData();
  }

  if (cardBeingProcessed) return;
  if (millis() - lastReadTime < READ_INTERVAL) return;
  lastReadTime = millis();

  if (!rfid.PICC_IsNewCardPresent()) return;
  if (!rfid.PICC_ReadCardSerial()) return;

  cardBeingProcessed = true;

  String uid = formatUID(rfid.uid.uidByte, rfid.uid.size);
  String nama = CariNamaSiswa(uid);
  
  if (nama == "TIDAK DIKENAL") {
    Serial.println("TAP UNKNOWN: " + uid);
    rfid.PICC_HaltA();
    rfid.PCD_StopCrypto1();
    cardBeingProcessed = false;
    return;
  }

  String jam = getCurrentTime();
  String tgl = getCurrentDate();
  String week = getCurrentWeek();
  
  updateLastTaps(nama, jam + " | " + tgl + " | Week " + week);

  Serial.println(nama + " (" + uid + ") | " + jam + " | " + tgl);
  
  if (WiFi.status() == WL_CONNECTED) {
    kirimKeGoogleSheets(nama, uid, tgl, jam);
  }

  rfid.PICC_HaltA();
  rfid.PCD_StopCrypto1();
  cardBeingProcessed = false;
}