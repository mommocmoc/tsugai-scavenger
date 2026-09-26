/*
 * ==============================================================================
 *  SCAVENGER  —  a lamp in the dark, and what is holding it
 * ==============================================================================
 *  Board    : Waveshare ESP32-S3-Touch-LCD-2  (2.0" ST7789 IPS, 320x240)
 *  Shows    : nothing but the angler's lamp drifting across black water. Touch
 *             the screen and 愛 rises, swallows the thing, and 誠 gives it back.
 *
 *  YOU PROBABLY DON'T NEED TO READ THIS FILE.  Everything tunable is in
 *  config.h; the creatures are in scavenger.h.
 *
 *  Build & flash:   ./tools/flash.sh scavenger
 * ==============================================================================
 */

#include <Arduino_GFX_Library.h>

#include "config.h"

// ==============================================================================
//  Display
// ==============================================================================
Arduino_DataBus *bus = new Arduino_ESP32SPI(
    LCD_PIN_DC, LCD_PIN_CS, LCD_PIN_SCLK, LCD_PIN_MOSI, LCD_PIN_MISO);

Arduino_GFX *gfx = new Arduino_ST7789(
    bus, LCD_PIN_RST, SCREEN_ROTATION, true /* IPS */, LCD_PANEL_W, LCD_PANEL_H);

#include "lowres.h"
#include "scavenger.h"

#if TOUCH_ENABLED
  #include "touch.h"
#else
  static const bool touch_ready = false;
#endif

// ==============================================================================
//  Waiting
// ==============================================================================
//  Only the lamp. It wanders on two slow sines that never quite line up, so the
//  path keeps changing, and it breathes while it goes.
static void drawLamp(unsigned long t) {
  if (!loBegin()) return;
  lo->fillScreen(S_VOID);

  int x = LO_W / 2 + (int)(LAMP_DRIFT_X * sinf(t / 2300.0f));
  int y = LO_H / 2 + (int)(LAMP_DRIFT_Y * sinf(t / 1470.0f));
  int glow = LAMP_GLOW + (int)(LAMP_BREATH * sinf(t / 700.0f));
  if (glow < 1) glow = 1;

  scavLamp(x, y, glow);
  loBlit(0, 0, LO_W, LO_H);
}

void setup() {
  Serial.begin(115200);
  delay(200);
  Serial.println("\n=== Scavenger ===");

  ledcAttach(LCD_PIN_BL, 5000 /* Hz */, 10 /* bits */);
  ledcWrite(LCD_PIN_BL, (1 << 10) * SCREEN_BRIGHTNESS / 100);

  if (!gfx->begin()) {
    Serial.println("[LCD] init FAILED — check wiring / board selection");
  } else {
    Serial.printf("[LCD] ready (%dx%d)\n", gfx->width(), gfx->height());
  }
  gfx->fillScreen(0x0000);

#if TOUCH_ENABLED
  if (touchBegin()) Serial.println("[TOUCH] CST816 ready — touch the lamp");
  else              Serial.println("[TOUCH] none — it will surface on its own");
#endif

  drawLamp(millis());
}

void loop() {
  static unsigned long waiting_since = millis();

#if TOUCH_ENABLED
  // Touch anywhere: the lamp was attached to something after all.
  int tx, ty;
  if (touchTapped(&tx, &ty)) {
    scavPlay();
    waiting_since = millis();
    return;
  }
#endif

  // No panel to touch? Then it surfaces by itself every so often.
  if (!touch_ready && millis() - waiting_since >= AUTO_PLAY_MS) {
    scavPlay();
    waiting_since = millis();
    return;
  }

  drawLamp(millis());
  delay(40);
}
