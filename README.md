# Scavenger

[한국어](README.ko.md)

A tsugai — a pair — living on a 2-inch screen.

The display is black. A small green light drifts across it, breathing, the way a
paper lantern does on water. That is all it ever does, until you touch it.

Then 愛 comes up out of the dark: a deep-sea angler with that lamp over its
head and a mouth that takes anything. It swallows the thing floating there. The
lump travels down its body. It shudders, spits the thing back out — and 誠, the
small pale one, is already waiting to collect it.

`HERE. TAKE IT.`

Then the fish sinks away, the lamp going out last, and the screen is black
again.

掃除屋 — the ones who tidy things away.

<p align="center">
  <img src="docs/media/scavenger.gif" width="600" alt="愛 fills the screen mid-act: the jaw open, teeth lit, 誠 waiting at the edge.">
</p>

<p align="center"><em>Running on real hardware &mdash; the default config, straight out of <code>git clone</code>.</em></p>

## What you need

| | |
|---|---|
| Board | Waveshare ESP32-S3-Touch-LCD-2 (2.0" ST7789 IPS, 320×240, CST816 touch) |
| Cable | USB-C **data** cable — charge-only cables look identical and won't work |
| Host | macOS or Linux, Python 3 for the color helper |

No microSD card, no sensors, no network. The whole thing is drawn.

A board without a working touch panel is fine too: set `TOUCH_ENABLED false`
and it surfaces on its own every few seconds.

## Run it

```bash
./tools/setup.sh     # arduino-cli, the ESP32 core, the graphics library
./tools/build.sh     # compile — no board needed
./tools/flash.sh     # plug the board in and send it
```

`setup.sh` is idempotent; run it as often as you like. If the board isn't
found, `./tools/doctor.sh` prints one line per requirement and tells you which
one is missing.

## Change it

Everything a person should ever want to change is in **`scavenger/config.h`**,
with a comment above each line. Nothing else needs editing.

| Setting | Default | What it does |
|---|---|---|
| `SPIT_LABEL` | `"BFG-9000"` | what the scavenger gives back, named on the last frame (≤ ~24 chars) |
| `FRAME_MS` | `10` | pause between frames. Lower is faster |
| `TOUCH_ENABLED` | `true` | tap to summon. `false` → it plays by itself |
| `AUTO_PLAY_MS` | `6000` | how long the lamp drifts before surfacing, when there's no touch |
| `LAMP_DRIFT_X` / `_Y` | `40` / `22` | how far the light wanders, in canvas pixels (the canvas is 160×120) |
| `LAMP_GLOW` | `9` | brightness of the lamp, 1–10 |
| `LAMP_BREATH` | `3` | how much that brightness rises and falls |
| `SCREEN_BRIGHTNESS` | `85` | backlight, 0–100 |
| `SCREEN_ROTATION` | `1` | `1` landscape, `3` flipped |

Section 4 of that file is the board's pin map. Leave it alone unless you
changed boards.

## Under the hood

Everything is drawn into a 160×120 canvas and pushed to the panel at 2×, so the
pixels stay chunky on purpose — no anti-aliasing, no smooth curves, and the
palette is 20 hand-picked RGB565 values at the top of `scavenger.h`.

| File | What it holds |
|---|---|
| `scavenger/scavenger.ino` | boot, the waiting lamp, and the one call that starts the act |
| `scavenger/scavenger.h` | the palette, 愛, 誠, the lamp, and `scavPlay()` |
| `scavenger/config.h` | every number you'd want to change |
| `scavenger/lowres.h` | the canvas, the 2× blit, frame timing, small helpers |
| `scavenger/touch.h` | the CST816 panel: did someone tap, and where |

`scavPlay()` is nine passes, in order: the light alone · the fish resolving out
of the dark · the jaw dropping · gone (one white frame) · the lump going down ·
誠 arriving · the handover · sinking away · and a last look at what came back.
It is one blocking call that owns the screen until the act is over.

To change how a creature is drawn, edit `scavFish()` or `scavMakoto()`. To add
or reorder a beat, edit `scavPlay()`. For a new color, don't guess at hex:

```bash
python3 tools/rgb565.py "#7fae5e"
```

RGB565 is not RGB888, and a wrong constant shows up as a color you didn't ask
for rather than an error.

After changing anything outside `config.h`:

```bash
./tools/verify.sh    # compiles every configuration that behaves differently
```

## If something looks wrong

| Symptom | Try |
|---|---|
| Nothing on screen | `./tools/monitor.sh`, then tap RESET — the boot log says whether the LCD came up |
| Upload fails or no port | Hold **BOOT**, tap **RESET**, release **BOOT**, re-run `flash.sh` |
| Tapping does nothing | The boot log says if the CST816 answered. If not, set `TOUCH_ENABLED false` |
| Text cut off | 6px font on a 160px canvas — about 26 characters, then it clips silently |
| Colors look off | Run `tools/rgb565.py` on the hex you meant, compare against `scavenger.h` |

`./tools/doctor.sh` covers most of this in one shot, and its output is meant to
be pasted to a coding agent.

## Working on this with an agent

[AGENTS.md](AGENTS.md) is the canonical guide — the file map, the one rule
(config.h and nowhere else), and the definition of done. `CLAUDE.md` and
`GEMINI.md` point at it.

## License

MIT. See [LICENSE](LICENSE).
