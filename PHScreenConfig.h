/*
 * PHScreenConfig
 *     Configure the app-specific screens used by PurpleHaze
 *
 */


#ifndef PHScreenConfig_h
#define PHScreenConfig_h

//--------------- Begin:  Includes ---------------------------------------------
//                                  Core Libraries
//                                  Third Party Libraries
//                                  WebThing Includes
#include <plugins/PluginMgr.h>
#include <gui/ScreenMgr.h>
#include <screens/oled/SensorGraphScreen.h>
//                                  Local Includes
#include "src/screens/SplashScreen.h"
#include "src/screens/HomeScreen.h"
#include "src/screens/AQIScreen.h"
//--------------- End:    Includes ---------------------------------------------


class PHScreens {
public:
  SplashScreen*       splashScreen;
  HomeScreen*         homeScreen;
  AQIScreen*          aqiScreen;
  AQIGraphScreen*     aqiGraphScreen;
#if defined(HAS_WEATHER_SENSOR)
  WeatherGraphScreen* weatherGraphScreen;
#endif

	Screen* registerScreens(
		PHSettings* settings, PluginMgr& pluginMgr,
		const AQIMgr& aqiMgr, const WeatherMgr& weatherMgr)
{
	  // CUSTOM: Register any app-specific Screen objects
	  splashScreen = new SplashScreen();
	  homeScreen = new HomeScreen();
	  aqiScreen = new AQIScreen();
	  aqiGraphScreen = new AQIGraphScreen(aqiMgr);

    ScreenMgr.registerScreen("Home", homeScreen);
    ScreenMgr.setAsHomeScreen(homeScreen);
	  ScreenMgr.registerScreen("AQI", aqiScreen);
	  ScreenMgr.registerScreen("AQI-Graph", aqiGraphScreen);
    ScreenMgr.registerScreen("Splash", splashScreen);

#if defined(HAS_WEATHER_SENSOR)
    weatherGraphScreen = new WeatherGraphScreen(weatherMgr);
	  ScreenMgr.registerScreen("Temp-Graph", weatherGraphScreen);
#endif

	  // CUSTOM: Add a sequence of screens that the user can cycle through
	  auto& sequence = ScreenMgr.sequence;
    sequence.clear();
    sequence.push_back(homeScreen);
	  sequence.push_back(aqiScreen);
	  sequence.push_back(aqiGraphScreen);
#if defined(HAS_WEATHER_SENSOR)
	  sequence.push_back(weatherGraphScreen);
#endif
	  sequence.push_back(wtAppImpl->screens.weatherScreen);
	  sequence.push_back(wtAppImpl->screens.forecastFirst3);
	  sequence.push_back(wtAppImpl->screens.forecastLast2);
	  // Add any plugins to the sequence
	  uint8_t nPlugins = pluginMgr.getPluginCount();
	  for (int i = 0; i < nPlugins; i++) {
	    Plugin* p = pluginMgr.getPlugin(i);
	    sequence.push_back(p->getFlexScreen());
	  }
	  sequence.push_back(wtAppImpl->screens.infoScreen);

Log.verbose("Screen Sequence");
for (const Screen* s : sequence) {
	Log.verbose("%s", s->registeredAs->c_str());
}
  // Reconcile the screen list from the settings, with the list of
  // screens we're actually using.
  ScreenMgr.reconcileScreenSequence(settings->screenSettings);

	  return splashScreen;
	}
};

#endif	// PHScreenConfig_h