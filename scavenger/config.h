/*
 * ==============================================================================
 *  config.h  —  THE ONLY FILE YOU NEED TO EDIT
 * ==============================================================================
 *  SCAVENGER, waiting in the dark.
 *
 *  The screen shows nothing but the angler's lamp, drifting the way a paper
 *  lantern does. Touch it and 愛 comes up out of the black, swallows the thing
 *  floating there, and 誠 hands it back. Then the lamp goes back to drifting.
 *
 *  Build & flash:   ./tools/flash.sh scavenger
 * ==============================================================================
 */

#pragma once

// ==============================================================================
//  1. THE ANIMATION
// ==============================================================================

// What the scavenger gives back. Keep it under ~24 characters.
#define SPIT_LABEL              "BFG-9000"

// Pause between animation frames, in milliseconds. Lower is faster.
#define FRAME_MS                10

// Touch to summon. Without a panel the sketch plays on its own instead.
#define TOUCH_ENABLED         true

// How long the lamp drifts before it summons itself anyway, in milliseconds.
// Only used when there is no touch panel.
#define AUTO_PLAY_MS          6000


// ==============================================================================
//  2. THE LAMP
// ==============================================================================
//  How far the light wanders from the middle of the screen, in canvas pixels
//  (the screen is 160x120 of them). Bigger numbers, longer drift.
#define LAMP_DRIFT_X            40
#define LAMP_DRIFT_Y            22

// Brightness of the lamp, 1-10, and how much it breathes.
#define LAMP_GLOW                9
#define LAMP_BREATH              3


// ==============================================================================
//  3. SCREEN
// ==============================================================================
#define SCREEN_BRIGHTNESS       85    // 0-100
#define SCREEN_ROTATION          1    // 1 = landscape 320x240, 3 = flipped

#define SCREEN_W               320
#define SCREEN_H               240


// ==============================================================================
//  4. HARDWARE — don't touch unless you changed boards
// ==============================================================================
#define LCD_PIN_SCLK   39
#define LCD_PIN_MOSI   38
#define LCD_PIN_MISO   40
#define LCD_PIN_DC     42
#define LCD_PIN_RST    -1
#define LCD_PIN_CS     45
#define LCD_PIN_BL      1

#define TOUCH_PIN_SDA  48
#define TOUCH_PIN_SCL  47

#define LCD_PANEL_W   240   // native panel width  (before rotation)
#define LCD_PANEL_H   320   // native panel height (before rotation)
