//--------------- Begin:  Includes ---------------------------------------------
//                                  Core Libraries
#include <Arduino.h>
//                                  Third Party Libraries
#include <Output.h>
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
static constexpr uint16_t Reading_XOrigin = 0;
static constexpr uint16_t Reading_YOrigin = 51;
#if defined(HAS_WEATHER_SENSOR)
  static constexpr uint16_t Reading_Width = 63;
#else
  static constexpr uint16_t Reading_Width = 128;
#endif
static constexpr uint16_t Reading_Height = Reading_FontHeight;
static constexpr uint16_t Reading_BorderSize = 1;

static constexpr auto Label_Font = Display.FontID::S10;
static constexpr uint16_t Label_FontHeight = 13;


HomeScreen::HomeScreen() {

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
    Display.oled->drawString(Reading_Width/2, yPos, "AQI");
  #if defined(HAS_WEATHER_SENSOR)
    Display.oled->drawString(Reading_Width + Reading_Width/2, yPos, "Temp");
  #endif
}

void HomeScreen::drawReadings() {
  const AQIReadings& aqiReadings = phApp->aqiMgr.getLastReadings();
  String aqiToDisplay(phApp->aqiMgr.derivedAQI(aqiReadings.env.pm25));

  Label l;
  l.init(0, Reading_YOrigin, Reading_Width, Reading_Height, 0);
  l.drawSimple(aqiToDisplay, Reading_Font, Reading_BorderSize, WHITE, WHITE, BLACK);
#if defined(HAS_WEATHER_SENSOR)
  int temp = Output::temp(phApp->weatherMgr.getLastReadings().temp);
  l.region.x += Reading_Width;
  l.drawSimple(String(temp), Reading_Font, Reading_BorderSize, WHITE, WHITE, BLACK);
#endif
}
