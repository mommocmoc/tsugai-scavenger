#!/usr/bin/env bash
#
# monitor.sh — watch the serial output. Ctrl-C to quit.
#
# The sketch only prints while it boots: display init, and whether it found the
# touch panel. Reset the board (tap RESET) if you attached too late to see it.
#
source "$(dirname "${BASH_SOURCE[0]}")/common.sh"
need_cli

PORT="${1:-}"
[ -z "$PORT" ] && { PORT="$(detect_port)" || die "No board found. Is it plugged in?"; }

bold "Monitoring $PORT at ${BAUD} baud — Ctrl-C to quit"
echo
arduino-cli monitor --port "$PORT" --config "baudrate=$BAUD"
