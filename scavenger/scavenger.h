/*
 * ==============================================================================
 *  scavenger.h  —  掃除屋 SCAVENGER (스캐빈저): the pair that tidies things away
 * ==============================================================================
 *  愛 (ai) is the big one: a deep-sea angler with a lamp over its head and a
 *  mouth that takes anything. 誠 (makoto) is the little one that gives back
 *  whatever 愛 swallowed.
 *
 *  The act:
 *    1  black water, and a small green light drifting in it
 *    2  the light comes closer — the fish resolves out of the dark, grinning
 *    3  the jaw drops, it lunges, and the thing is gone
 *    4  the lump travels down its body; it shudders
 *    5  it spits the thing back out; 誠 is waiting there to collect it
 *    6  the fish sinks away, the lamp going out last
 *
 *  Drawn on the low-res canvas from lowres.h. The fish faces left.
 * ==============================================================================
 */

#pragma once

// ---- Palette ----------------------------------------------------------------
#define S_VOID      0x0041   // #050a08 the water it hides in
#define S_INK       0x10C2   // #101a12 fins, spots, outline
#define S_BODY_D    0x2A45   // #2c4a28 flank in shadow
#define S_BODY_M    0x4BC7   // #4e7a3e flank
#define S_BODY_L    0x7D6B   // #7fae5e lit by the lamp
#define S_BELLY     0x9E2F   // #9cc47a belly / throat
#define S_MOUTH     0x08A2   // #0e1410 inside the mouth
#define S_GUM       0x2183   // #23301f gums
#define S_TOOTH     0xF79D   // #f2f2ea teeth
#define S_TOOTH_D   0xBE16   // #b9c0b0 teeth in shadow
#define S_EYE       0xF799   // #f7f0cf eye white
#define S_IRIS      0xED43   // #eaa81b iris
#define S_PUPIL     0x3941   // #3a2a08 pupil
#define S_LAMP      0xEFFA   // #eaffd0 the lamp's core
#define S_LAMP_M    0x9F8B   // #9cf25a the lamp
#define S_LAMP_D    0x3BC5   // #3f7a2a its glow on the water
#define S_GLOW      0x1983   // #1a3018 the far edge of the glow
#define S_MAKOTO    0xCF17   // #cfe0b8 誠's pale face
#define S_MAKOTO_D  0x1923   // #1a2418 誠's body
#define S_ITEM      0x9F8B   // what comes back out

// A thing the scavenger tidied away: a chunky canister.
static void scavItem(int cx, int cy, int s) {
  if (s < 2) return;
  lo->fillRect(cx - s, cy - s, s * 2, s * 2, S_INK);
  lo->fillRect(cx - s + 1, cy - s + 1, s * 2 - 2, s * 2 - 2, S_ITEM);
  lo->fillRect(cx - s + 1, cy - s + 1, s, s / 2 + 1, S_LAMP);
  lo->fillRect(cx - s / 2, cy - 1, s, 2, S_INK);
}

// 誠: a small pale face on a dark little body, holding station in the water.
static void scavMakoto(int cx, int cy, int r, bool blink) {
  if (r < 3) return;
  for (int s = -1; s <= 1; s += 2)                       // fins
    lo->fillTriangle(cx + s * r, cy, cx + s * (r + 3), cy - 3,
                     cx + s * (r + 2), cy + 3, S_MAKOTO_D);
  lo->fillCircle(cx, cy + 1, r, S_MAKOTO_D);             // body
  lo->fillCircle(cx, cy, r - 1, S_MAKOTO);               // face
  if (blink) {
    lo->drawFastHLine(cx - r / 2 - 1, cy - 1, 2, S_PUPIL);
    lo->drawFastHLine(cx + r / 2 - 1, cy - 1, 2, S_PUPIL);
  } else {
    lo->fillRect(cx - r / 2 - 1, cy - 2, 2, 3, S_IRIS);
    lo->fillRect(cx + r / 2 - 1, cy - 2, 2, 3, S_IRIS);
  }
  lo->drawFastHLine(cx - r / 2, cy + r / 2, r, S_PUPIL);  // the little grin
}

// The lamp on its stalk, and the light it throws into the water.
static void scavLamp(int x, int y, int glow) {
  if (glow <= 0) return;
  int halo = 4 + glow;
  loSeed(0x1AB0 + glow);
  for (int dy = -halo; dy <= halo; dy++) {               // dithered glow
    for (int dx = -halo; dx <= halo; dx++) {
      int d2 = dx * dx + dy * dy;
      if (d2 > halo * halo) continue;
      if ((loRand() & 7) > 3) continue;
      lo->drawPixel(x + dx, y + dy, (d2 < (halo * halo) / 3) ? S_LAMP_D : S_GLOW);
    }
  }
  lo->fillCircle(x, y, 2 + glow / 5, S_LAMP_M);
  lo->fillCircle(x - 1, y - 1, 1 + glow / 8, S_LAMP);
}

/*
 *  愛, side on, facing left.
 *    cx, cy  center of the head
 *    r       head radius — the rest of the fish scales from it
 *    open    how far the lower jaw has dropped, in pixels
 *    glow    0..10, the lamp
 *    phase   wobble, for the tail and fins
 *    lump    0..100, where a swallowed thing is travelling down the body
 */
static void scavFish(int cx, int cy, int r, int open, int glow, int phase, int lump) {
  const int wob = ((phase / 2) & 1) ? 1 : -1;

  // --- tail and body, stretching back to the right --------------------------
  const int tail_x  = cx + r * 5 / 2;
  const int tail_y  = cy - r / 4 + wob * 2;                    // the tail swings
  const int top_y   = cy - r + 1, bot_y = cy + r - 1;          // where it meets the head

  // The flank: top edge sweeps down to the tail root, belly sweeps up to it.
  lo->fillTriangle(cx, top_y, tail_x, tail_y - r / 5, cx, bot_y, S_BODY_D);
  lo->fillTriangle(cx, bot_y, tail_x, tail_y + r / 5, tail_x, tail_y - r / 5, S_BODY_D);
  lo->fillTriangle(cx, bot_y, cx + r, cy + r / 2, tail_x, tail_y + r / 5, S_INK);

  // Tail fin: two ragged lobes, dark but still a shape against the water.
  lo->fillTriangle(tail_x - 2, tail_y - r / 5, tail_x + r, tail_y - r + wob * 3,
                   tail_x + r / 2, tail_y + r / 6, S_BODY_D);
  lo->fillTriangle(tail_x - 2, tail_y + r / 5, tail_x + r, tail_y + r / 2 + wob * 3,
                   tail_x + r / 2, tail_y - r / 6, S_INK);

  // Dorsal spikes, riding the top edge wherever it happens to be.
  for (int i = 0; i < 6; i++) {
    int sx = cx + (i * (tail_x - cx)) / 6;
    int sy = top_y + ((tail_y - r / 5 - top_y) * (sx - cx)) / (tail_x - cx);
    int sh = r / 3 - (i * r) / 18;
    if (sh < 3) sh = 3;
    lo->fillTriangle(sx, sy + 1, sx + r / 4, sy + 1, sx + r / 8, sy - sh, S_BODY_D);
  }

  // --- head -----------------------------------------------------------------
  lo->fillCircle(cx, cy, r, S_BODY_M);
  lo->fillCircle(cx - r / 5, cy - r / 2, r * 5 / 9, S_BODY_L);   // lit from above
  lo->fillCircle(cx + r / 3, cy + r / 3, r * 2 / 3, S_BODY_D);   // shaded flank

  loSeed(0xF15A);
  for (int i = 0; i < 14; i++) {                                 // spots
    int sx = cx - r + (loRand() * (r * 3)) / 256;
    int sy = cy - r + (loRand() * (r * 2)) / 256;
    int ss = 1 + (loRand() & 1);
    lo->fillRect(sx, sy, ss + 1, ss, S_INK);
  }

  // A swallowed thing, travelling backwards under the skin.
  if (lump > 0) {
    int lx = cx + (lump * (tail_x - cx - r / 2)) / 100;
    int ly = cy + r / 4 + ((tail_y - cy - r / 4) * (lx - cx)) / (tail_x - cx);
    int lr = (r / 5) - (lump * r) / 500;                 // and shrinks as it goes
    if (lr < 2) lr = 2;
    lo->fillCircle(lx, ly, lr, S_BELLY);
  }

  // --- the mouth: a grin from the snout back to the hinge --------------------
  const int ax = cx - r + 1, ay = cy + r / 3;          // snout end, low
  const int bx = cx + r - 2, by = cy + r / 8;          // hinge, further up
  if (open > 0) {
    lo->fillTriangle(ax, ay, bx, by, ax, ay + open, S_MOUTH);
    lo->fillTriangle(ax, ay + open, bx, by, cx + r / 4, cy + r + open / 2, S_BODY_M);
    lo->fillTriangle(ax, ay + open, cx + r / 4, cy + r + open / 2,
                     cx - r / 3, cy + r + open / 3, S_BELLY);
    lo->drawLine(ax, ay + open, bx, by, S_GUM);
  }
  lo->drawLine(ax, ay, bx, by, S_GUM);

  // --- teeth: long at the snout, shrinking toward the hinge -----------------
  const int span = bx - ax;
  for (int i = 0; i < 9; i++) {
    int t   = (i * span) / 9;
    int tx  = ax + t;
    int uy  = ay + ((by - ay) * t) / span;
    int len = r / 2 - (i * r) / 24;
    if (len < 3) len = 3;
    lo->fillTriangle(tx, uy, tx + 3, uy, tx + 1, uy + len, S_TOOTH);
    lo->drawLine(tx + 3, uy, tx + 1, uy + len, S_TOOTH_D);
    if (open > 0)
      lo->fillTriangle(tx, uy + open, tx + 3, uy + open, tx + 1, uy + open - len, S_TOOTH);
  }

  // --- the eye --------------------------------------------------------------
  int ex = cx - r / 3, ey = cy - r / 4;
  int er = (r / 6 < 3) ? 3 : r / 6;
  lo->fillCircle(ex, ey, er + 1, S_INK);
  lo->fillCircle(ex, ey, er, S_EYE);
  lo->fillCircle(ex - 1, ey, er / 2 + 1, S_IRIS);
  lo->drawPixel(ex - 1, ey, S_PUPIL);

  // --- the lamp, on its stalk over the snout --------------------------------
  int lx = cx - r - r / 3, ly = cy - r - r / 2;
  lo->drawLine(cx - r / 4, cy - r + 2, cx - r / 2, cy - r - r / 4, S_BODY_D);
  lo->drawLine(cx - r / 2, cy - r - r / 4, lx, ly, S_BODY_D);
  scavLamp(lx, ly, glow);
}

// Dissolve the picture into the water: 0 shows everything, 8 nothing.
static void scavDim(int amount) {
  if (amount <= 0) return;
  loSeed(0x51D0 + amount);
  uint16_t *fb = lo->getFramebuffer();
  for (int y = 0; y < LO_H; y++)
    for (int x = 0; x < LO_W; x++)
      if ((loRand() & 7) < amount) fb[y * LO_W + x] = S_VOID;
}

// ==============================================================================
//  The performance
// ==============================================================================
static void scavPlay() {
  if (!loBegin()) return;

  const int item_x = 30, item_y = 62;

  // --- scene 1: a light in the dark, and nothing else -----------------------
  for (int f = 0; f <= 7; f++) {
    lo->fillScreen(S_VOID);
    scavLamp(loLerp(140, 96, f, 7), 30 + ((f & 2) ? 1 : 0), loLerp(2, 7, f, 7));
    loMsg("SOMETHING IS FISHING", S_LAMP_D);
    loFrame();
  }

  // --- scene 2: the fish comes up out of it ---------------------------------
  for (int f = 0; f <= 9; f++) {
    lo->fillScreen(S_VOID);
    scavItem(item_x, item_y, 5);
    scavFish(loLerp(150, 96, f, 9), loLerp(74, 62, f, 9), loLerp(16, 30, f, 9),
             0, loLerp(7, 10, f, 9), f, 0);
    scavDim(loLerp(5, 0, f, 6));
    loFrame();
  }

  // --- scene 3: the jaw drops, and it comes on ------------------------------
  for (int f = 0; f <= 8; f++) {
    lo->fillScreen(S_VOID);
    scavItem(item_x, item_y, 5);
    scavFish(loLerp(96, 74, f, 8), 62, loLerp(30, 36, f, 8),
             loLerp(0, 30, f, 8), 10, f, 0);
    loMsg("SCAVENGER", S_LAMP_M);
    loFrame();
  }

  // --- scene 4: gone --------------------------------------------------------
  for (int f = 0; f <= 2; f++) {
    lo->fillScreen(S_VOID);
    int open = loLerp(30, 0, f, 2);
    if (open > 12) scavItem(item_x + 4, item_y, 5);
    scavFish(loLerp(74, 58, f, 2), 62, 36, open, 10, f, 0);
    loFrame();
  }
  lo->fillScreen(S_LAMP_M);
  loFrame();

  // --- scene 5: the lump goes down ------------------------------------------
  for (int f = 0; f <= 9; f++) {
    lo->fillScreen(S_VOID);
    scavFish(58 + ((f & 1) ? 1 : 0), 62, 36, (f & 1) ? 3 : 0, 9, f,
             loLerp(5, 95, f, 9));
    loMsg("TIDIED AWAY", S_BELLY);
    loFrame();
  }

  // --- scene 6: 誠 turns up for the handover --------------------------------
  for (int f = 0; f <= 5; f++) {
    lo->fillScreen(S_VOID);
    scavFish(loLerp(58, 74, f, 5), 62, loLerp(36, 31, f, 5),
             loLerp(0, 10, f, 5), 9, f, 95);
    scavMakoto(loLerp(-10, 18, f, 5), 40, 7, false);
    loMsg("HRRK---", S_BELLY);
    loFrame();
  }

  // --- scene 7: it gives the thing back -------------------------------------
  for (int f = 0; f <= 9; f++) {
    lo->fillScreen(S_VOID);
    scavFish(74, 62, 31, loLerp(34, 12, f, 9), 10, f, loLerp(95, 0, f, 4));
    int ix = loLerp(46, item_x, f, 9);
    int iy = loLerp(66, item_y, f, 9) - (f * (9 - f)) / 2;     // a flat arc
    scavItem(ix, iy, loLerp(3, 6, f, 9));
    for (int d = 0; d < 4; d++)                                 // spray
      lo->fillRect(ix + 8 + d * 5, iy + 3 + (d & 1) * 4, 2, 2, S_LAMP_D);
    scavMakoto(18, 40, 7, f > 6);
    loMsg("HERE. TAKE IT.", S_LAMP_M);
    loFrame();
  }

  // --- scene 8: back into the dark, lamp last -------------------------------
  for (int f = 0; f <= 9; f++) {
    lo->fillScreen(S_VOID);
    scavFish(loLerp(74, 130, f, 9), loLerp(62, 76, f, 9), loLerp(31, 24, f, 9),
             0, loLerp(10, 3, f, 9), f, 0);
    scavDim(loLerp(0, 7, f, 9));
    scavItem(item_x, item_y, 6);
    scavMakoto(18, 40, 7, false);
    loFrame();
  }

  // --- curtain: what came back ----------------------------------------------
  for (int f = 0; f <= 8; f++) {
    lo->fillScreen(S_VOID);
    scavItem(LO_W / 2, 58, 9);
    scavMakoto(LO_W / 2 + 30, 58, 7, (f & 2) != 0);
    const char *label = SPIT_LABEL;
    int lx = (LO_W - ((int)strlen(label) * 6 - 1)) / 2;
    lo->setTextSize(1);
    lo->setTextColor(0x0000);  lo->setCursor(lx + 1, 80); lo->print(label);
    lo->setTextColor(S_LAMP_M); lo->setCursor(lx, 79);    lo->print(label);
    loMsg("YOU GOT IT BACK", S_EYE);
    loFrame();
  }
}
