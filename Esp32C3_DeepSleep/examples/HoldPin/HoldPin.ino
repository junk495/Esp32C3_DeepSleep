/**
 * @file HoldPin.ino
 * @author Dein Name
 * @date 16. June 2025
 * @brief Example for holding a GPIO pin's state during deep sleep.
 *
 * This sketch demonstrates how to use the holdGPIO() function to keep an
 * external LED lit even when the ESP32-C3 is in deep sleep.
 * It uses a timer to wake up after a few seconds.
 *
 * Hardware Setup:
 * - Connect an LED (with a resistor, e.g., 220 Ohm) between GPIO 1 and GND.
 * GPIO 1 is an RTC-capable pin.
 */

#include <Esp32C3_DeepSleep.h>

// Use an RTC-capable pin. GPIO 1 is a valid choice.
#define HOLD_PIN 1
#define PIN_LEVEL HIGH

// Sleep for 10 seconds
#define SLEEP_TIME_SECONDS 10

void setup() {
  // Start serial communication
  Serial.begin(115200);
  delay(2000); // Wait for Serial Monitor to connect
  Serial.println("\n--- ESP32-C3 Hold Pin Example ---");

  // Set the pin as a normal output first
  pinMode(HOLD_PIN, OUTPUT);

  // Check the reason for the wakeup
  esp_sleep_wakeup_cause_t cause = Esp32C3_DeepSleep::wakeupCause();

  if (cause == ESP_SLEEP_WAKEUP_TIMER) {
      Serial.println("Woke up from timer.");
      Serial.println("The LED should have been ON during sleep.");
      delay(1000);

      // IMPORTANT: Release the hold to regain normal control of the pin
      Esp32C3_DeepSleep::releaseAllHolds();
      Serial.println("GPIO holds have been released.");
      delay(1000);

      // Demonstrate that the hold is released by turning the LED off
      Serial.println("Turning the LED OFF now to show control is back.");
      digitalWrite(HOLD_PIN, !PIN_LEVEL);
      delay(3000); // Keep it off for 3 seconds to see the effect

  } else {
      Serial.println("Initial boot. Configuring GPIO hold and timer wakeup.");
      delay(1000);

      // Turn the LED on before sleeping
      Serial.printf("Turning LED ON (Pin %d)\n", HOLD_PIN);
      digitalWrite(HOLD_PIN, PIN_LEVEL);
      delay(1000);

      // Tell the ESP32-C3 to hold this pin's state during deep sleep
      Esp32C3_DeepSleep::holdGPIO(HOLD_PIN, PIN_LEVEL);
      Serial.printf("Pin %d will be held %s during deep sleep.\n", HOLD_PIN, PIN_LEVEL ? "HIGH" : "LOW");

      // Configure the timer to wake us up
      Esp32C3_DeepSleep::beginTimerWakeup(SLEEP_TIME_SECONDS * 1000000ULL);
      Serial.printf("Configured timer wakeup for %d seconds.\n", SLEEP_TIME_SECONDS);
  }

  // Final message before sleeping
  Serial.printf("\nEntering deep sleep for %d seconds...\n", SLEEP_TIME_SECONDS);
  Serial.println("--> OBSERVE THE LED: It should stay ON during sleep. <--");
  delay(500);

  // Ensure all serial messages are sent
  Serial.flush();

  // Enter deep sleep
  Esp32C3_DeepSleep::goDeepSleep();
}

void loop() {
  // This part is never reached.
}