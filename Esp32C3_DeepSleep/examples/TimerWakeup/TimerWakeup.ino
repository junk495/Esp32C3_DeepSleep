#include <Esp32C3_DeepSleep.h>

// Das Board wacht alle 10 Sekunden auf
#define SLEEP_TIME_SECONDS 10

void setup() {
  Serial.begin(115200);
  delay(1000); // kurz warten bis der serielle Monitor verbunden ist

  // Finde den Grund für das Aufwachen heraus
  esp_sleep_wakeup_cause_t cause = Esp32C3_DeepSleep::wakeupCause();

  if (cause != ESP_SLEEP_WAKEUP_TIMER) {
    Serial.println("Konfiguriere Timer als Aufweckgrund...");
    // Konfiguriere den Timer-Wakeup (Zeit in Mikrosekunden)
    Esp32C3_DeepSleep::beginTimerWakeup(SLEEP_TIME_SECONDS * 1000000ULL);
    Serial.printf("Gehe für %d Sekunden schlafen.\n", SLEEP_TIME_SECONDS);
  } else {
    Serial.println("Aufgewacht durch Timer!");
    // Hier könnte Code stehen, der nach dem Aufwachen ausgeführt wird
  }

  Serial.println("Setup abgeschlossen. Starte jetzt Deep Sleep.");
  Serial.flush(); // Sicherstellen, dass alle seriellen Daten gesendet wurden

  // Schicke den ESP32-C3 in den Tiefschlaf
  Esp32C3_DeepSleep::goDeepSleep();
}

void loop() {
  // Dieser Code wird nie erreicht, da der ESP32-C3
  // nach dem Deep Sleep immer im setup() neu startet.
}