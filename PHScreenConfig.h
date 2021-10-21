/*
 * PHScreenConfig
 *     Configure the app-specific screens used by PurpleHaze
 *
 */


#ifndef PHScreenConfig_h
#define PHScreenConfig_h

#include "PurpleHazeApp.h"
#include "gui/ScreenMgr.h"
#include <plugins/PluginMgr.h>
#include "src/screens/SplashScreen.h"
#include "src/screens/HomeScreen.h"
#include "src/screens/AQIScreen.h"
#include "src/screens/PHGraphScreen.h"

class PHScreens {
public:
  SplashScreen*       splashScreen;
  HomeScreen*         homeScreen;
  AQIScreen*          aqiScreen;
  AQIGraphScreen*     aqiGraphScreen;
  WeatherGraphScreen* weatherGraphScreen;

	Screen* registerScreens(PluginMgr& pluginMgr) {
	  // CUSTOM: Register any app-specific Screen objects
	  splashScreen = new SplashScreen();
	  homeScreen = new HomeScreen();
	  aqiScreen = new AQIScreen();
	  aqiGraphScreen = new AQIGraphScreen();
	  weatherGraphScreen = new WeatherGraphScreen();

	  ScreenMgr.registerScreen("Splash", splashScreen);
	  ScreenMgr.registerScreen("Home", homeScreen);
	  ScreenMgr.registerScreen("AQI", aqiScreen);
	  ScreenMgr.registerScreen("AGraph", aqiGraphScreen);
	  ScreenMgr.registerScreen("WGraph", weatherGraphScreen);
	  ScreenMgr.setAsHomeScreen(homeScreen);

	  // CUSTOM: Add a sequence of screens that the user can cycle through
	  BaseScreenMgr::ScreenSequence* sequence = new BaseScreenMgr::ScreenSequence;
	  sequence->push_back(homeScreen);
	  sequence->push_back(aqiScreen);
	  sequence->push_back(aqiGraphScreen);
	  sequence->push_back(weatherGraphScreen);
	  sequence->push_back(wtAppImpl->screens.weatherScreen);
	  sequence->push_back(wtAppImpl->screens.forecastFirst3);
	  sequence->push_back(wtAppImpl->screens.forecastLast2);
	  // Add any plugins to the sequence
	  uint8_t nPlugins = pluginMgr.getPluginCount();
	  for (int i = 0; i < nPlugins; i++) {
	    Plugin* p = pluginMgr.getPlugin(i);
	    sequence->push_back(p->getFlexScreen());
	  }
	  sequence->push_back(wtAppImpl->screens.infoScreen);
	  ScreenMgr.setSequence(sequence);

	  return splashScreen;
	}
};

#endif	// PHScreenConfig_h