/**
 * @file AdvancedScenario.ino
 * @author Dein Name
 * @date 16. June 2025
 * @brief Advanced example combining timer, pin wakeup, pin holding, and timekeeping.
 *
 * This sketch demonstrates a complex deep sleep scenario:
 * 1. Wakes up every 30 seconds to display a running "system uptime".
 * 2. Can also be woken up by a button on GPIO 2.
 * 3. An LED on GPIO 1 is kept HIGH during deep sleep, but is LOW when awake.
 *
 * Hardware Setup:
 * - An LED (with resistor) between GPIO 1 and GND. This LED will be ON during sleep.
 * - A push button between GPIO 2 and GND.
 */

#include <Esp32C3_DeepSleep.h>

// --- Konfiguration ---
#define STATUS_PIN 1        // Pin, der im Schlaf HIGH und im Wachzustand LOW ist.
#define WAKEUP_PIN 2        // Pin, der bei LOW-Signal aufweckt.
#define SLEEP_INTERVAL_S 30 // 30 Sekunden

// --- Zeitmessung ---
// Diese Variable wird im RTC-Speicher abgelegt und überlebt den Deep Sleep.
// Sie zählt, wie oft das Gerät durch den Timer aufgewacht ist.
RTC_DATA_ATTR int timer_wakeup_count = 0;

// Hilfsfunktion zur Anzeige der System-Laufzeit
void printSystemTime() {
  // Wir simulieren die Systemzeit durch Zählen der 30-Sekunden-Intervalle.
  uint32_t total_seconds = timer_wakeup_count * SLEEP_INTERVAL_S;
  int hours = total_seconds / 3600;
  int minutes = (total_seconds % 3600) / 60;
  int seconds = total_seconds % 60;
  Serial.printf("System Uptime: %02d:%02d:%02d\n", hours, minutes, seconds);
}

void setup() {
  Serial.begin(115200);
  delay(2000);
  Serial.println("\n--- Advanced Deep Sleep Scenario ---");

  // Pin-Modus setzen
  pinMode(STATUS_PIN, OUTPUT);

  // --- AKTIONEN BEIM AUFWACHEN ---
  // 1. Alle GPIO-Holds lösen, um die Kontrolle wiederzuerlangen.
  Esp32C3_DeepSleep::releaseAllHolds();

  // 2. Den "Wachzustand" für Pin 1 herstellen (LOW). Die LED geht aus.
  digitalWrite(STATUS_PIN, LOW);
  Serial.printf("State: Awake. Pin %d is LOW.\n", STATUS_PIN);

  // 3. Grund für das Aufwachen prüfen und entsprechend reagieren.
  esp_sleep_wakeup_cause_t cause = Esp32C3_DeepSleep::wakeupCause();

  switch (cause) {
    case ESP_SLEEP_WAKEUP_TIMER:
      timer_wakeup_count++; // Zeit nur beim Timer-Wakeup weiterzählen
      Serial.println("Wakeup Cause: Timer.");
      printSystemTime();
      break;
    
    case ESP_SLEEP_WAKEUP_GPIO:
      Serial.println("Wakeup Cause: GPIO Pin.");
      Serial.println("Manual wakeup triggered. Time will not be advanced.");
      printSystemTime(); // Zeige die zuletzt bekannte Zeit an
      break;

    default:
      Serial.println("Wakeup Cause: Initial Boot (Power-On).");
      timer_wakeup_count = 0; // Zeitzählung zurücksetzen
      break;
  }
  
  delay(2000); // Warten, damit die Nachrichten gelesen werden können.

  // --- KONFIGURATION FÜR DEN NÄCHSTEN SCHLAFZYKLUS ---
  Serial.println("\nConfiguring for next sleep cycle...");

  // 1. Timer konfigurieren
  Esp32C3_DeepSleep::beginTimerWakeup(SLEEP_INTERVAL_S * 1000000ULL);
  Serial.printf("- Timer wakeup is set for %d seconds.\n", SLEEP_INTERVAL_S);

  // 2. Pin-Wakeup konfigurieren
  Esp32C3_DeepSleep::addWakeupPin(WAKEUP_PIN, false);
  Serial.printf("- Pin %d is a wakeup source.\n", WAKEUP_PIN);

  // 3. Pin für den "Schlafzustand" konfigurieren (HIGH). Die LED geht an.
  Esp32C3_DeepSleep::holdGPIO(STATUS_PIN, HIGH);
  Serial.printf("- Pin %d will be held HIGH during sleep.\n", STATUS_PIN);
  
  // Letzte Nachricht
  Serial.println("\nEntering deep sleep now...");
  Serial.flush();
  delay(200);

  Esp32C3_DeepSleep::goDeepSleep();
}

void loop() {
  // Wird nie erreicht
}