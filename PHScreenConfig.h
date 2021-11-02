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
#if defined(HAS_AQI_SENSOR)
  AQIScreen*          aqiScreen;
  AQIGraphScreen*     aqiGraphScreen;
#endif
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

    ScreenMgr.registerScreen("Home", homeScreen);
    ScreenMgr.setAsHomeScreen(homeScreen);
    ScreenMgr.registerScreen("Splash", splashScreen, true);

#if defined(HAS_AQI_SENSOR)
	  aqiScreen = new AQIScreen();
	  aqiGraphScreen = new AQIGraphScreen(aqiMgr);
	  ScreenMgr.registerScreen("AQI", aqiScreen);
	  ScreenMgr.registerScreen("AQI-Graph", aqiGraphScreen);
	  aqiGraphScreen->selectBuffer(settings->aqiSettings.graphRange);
#else
	  (void)aqiMgr;
#endif
#if defined(HAS_WEATHER_SENSOR)
    weatherGraphScreen = new WeatherGraphScreen(weatherMgr);
	  ScreenMgr.registerScreen("Temp-Graph", weatherGraphScreen);
	  weatherGraphScreen->selectBuffer(settings->weatherSettings.graphRange);
#else
	  (void)weatherMgr;
#endif

	  // CUSTOM: Add a sequence of screens that the user can cycle through
	  auto& sequence = ScreenMgr.sequence;
    sequence.clear();
    sequence.push_back(homeScreen);
#if defined(HAS_AQI_SENSOR)
	  sequence.push_back(aqiScreen);
	  sequence.push_back(aqiGraphScreen);
#endif
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

  // Log.verbose("PHScreens::registerScreens, The default screen sequence is:");
  // for (const Screen* s : sequence) {
  // 	Log.verbose("%s", s->name.c_str());
  // }
    
  // Reconcile the screen list from the settings, with the list of
  // screens we're actually using.
  ScreenMgr.reconcileScreenSequence(settings->screenSettings);

	  return splashScreen;
	}
};

#endif	// PHScreenConfig_h