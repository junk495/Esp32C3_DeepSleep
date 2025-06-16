/**
 * @file PinWakeup.ino
 * @author junk495
 * @date 16. June 2025
 * @brief Example for waking up the ESP32-C3 from deep sleep using a GPIO pin.
 *
 * This sketch demonstrates how to use the Esp32C3_DeepSleep library
 * to configure a pin wakeup. The ESP32-C3 will go into deep sleep
 * and can be woken up by pulling GPIO 2 to LOW (e.g., by connecting it to GND).
 */

#include <Esp32C3_DeepSleep.h>

// Define which GPIO pin will be used for wakeup
#define WAKEUP_PIN 2

void setup() {
  // Start serial communication
  Serial.begin(115200);
  // Wait 2 seconds to allow the Serial Monitor to connect.
  delay(2000);
  Serial.println("\n--- ESP32-C3 Pin Wakeup Example ---");
  delay(500);

  // Check the reason for the wakeup
  esp_sleep_wakeup_cause_t cause = Esp32C3_DeepSleep::wakeupCause();

  // Check if the wakeup was caused by a GPIO pin
  if (cause == ESP_SLEEP_WAKEUP_GPIO) {
    Serial.println("Wakeup successful!");
    Serial.println("Triggered by a GPIO pin.");
    delay(2000); // Wait so the message can be read
    Serial.println("Going back to sleep now.");

  } else {
    // If it's the first boot or another wakeup reason
    Serial.printf("Initial boot or other wakeup reason (Cause: %d).\n", cause);
    Serial.println("Configuring Pin-Wakeup now...");
    delay(1000); // Wait a moment for readability

    // Configure WAKEUP_PIN to wake the device up when the signal is LOW.
    // The library automatically enables an internal pull-up resistor for this mode.
    Esp32C3_DeepSleep::addWakeupPin(WAKEUP_PIN, false); // 'false' means: wake on LOW

    Serial.printf("Configuration done. GPIO %d is now a wakeup source.\n", WAKEUP_PIN);
    delay(1000);
    Serial.println("--> TO TEST: Briefly connect Pin 2 to GND. <--");
  }

  // Final message before sleeping
  Serial.println("\nEntering deep sleep now...");
  delay(500);

  // Ensure all serial messages are sent before sleeping
  Serial.flush();

  // Enter deep sleep. The program does not return from here.
  Esp32C3_DeepSleep::goDeepSleep();
}

void loop() {
  // This part is never reached. The ESP32-C3 restarts in setup() after waking up.
}
