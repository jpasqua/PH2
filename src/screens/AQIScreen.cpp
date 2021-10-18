#include <gui/devices/DeviceSelect.h>
#if DEVICE_TYPE == DEVICE_TYPE_OLED

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
#include "AQIScreen.h"
#include "AQIIcons.h"
//--------------- End:    Includes ---------------------------------------------


void AQIScreen::display(bool) {
  const AQIReadings& aqiReadings = phApp->aqiMgr.getLastReadings();
  auto aqi = phApp->aqiMgr.derivedAQI(aqiReadings.env.pm25);
  Display.oled->clear();

  auto font = aqi < 200 ? Display.FontID::D32 : Display.FontID::D16;
  Display.setFont(font);
  Display.oled->setTextAlignment(TEXT_ALIGN_CENTER_BOTH);
  Display.oled->drawString(Display.XCenter + Display.XCenter/2, Display.YCenter, String(aqi));

  auto bracket = min(phApp->aqiMgr.aqiBracket(aqi), N_AQI_ICONS);
  const uint8_t* aqiIcon = AQILevels[bracket];
  Display.oled->drawXbm(0, 0, AQI_ICON_WIDTH, AQI_ICON_HEIGHT, aqiIcon);

  Display.oled->display();
}

void AQIScreen::processPeriodicActivity() { }


#endif