// BUZZER1
#define BUZZER_PIN    26
#define BUZZER_FREQ   1000

unsigned long waktuLalu = 0;
const unsigned long JEDA_BEEP = 2000; // Bunyi selama 2000ms

void buzzerInit() {
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);
}

void buzzerBeep(int durasi) {
  tone(BUZZER_PIN, BUZZER_FREQ, durasi);
}

void setup() {
  Serial.begin(115200);
  delay(1000);
 
  buzzerInit();
  Serial.println("Tes Buzzer");
}

void loop() {
  if (millis() - waktuLalu >= JEDA_BEEP) {
    waktuLalu = millis();
 
    Serial.println("Buzzer Bunyi: Beep!");
    buzzerBeep(150); // Bunyi selama 150 milidetik
  }
}
