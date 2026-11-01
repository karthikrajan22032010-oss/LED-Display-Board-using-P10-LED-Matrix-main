#include <DMD32.h>
#include "fonts/SystemFont5x7.h"

// Number of panels
#define DISPLAYS_WIDE 1
#define DISPLAYS_HIGH 1

DMD dmd(DISPLAYS_WIDE, DISPLAYS_HIGH);

// Timer for refresh (ESP32)
hw_timer_t * timer = NULL;

// Interrupt function
void IRAM_ATTR triggerScan() {
  dmd.scanDisplayBySPI();
}

void setup() {
  // Initialize display
  dmd.clearScreen(true);

  // Setup timer (ESP32)
  timer = timerBegin(0, 80, true); // 80 prescaler → 1µs
  timerAttachInterrupt(timer, &triggerScan, true);
  timerAlarmWrite(timer, 2000, true); // refresh
  timerAlarmEnable(timer);
}

void loop() {
  dmd.selectFont(SystemFont5x7);

  dmd.drawMarquee("WELCOME", 7, (32 * DISPLAYS_WIDE), 0);

  long start = millis();
  long timer = start;
  boolean ret = false;

  while (!ret) {
    if ((timer + 30) < millis()) {
      ret = dmd.stepMarquee(-1, 0);
      timer = millis();
    }
  }
}