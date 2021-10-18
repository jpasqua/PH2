//--------------- Begin:  Includes ---------------------------------------------
//                                  Core Libraries
#include <Arduino.h>
//                                  Third Party Libraries
#include <TimeLib.h>
//                                  WebThingApp
#include <gui/Display.h>
#include <gui/Theme.h>
//                                  Local Includes
#include "../../PurpleHazeApp.h"
//--------------- End:    Includes ---------------------------------------------


static inline uint16_t compose(int h, int m) { return(h * 100 + m); }

static constexpr auto Reading_Font = Display.FontID::S10;
static constexpr uint16_t Reading_FontHeight = 13;
static constexpr uint16_t Reading_XOrigin = 1;
static constexpr uint16_t Reading_YOrigin = 51;
static constexpr uint16_t Reading_Width = 63;
static constexpr uint16_t Reading_Height = Reading_FontHeight;
static constexpr uint16_t Reading_BorderSize = 1;

static constexpr auto Label_Font = Display.FontID::S10;
static constexpr uint16_t Label_FontHeight = 13;


HomeScreen::HomeScreen() {
  // uint16_t x = Reading_XOrigin;
  // labels = new Label[NCurrencyLabels];
  // for (int i = 0; i < NCurrencyLabels; i++) {
  //   labels[i].init(x, Reading_YOrigin, Reading_Width, Reading_Height, i);
  //   x += Reading_Width - Reading_BorderSize;  // Make the borders overlap
  // }
}

void HomeScreen::display(bool) {
  auto oled = Display.oled;
  
  oled->clear();

  bool am = true;
  int  m = minute();
  int  h = hour();
  compositeTime = compose(h, m);

  if (h > 12) { h -= 12; am = false; }
  else if (h == 0) { h = 12;}
  else if (h == 12) { am = false; }

  constexpr uint8_t bufSize = 32;
  char buf[bufSize];
  snprintf(buf, bufSize, "%d:%02d", h, m);
  Display.setFont(Display.FontID::D32);
  oled->setTextAlignment(TEXT_ALIGN_RIGHT);
  oled->drawString(Display.Width-24, 2, buf);
  oled->setTextAlignment(TEXT_ALIGN_LEFT);
  Display.setFont(Display.FontID::S10);
  oled->drawString(Display.Width-20, 4, am ? "AM" : "PM");

  drawLabels();
  drawReadings();

  oled->display();
}

void HomeScreen::processPeriodicActivity() {
  if (compositeTime != compose(hour(), minute())) display(true);
}

// ----- Private Functions

void HomeScreen::drawLabels() {
  Display.oled->setTextAlignment(TEXT_ALIGN_CENTER);
  Display.oled->setColor(Theme::Color_NormalText);
  Display.setFont(Label_Font);

  uint16_t yPos = Reading_YOrigin - Label_FontHeight + 1; // Add an extra pixel for padding
  Display.oled->drawString(Display.XCenter/2, yPos, "AQI");
  Display.oled->drawString(Display.XCenter + Display.XCenter/2, yPos, "Temp");
}

void HomeScreen::drawReadings() {
  Label l;

  l.init(0, Reading_YOrigin, Reading_Width, Reading_Height, 0);
  l.drawSimple("34", Reading_Font, 1, WHITE, WHITE, BLACK);
  l.region.x += Display.Width/2;
  l.drawSimple("74", Reading_Font, 1, WHITE, WHITE, BLACK);
}
