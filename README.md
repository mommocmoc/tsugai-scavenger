# Scavenger

[한국어](README.ko.md)

A *tsugai* (つがい, pair) living inside a 2-inch screen.

The screen is pitch black. A single soft green light drifts across the dark, breathing gently like a paper lantern on water. If left untouched, that is all it will ever do.

Touch the screen, and 愛 (Ai) rises from the depths: a deep-sea anglerfish with a drifting lure over its head and a gaping jaw that consumes anything in its path. It swallows the floating object whole. The lump travels visibly down its body. It shudders, spits the item back out — and 誠 (Makoto), the small pale companion, is already in place waiting to receive it.

`HERE. TAKE IT.`

The fish then slowly sinks back into the abyss. The lamp lingers until the very last moment before extinguishing, returning the display to pure darkness.

掃除屋 (*sōjiya*) — the ones who tidy things away.

<p align="center">
  <img src="docs/media/scavenger.gif" width="600" alt="愛 fills the screen mid-act: the jaw open, teeth lit, 誠 waiting at the edge.">
</p>

<p align="center"><em>Running on physical hardware &mdash; default configuration right after <code>git clone</code>.</em></p>

## Requirements

| Item | Details |
|---|---|
| **Board** | Waveshare ESP32-S3-Touch-LCD-2 (2.0" ST7789 IPS, 320×240, CST816 touch) |
| **Cable** | USB-C **data** cable (charge-only cables look identical but will not transfer data) |
| **Host** | macOS or Linux, with Python 3 for the color conversion tool |

No microSD card, external sensors, or network connection required. Every visual element is rendered procedurally in code.

Boards without a functional touch panel are fully supported: simply set `TOUCH_ENABLED false`, and the animation will trigger automatically every few seconds.

## Getting Started

```bash
./tools/setup.sh     # Install arduino-cli, ESP32 core, and graphics libraries
./tools/build.sh     # Compile the firmware (no board connection required)
./tools/flash.sh     # Connect the board and upload firmware
```

`setup.sh` is idempotent and safe to run multiple times (it automatically skips already installed components). If your board is not detected, run `./tools/doctor.sh` to diagnose missing dependencies or port issues line by line.

## Customization

All user-tunable settings reside in a single file: **[`scavenger/config.h`](scavenger/config.h)**. Each option is documented with clear comments, so you never need to edit any other source files.

| Setting | Default | Description |
|---|---|---|
| `SPIT_LABEL` | `"BFG-9000"` | Label of the returned item shown on the final frame (≤ ~24 characters) |
| `FRAME_MS` | `10` | Delay between animation frames in ms (lower values mean faster speed) |
| `TOUCH_ENABLED` | `true` | Summon on touch. If `false`, plays automatically on a timer |
| `AUTO_PLAY_MS` | `6000` | Idle drift duration before surfacing when touch is disabled (ms) |
| `LAMP_DRIFT_X` / `_Y` | `40` / `22` | Maximum drift offset from center in canvas pixels (canvas is 160×120) |
| `LAMP_GLOW` | `9` | Base lamp brightness radius (1–10) |
| `LAMP_BREATH` | `3` | Amplitude of the lamp breathing pulsation |
| `SCREEN_BRIGHTNESS` | `85` | Display backlight intensity (0–100) |
| `SCREEN_ROTATION` | `1` | Orientation (`1`: default landscape, `3`: 180° flipped) |

Section 4 of `config.h` defines the hardware pin assignments. Keep these unchanged unless you are porting to different hardware.

## Architecture

Visuals are rendered onto a 160×120 internal framebuffer and scaled up 2× to fit the 320×240 panel. The chunky pixel look is intentional — there is no anti-aliasing or artificial smoothing. The palette consists solely of 20 hand-picked RGB565 color constants defined at the top of `scavenger.h`.

| Source File | Responsibility |
|---|---|
| [`scavenger/scavenger.ino`](scavenger/scavenger.ino) | Bootstrapping, idle lamp drift loop, and act invocation |
| [`scavenger/scavenger.h`](scavenger/scavenger.h) | Color palette, rendering routines for 愛, 誠, lamp, and `scavPlay()` sequence |
| [`scavenger/config.h`](scavenger/config.h) | Central configuration parameters |
| [`scavenger/lowres.h`](scavenger/lowres.h) | Canvas buffer management, 2× blit scaling, frame timing, helper utilities |
| [`scavenger/touch.h`](scavenger/touch.h) | CST816 touch controller driver and gesture detection |

`scavPlay()` orchestrates 9 sequential beats: solitary lamp · the angler resolving from the dark · jaw dropping wide · consumption flash (single white frame) · lump traveling down · arrival of 誠 · handover · sinking into the deep · final glimpse of the returned item. It is a single blocking function that retains full control of the display until the performance concludes.

To alter the visual design of the creatures, edit `scavFish()` or `scavMakoto()`. To add or reorder story beats, adjust `scavPlay()`. When adding new colors, avoid guessing raw hex values — use the conversion helper:

```bash
python3 tools/rgb565.py "#7fae5e"
```

RGB565 is fundamentally different from standard 24-bit RGB888. Incorrect values will compile without warnings but produce unexpected colors on screen.

Whenever you modify files outside `config.h`, verify compatibility across configurations:

```bash
./tools/verify.sh    # Compiles and validates all supported feature permutations
```

## Troubleshooting

| Symptom | Recommended Action |
|---|---|
| **Blank / Black Screen** | Run `./tools/monitor.sh` and press the **RESET** button. Verify in serial logs that the LCD initializes successfully. |
| **Upload Failure / Port Missing** | Hold the **BOOT** button, press and release **RESET**, release **BOOT**, then run `./tools/flash.sh` again. |
| **No Touch Response** | Check the serial boot log to see if the CST816 IC was recognized. If using an unsupported screen, set `TOUCH_ENABLED false`. |
| **Text Truncated** | With a 6px font on a 160px canvas, text strings exceeding ~26 characters are clipped silently. |
| **Incorrect Colors** | Convert your target hex color via `python3 tools/rgb565.py` and compare against definitions in `scavenger.h`. |

Running `./tools/doctor.sh` diagnoses almost all common hardware and toolchain issues, producing output formatted for easy sharing with coding assistants.

## Working with AI Agents

See [AGENTS.md](AGENTS.md) for canonical agent instructions — codebase layout, the rule of confining user changes to `config.h`, and completion criteria. `CLAUDE.md` and `GEMINI.md` point directly to it.

## License

MIT License. See [LICENSE](LICENSE) for details.
