#include <gui/devices/DeviceSelect.h>
#if DEVICE_TYPE == DEVICE_TYPE_OLED

//--------------- Begin:  Includes ---------------------------------------------
//                                  Core Libraries
#include <Arduino.h>
//                                  Third Party Libraries
#include <ArduinoLog.h>
#include <TimeLib.h>
//                                  WebThingApp
#include <gui/Display.h>
#include <gui/Theme.h>
//                                  Local Includes
#include "../../PurpleHazeApp.h"
#include "PHGraphScreen.h"
//--------------- End:    Includes ---------------------------------------------


AQIGraphScreen::AQIGraphScreen() {
  provider.setRange(AQIMgr::HistoryRange::Range_1Day);
  dataProvider = &provider;
  displayParams = { true, 0.0, 100.0 };
};


WeatherGraphScreen::WeatherGraphScreen() {
  provider.setRange(WeatherMgr::HistoryRange::Range_1Day);
  dataProvider = &provider;
  displayParams = { true, 30.0, 100.0 };
};


#endif