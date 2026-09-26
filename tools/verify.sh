#!/usr/bin/env bash
#
# verify.sh — compile every configuration that behaves differently.
#
# Run this after changing anything that isn't just a number in config.h —
# scavenger.h, lowres.h, touch.h. It catches the classic "compiles with touch
# on, breaks with touch off" mistake in about a minute. config.h is restored
# exactly as it was, even if a build fails.
#
source "$(dirname "${BASH_SOURCE[0]}")/common.sh"
need_cli
set +e

CFG="$ROOT/scavenger/config.h"
BAK="$(mktemp)"
cp "$CFG" "$BAK"

restore() { cp "$BAK" "$CFG"; rm -f "$BAK"; }
trap restore EXIT INT TERM

# sed -i differs between GNU and BSD; this works on both.
edit() { sed -i.tmp "$1" "$CFG" && rm -f "$CFG.tmp"; }

fails=0
BUILD_DIR="$ROOT/.build/verify"
try() {
  if arduino-cli compile --fqbn "$FQBN" --build-path "$BUILD_DIR" "$SKETCH" >/dev/null 2>&1; then
    ok "$1"
  else
    warn "$1  — FAILED"
    fails=$((fails + 1))
  fi
}

bold "Verifying every configuration"
echo

try "default config.h"

edit "s/^#define TOUCH_ENABLED .*/#define TOUCH_ENABLED         false/"
try "without touch (plays on its own)"
cp "$BAK" "$CFG"

edit "s/^#define SCREEN_ROTATION .*/#define SCREEN_ROTATION          3/"
try "rotation 3 (flipped)"
cp "$BAK" "$CFG"

edit "s/^#define LAMP_GLOW .*/#define LAMP_GLOW                1/"
edit "s/^#define LAMP_BREATH .*/#define LAMP_BREATH              0/"
try "lamp at its dimmest, no breathing"
cp "$BAK" "$CFG"

edit "s/^#define SPIT_LABEL .*/#define SPIT_LABEL              \"\"/"
try "empty SPIT_LABEL"
cp "$BAK" "$CFG"

echo
if [ "$fails" -eq 0 ]; then
  bold "All configurations build."
else
  die "$fails configuration(s) failed to build."
fi
