/*
 * PurpleHazeApp:
 *    This is the core of the MultiMon functionality. It is invoked
 *    by the primary sketch ".ino" file.
 *
 * NOTES:
 * o PurpleHazeApp is derived from WTAppImpl, which is itself derived
 *   from WTApp.
 * o This is a singleton and is made available to the whole app via
 *   an extern declaration in WTApp.h:
 *       WTApp* wtApp;
 * o This must be cast back to PurpleHazeApp* in order to get to app-specific
 *   data and functions. Technically this should be a dynamic_cast, but for
 *   efficiency, we perform a "C-style" cast.
 * o Macros are provided to easily get the app and settings in their
 *   specialized forms.
 *
 * Customization:
 * o To add a new screen to the app, declare it here and instantiate it
 *   in the associated .cpp file.
 */


#ifndef PurpleHazeApp_h
#define PurpleHazeApp_h

//--------------- Begin:  Includes ---------------------------------------------
//                                  Core Libraries
//                                  Third Party Libraries
#include <Indicators.h>
//                                  WebThing Includes
#include <WTAppImpl.h>
#include <sensors/WeatherMgr.h>
//                                  Local Includes
#include "PHSettings.h"
#include "src/clients/AQIMgr.h"
#include "src/screens/SplashScreen.h"
#include "src/screens/HomeScreen.h"
#include "src/screens/AQIScreen.h"
#include "src/hardware/SecondarySerial.h"
//--------------- End:    Includes ---------------------------------------------


// A few convenience macros:
// phApp simply yields the wtApp singleton cast as a PurpleHazeApp*
// phSettings yields the settings object from wtApp, cast as a PHSettings*
#define phApp ((PurpleHazeApp*)wtApp)
#define phSettings ((PHSettings*)phApp->settings)


class PurpleHazeApp : public WTAppImpl {
public:
  static void create();

  AQIMgr aqiMgr;
  WeatherMgr weatherMgr;

  Indicator* sensorIndicator;
  Indicator* qualityIndicator;
  Indicator* busyIndicator;
  NeoPixelIndicators* indicators;

  // CUSTOM: Screens implemented by this app
  SplashScreen*   splashScreen;
  HomeScreen*     homeScreen;
  AQIScreen*      aqiScreen;

  // CUSTOM: Data defined by this app which is available to the whole app


  // ----- Functions that *must* be provided by subclasses
  virtual void app_registerDataSuppliers() override;
  virtual Screen* app_registerScreens() override;
  virtual void app_initWebUI() override;
  virtual void app_initClients() override;
  virtual void app_conditionalUpdate(bool force = false) override;
  virtual void app_loop() override;

  // ----- Functions that *may* be provided by subclasses
  virtual void app_registerButtons() override;

  // ----- Public functions
  PurpleHazeApp(PHSettings* settings);
  void setIndicatorBrightness(uint8_t b);

protected:
  virtual void configModeCallback(const String &ssid, const String &ip) override;

private:
  void prepBlynk();
  void prepSensors();
  void configureDisplay();
  void configureNonButtonPins();
  void configureIndicators();

  SecondarySerial streamToSensor;
};

#endif	// PurpleHazeApp_h
