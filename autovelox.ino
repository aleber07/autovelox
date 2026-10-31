#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 6);  // RS, E, D4, D5, D6, D7

const int TRIG_PIN = 9;
const int ECHO_PIN = 10;
const int FLASH1_PIN = 8;
const int FLASH2_PIN = A0;
const int VERDE_PIN = A1;
const int ROSSO_PIN = A2;
const int BUZZER_PIN = 7;
const int BUTTON_PIN = 2;

const int SOGLIA_CM = 100;
const unsigned long COOLDOWN_MS = 1500;

unsigned long ultimoFlash = 0;
int fotoScattate = 0;
bool attivo = true;
bool ultimoStato = HIGH;

void setup() {
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(FLASH1_PIN, OUTPUT);
  pinMode(FLASH2_PIN, OUTPUT);
  pinMode(VERDE_PIN, OUTPUT);
  pinMode(ROSSO_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  Serial.begin(9600);
  lcd.begin(16, 2);
  lcd.print("Autovelox pronto");
  aggiornaLedStato();
  delay(1000);
  lcd.clear();
  Serial.println("Autovelox pronto");
}

void aggiornaLedStato() {
  digitalWrite(VERDE_PIN, attivo ? HIGH : LOW);
  digitalWrite(ROSSO_PIN, attivo ? LOW : HIGH);
}

float misuraDistanza() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  long durata = pulseIn(ECHO_PIN, HIGH, 30000);
  if (durata == 0) return -1;
  return durata * 0.0343 / 2.0;
}

void flash() {
  digitalWrite(FLASH1_PIN, HIGH);
  digitalWrite(FLASH2_PIN, HIGH);
  tone(BUZZER_PIN, 2000, 80);
  delay(150);
  digitalWrite(FLASH1_PIN, LOW);
  digitalWrite(FLASH2_PIN, LOW);
  delay(50);
  digitalWrite(FLASH1_PIN, HIGH);
  digitalWrite(FLASH2_PIN, HIGH);
  delay(80);
  digitalWrite(FLASH1_PIN, LOW);
  digitalWrite(FLASH2_PIN, LOW);
}

void aggiornaDisplay(float distanza) {
  lcd.setCursor(0, 0);
  if (!attivo) {
    lcd.print("Sistema spento  ");
  } else {
    lcd.print("Foto scattate:");
    lcd.print(fotoScattate);
    lcd.print("   ");
  }
  lcd.setCursor(0, 1);
  if (distanza < 0) {
    lcd.print("Distanza: --    ");
  } else {
    lcd.print("Distanza: ");
    lcd.print((int)distanza);
    lcd.print(" cm   ");
  }
}

void loop() {
  bool stato = digitalRead(BUTTON_PIN);
  if (stato == LOW && ultimoStato == HIGH) {
    attivo = !attivo;
    aggiornaLedStato();
    Serial.println(attivo ? "Attivo" : "Spento");
    delay(200);
  }
  ultimoStato = stato;

  if (!attivo) {
    aggiornaDisplay(-1);
    delay(100);
    return;
  }

  float distanza = misuraDistanza();
  aggiornaDisplay(distanza);

  if (distanza > 0) {
    Serial.print("Distanza: ");
    Serial.print(distanza);
    Serial.println(" cm");

    if (distanza < SOGLIA_CM && millis() - ultimoFlash > COOLDOWN_MS) {
      flash();
      fotoScattate++;
      ultimoFlash = millis();
      lcd.setCursor(0, 1);
      lcd.print("FLASH! Foto ok  ");
      delay(500);
    }
  }
  delay(100);
}