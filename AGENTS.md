# AGENTS.md

Instructions for a coding agent (Claude Code, Codex, Antigravity, Cursor, …)
operating this repository. Humans want `README.md` instead.

## What this project is

Firmware for one animation on a 320×240 ST7789 LCD driven by an ESP32-S3.

The screen shows nothing but an angler fish's lamp, drifting. Touch it and 愛
comes up out of the black, swallows the thing floating there, and 誠 hands it
back. Then the lamp goes back to drifting.

One sketch, `scavenger/`. No build system beyond `arduino-cli`.

## The one rule

**User-facing changes go in `scavenger/config.h`. Nothing else.**

When someone asks for a slower animation, a dimmer lamp, a different returned
object, a flipped screen — edit `config.h` and stop. `scavenger.ino` is the
loop that waits, and `scavenger.h` is the creature; changing either to
accomplish a config-level request is the most common way to break this project.

Escalate beyond `config.h` only when the request genuinely needs it:

| Request | File to edit |
|---|---|
| Speed, pacing, auto-play interval | `scavenger/config.h` |
| Lamp drift, glow, breathing | `scavenger/config.h` |
| What comes back out (the label) | `scavenger/config.h` |
| Brightness, rotation | `scavenger/config.h` |
| Touch on/off | `scavenger/config.h` |
| How 愛 or 誠 is **drawn** | `scavenger/scavenger.h` |
| A new beat in the act, reordering scenes | `scavenger/scavenger.h` — `scavPlay()` |
| What the lamp does *while waiting* | `scavenger/scavenger.ino` — `drawLamp()` |
| Canvas size, blit, frame timing | `scavenger/lowres.h` |
| Touch panel behaviour | `scavenger/touch.h` |

## Commands

```bash
./tools/setup.sh         # install arduino-cli, ESP32 core, libraries (idempotent)
./tools/doctor.sh        # diagnose the environment; read this before guessing
./tools/build.sh         # compile only — no board required
./tools/flash.sh         # compile + upload (auto-detects the port)
./tools/monitor.sh       # serial log at 115200
./tools/verify.sh        # compile every configuration that behaves differently
```

Board FQBN, if you need to invoke `arduino-cli` directly:

```
esp32:esp32:esp32s3:CDCOnBoot=cdc,FlashSize=16M,PSRAM=opi,PartitionScheme=huge_app
```

## How the sketch is put together

Everything is drawn into a 160×120 canvas (`lowres.h`) and pushed to the LCD at
2×, so the pixels stay coarse on purpose. The fish faces left.

| File | What it holds |
|---|---|
| `scavenger.ino` | boot, the waiting lamp, and the one line that starts the act |
| `scavenger.h` | the palette, 愛, 誠, the lamp, and `scavPlay()` |
| `config.h` | every number a user should ever touch |
| `lowres.h` | the canvas, the 2× blit, `loFrame()`, `loLerp()`, `loMsg()` |
| `touch.h` | the CST816 panel: "did someone tap, and where" |

`scavPlay()` is one blocking call that owns the screen from the first frame to
the last — roughly 70 frames over nine passes. It does not return until the act
is over, so `loop()` does nothing else while it runs. That is deliberate: there
is no state machine to keep in sync.

## Working rules

1. **Always `./tools/build.sh` after editing.** A compile takes under a minute
   and is the only proof the change is valid. Never report success without it.
2. **Never flash unless the user asked you to.** `build.sh` is the safe default;
   `flash.sh` writes to physical hardware.
3. **Run `./tools/verify.sh` after touching `scavenger.h`, `lowres.h`, or
   `touch.h`.** A change can compile with touch enabled and fail without it.
4. **Don't invent colors as raw hex.** Run `python3 tools/rgb565.py "#7fae5e"`
   and use what it prints. RGB565 is not RGB888.
5. **Check text length.** `loMsg()` and `SPIT_LABEL` use a fixed 6px font on a
   160px canvas — about 26 characters before it runs off the edge. It clips
   silently; no error, just a cut-off word.
6. **Coordinates are canvas pixels, not screen pixels.** The canvas is 160×120.
   A drift of 40 is a quarter of the screen, not 40 screen pixels.
7. **Preserve the comments in `config.h`.** They are the product; a user with no
   C++ background reads them to understand what they're changing.

## Gotchas

- `PartitionScheme=huge_app` is kept from the board's usual profile. The default
  scheme is tight once the graphics library and a full canvas are in.
- No touch panel is a supported state, not an error. With `TOUCH_ENABLED false`
  the act plays itself every `AUTO_PLAY_MS`.
- `scavDim()` reaches into the canvas framebuffer directly to knock out random
  pixels. If you change `LO_W`/`LO_H`, that loop follows automatically — but
  anything that assumes a 160-wide row will not.
- The board sometimes needs a manual bootloader entry: hold **BOOT**, tap
  **RESET**, release **BOOT**, then re-run `flash.sh`.
- **Apple Silicon (macOS) requires Rosetta 2**: Arduino CLI depends on an x86_64
  `ctags` binary on macOS. If build fails with `ctags: bad CPU type in executable`,
  install Rosetta 2: `softwareupdate --install-rosetta --agree-to-license`.

## Definition of done

- [ ] `./tools/build.sh` passes
- [ ] `./tools/verify.sh` passes, if anything outside `config.h` changed
- [ ] Text fits the ~26 character budget
- [ ] Told the user what to run next (`./tools/flash.sh`), rather than flashing
