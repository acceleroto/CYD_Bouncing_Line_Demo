// This animates a series of colored lines that bounce around the screen
// somewhat similar to some old school screen savers. It was written for
// the Cheap Yellow Display (CYD) model ESP32-2432S028R.
//
// This was made mostly as a vibe coding test with Cursor. The code is
// kinda cheesy, not well thought out and has bugs. It works well enough
// for a demo though.

#include <SPI.h>
#include <TFT_eSPI.h>

TFT_eSPI tft = TFT_eSPI();

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240
#define LINE_COLOR TFT_RED
#define BG_COLOR TFT_BLACK
#define FRAME_DELAY 0  // delay for the main loop in milliseconds
#define TRAIL_LENGTH 5  // Number of trail lines. This only works at 5 with the code below.
#define TRAIL_SPACING 3 // Factor to increase spacing between trail lines
#define VELOCITY_MIN 5  // Minimum velocity for bounces
#define VELOCITY_MAX 12  // Maximum velocity for bounces

// Structure to hold the end points of the line and their velocities
struct Point {
  float x;
  float y;
  float vx;
  float vy;
};

// Structure to hold trail positions
struct TrailPoint {
  float x1, y1, x2, y2;
};

Point p1 = {random(0, SCREEN_WIDTH), random(0, SCREEN_HEIGHT), random(VELOCITY_MIN, VELOCITY_MAX), random(VELOCITY_MIN, VELOCITY_MAX)};
Point p2 = {random(0, SCREEN_WIDTH), random(0, SCREEN_HEIGHT), random(VELOCITY_MIN, VELOCITY_MAX), random(VELOCITY_MIN, VELOCITY_MAX)};

// Trail array to store previous positions
TrailPoint trail[TRAIL_LENGTH];
int trailIndex = 0;
uint32_t frameCounter = 0; // Counter to control trail spacing

// Trail colors (progressively dimmer) - medium red to white
uint16_t trailColors[TRAIL_LENGTH] = {
  0x6000, // Blood red - back trail line
  0xCA0A, //
  0xFB0F, //
  0xE004, //
  0xF840  // Candy apple red - front trail line (not currently drawn)
};

/*
// Trail colors (progressively dimmer) - medium red to white
uint16_t trailColors[TRAIL_LENGTH] = {
  0xA800, // Medium red (153,0,0) - back trail line
  0xB880, // (178,64,64)
  0xE108, // (204,128,128)
  0xF3BE, // (229,191,191)
  0xFFFF  // White (255,255,255) - front trail line
};
*/

/*
// a a fading gray set of colors
uint16_t trailColors[TRAIL_LENGTH] = {
  0xFFFF, // White (255,255,255)
  0xE73A, // (229,229,229)
  0xCE59, // (204,204,204)
  0xB5B6, // (178,178,178)
  0xAD55  // Medium grey (153,153,153)
};
*/

void setup() {
  tft.init();
  tft.setRotation(3);//1 & 3 are the landscape rotations
  tft.fillScreen(BG_COLOR);
  
  // Initialize trail with current positions
  for (int i = 0; i < TRAIL_LENGTH; i++) {
    trail[i] = {p1.x, p1.y, p2.x, p2.y};
  }
}

void loop() {
  // Update positions every frame
  p1.x += p1.vx;
  p1.y += p1.vy;
  p2.x += p2.vx;
  p2.y += p2.vy;

  // Bounce logic for point 1
  if (p1.x <= 0) {
    p1.vx = random(VELOCITY_MIN, VELOCITY_MAX); // Random positive velocity
    p1.x = 0 + p1.vx; // Move away from the edge
  } else if (p1.x >= SCREEN_WIDTH - 1) {
    p1.vx = -random(VELOCITY_MIN, VELOCITY_MAX); // Random negative velocity
    p1.x = (SCREEN_WIDTH - 1) + p1.vx; // Move away from the edge
  }
  
  if (p1.y <= 0) {
    p1.vy = random(VELOCITY_MIN, VELOCITY_MAX); // Random positive velocity
    p1.y = 0 + p1.vy; // Move away from the edge
  } else if (p1.y >= SCREEN_HEIGHT - 1) {
    p1.vy = -random(VELOCITY_MIN, VELOCITY_MAX); // Random negative velocity
    p1.y = (SCREEN_HEIGHT - 1) + p1.vy; // Move away from the edge
  }

  // Bounce logic for point 2
  if (p2.x <= 0) {
    p2.vx = random(VELOCITY_MIN, VELOCITY_MAX); // Random positive velocity
    p2.x = 0 + p2.vx; // Move away from the edge
  } else if (p2.x >= SCREEN_WIDTH - 1) {
    p2.vx = -random(VELOCITY_MIN, VELOCITY_MAX); // Random negative velocity
    p2.x = (SCREEN_WIDTH - 1) + p2.vx; // Move away from the edge
  }
  
  if (p2.y <= 0) {
    p2.vy = random(VELOCITY_MIN, VELOCITY_MAX); // Random positive velocity
    p2.y = 0 + p2.vy; // Move away from the edge
  } else if (p2.y >= SCREEN_HEIGHT - 1) {
    p2.vy = -random(VELOCITY_MIN, VELOCITY_MAX); // Random negative velocity
    p2.y = (SCREEN_HEIGHT - 1) + p2.vy; // Move away from the edge
  }

  // Store current positions in trail every TRAIL_SPACING frames
  if (frameCounter % TRAIL_SPACING == 0) {
    trail[trailIndex] = {p1.x, p1.y, p2.x, p2.y};
    trailIndex = (trailIndex + 1) % TRAIL_LENGTH;
  }

  // Clear the entire screen
  tft.fillScreen(BG_COLOR);

  // Draw trail lines (from oldest to newest)
  //for (int i = 0; i < TRAIL_LENGTH; i++) {
  for (int i = 0; i < TRAIL_LENGTH - 1; i++) { //the 5th trail line (the one closest to the leading line) looks bad, so skip it.
    int index = (trailIndex + i) % TRAIL_LENGTH;
    // Interpolate trail positions to make them move smoothly
    float progress = (float)(frameCounter % TRAIL_SPACING) / TRAIL_SPACING;
    float x1, y1, x2, y2;
    
    if (i == 0) {
      // Oldest trail - interpolate between current and next trail position
      int nextIndex = (trailIndex + i + 1) % TRAIL_LENGTH;
      x1 = trail[index].x1 + (trail[nextIndex].x1 - trail[index].x1) * progress;
      y1 = trail[index].y1 + (trail[nextIndex].y1 - trail[index].y1) * progress;
      x2 = trail[index].x2 + (trail[nextIndex].x2 - trail[index].x2) * progress;
      y2 = trail[index].y2 + (trail[nextIndex].y2 - trail[index].y2) * progress;
    } else if (i == TRAIL_LENGTH - 1) {
      // Newest trail - interpolate between stored position and current line position
      x1 = trail[index].x1 + (p1.x - trail[index].x1) * progress;
      y1 = trail[index].y1 + (p1.y - trail[index].y1) * progress;
      x2 = trail[index].x2 + (p2.x - trail[index].x2) * progress;
      y2 = trail[index].y2 + (p2.y - trail[index].y2) * progress;
    } else {
      // Other trails - interpolate between their stored position and next
      int nextIndex = (trailIndex + i + 1) % TRAIL_LENGTH;
      x1 = trail[index].x1 + (trail[nextIndex].x1 - trail[index].x1) * progress;
      y1 = trail[index].y1 + (trail[nextIndex].y1 - trail[index].y1) * progress;
      x2 = trail[index].x2 + (trail[nextIndex].x2 - trail[index].x2) * progress;
      y2 = trail[index].y2 + (trail[nextIndex].y2 - trail[index].y2) * progress;
    }
    
    tft.drawLine(x1, y1, x2, y2, trailColors[i]);
  }

  // Draw current line (brightest)
  tft.drawLine(p1.x, p1.y, p2.x, p2.y, LINE_COLOR);

  frameCounter++;
  if (frameCounter >= UINT32_MAX) frameCounter = 0; //make sure the frame counter never reaches its limit
  delay(FRAME_DELAY); //maybe use this is you have a really fast processor & want to slow things down.
}
