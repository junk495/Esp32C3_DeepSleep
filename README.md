# Esp32-C3 DeepSleep Library

A simple and robust Arduino library for using the deep sleep features of the ESP32-C3, abstracting away the complexities of the ESP-IDF.

## Deep Sleep and Pin Wake-up Challenges on ESP32-C3

### Challenges with Arduino Platform

- **Incomplete ESP-IDF Integration:** Arduino-ESP32 core lags behind ESP-IDF, with poor support for ESP32-C3-specific APIs like ```gpio_wakeup_enable()```.
- **Simplified Abstraction:** Lacks fine-grained control over RTC-GPIOs, pull-ups, and power domains, leading to unreliable wake-ups and higher power consumption.
- **Pin Wake-up Instability:** Floating pins or noisy inputs (e.g., from switches) cause false or missed wake-ups.
- **Version Dependency:** Requires specific Arduino-ESP32 versions for ESP32-C3 support, adding complexity.
- **Debugging Challenges:** Limited error reporting hinders troubleshooting.

### Reliability of ESP-IDF

- **Native Support:** Full access to ESP32-C3 hardware with APIs tailored for deep sleep and GPIO wake-up.
- **Fine-Grained Configuration:** Precise control over GPIOs and power domains ensures stability and low power (<10 µA).
- **Robust Debouncing:** Advanced interrupt handling minimizes false wake-ups.
- **Up-to-Date Features:** Regular updates ensure compatibility and feature support.
- **Comprehensive Documentation:** Detailed guides aid implementation and troubleshooting.

### Conclusion

A dedicated Arduino library wrapping ESP-IDF’s deep sleep and GPIO wake-up APIs combines Arduino’s simplicity with ESP-IDF’s reliability. It provides easy-to-use functions, ensures compatibility, optimizes power consumption, and enhances debugging, making it an ideal solution for reliable pin wake-up (e.g., GPIO4, LOW trigger) on the ESP32-C3 while retaining the Arduino ecosystem.

This library was specifically developed and tested for ESP32-C3 based boards, such as the **Waveshare ESP32-C3-Zero**.

## Key Features

-   Simple, high-level wrappers for Timer and GPIO wakeup sources.
-   Support for using multiple GPIOs as a single wakeup event.
-   Helper functions to hold the state of RTC-capable GPIOs during deep sleep.
-   **Smart, automatic power management:** The library automatically powers down RTC memory when it's not needed (i.e., when no timer wakeup is configured) to maximize power savings, but keeps it on when a timer is active.

## Installation

### PlatformIO (Recommended)

The recommended method is to place the entire library folder (`Esp32C3_DeepSleep`) inside the `lib/` directory of your PlatformIO project. PlatformIO will automatically detect and use it.

Your project structure should look like this:

- Your_Project
  - lib
    - Esp32C3_DeepSleep
      - Esp32C3_DeepSleep.h
      - Esp32C3_DeepSleep.cpp
      - library.properties
      - keywords.txt
      - examples
        - TimerWakeup
          - TimerWakeup.ino
        - PinWakeup
          - PinWakeup.ino
        - AdvancedScenario
          - AdvancedScenario.ino
        - HoldPin
          - HoldPin.ino

### Arduino IDE

1.  Click on `Sketch` > `Include Library` > `Add .ZIP Library...`.
2.  Select the .zip file containing the library.
3.  The library will now be available in the `Sketch` > `Include Library` menu.

## Quick Start

Here is a simple example demonstrating a combined timer and pin wakeup.

```cpp
// main.cpp

#include <Arduino.h>
#include "Esp32C3_DeepSleep.h"

#define WAKEUP_PIN 2
#define SLEEP_TIME_SECONDS 60

void setup() {
  Serial.begin(115200);
  delay(2000); // Wait for monitor to connect

  // Check what woke us up
  esp_sleep_wakeup_cause_t cause = Esp32C3_DeepSleep::wakeupCause();

  if (cause == ESP_SLEEP_WAKEUP_GPIO) {
    Serial.println("Woken up by pin!");
  } else {
    Serial.println("Woken up by timer or on first boot.");
  }

  // --- Configure wakeup sources for the NEXT sleep cycle ---
  
  // 1. Wake up when Pin 2 is pulled LOW
  Esp32C3_DeepSleep::addWakeupPin(WAKEUP_PIN, false);
  
  // 2. Also wake up after 60 seconds
  Esp32C3_DeepSleep::beginTimerWakeup(SLEEP_TIME_SECONDS * 1000000ULL);

  Serial.println("Entering deep sleep...");
  Serial.flush(); // Ensure all messages are sent

  // goDeepSleep will automatically manage power domains based on our configuration
  Esp32C3_DeepSleep::goDeepSleep();
}

void loop() {
  // This is never reached
}

```

```ini
// platformio.ini

[env:esp32-c3-devkitc-02]
platform = espressif32
board = esp32-c3-devkitc-02
framework = arduino

monitor_speed = 115200

build_flags = 
    -D ARDUINO_USB_MODE=1
    -D ARDUINO_USB_CDC_ON_BOOT=1
    -Os
```

## API Reference

### Wakeup Configuration

```void addWakeupPin(uint8_t gpioPin, bool wakeOnHigh)```

Configures a single GPIO pin as a wakeup source. Multiple calls will add pins to the wakeup mask. All pins must share the same wakeup level (HIGH or LOW).

```void clearWakeupPins()```

Resets all configured GPIO wakeup sources.

```void beginTimerWakeup(uint64_t timeUs)```

Configures the timer to wake the chip up after a specified time in microseconds. Calling this function signals the library to keep RTC memory powered on during sleep.

### GPIO State Management

```void holdGPIO(uint8_t gpioPin)```

Latches the current state of an RTC-capable GPIO pin, preserving its level through deep sleep. You must set the pin's state with digitalWrite() before calling this function.

```void releaseAllHolds()```

Releases all active GPIO holds, allowing the pins to be controlled normally again. This is typically called right after waking up.

### Sleep Control & Information

```esp_sleep_wakeup_cause_t wakeupCause()```

Returns the reason for waking up from sleep (e.g., ESP_SLEEP_WAKEUP_TIMER, ESP_SLEEP_WAKEUP_GPIO).

```void goDeepSleep()```

Immediately enters deep sleep. This function automatically optimizes power domains based on the configured wakeup sources and does not return.

## Examples

For more detailed usage, please see the sketches in the examples folder:
- TimerWakeup: Demonstrates a simple wakeup by timer.
- PinWakeup: Shows how to wake up from a single GPIO pin.
- HoldPin: Illustrates how to keep an LED on during deep sleep.
- AdvancedScenario: A comprehensive example combining all major library features.

## License

This project is licensed under the MIT License.
