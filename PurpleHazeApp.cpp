/*
 * PurpleHazeApp:
 *    A simple example of a WebThingApp that display times and currency
 *    exchange rates.
 *
 * CUSTOMIZATION:
 * o To create your own app based on this sample, search for 'CUSTOM'
 *   to find areas that should be changed/customized
 * o Areas labeled 'BOILERPLATE' may require mechanical changes such as updating
 *   the names you have used for classes and variables
 *
 */


//--------------- Begin:  Includes ---------------------------------------------
//                                  Hardware Definitions
#include "src/hardware/HWConfig.h"
//                                  Core Libraries
//                                  Third Party Libraries
#include <ArduinoLog.h>
#include <TimeLib.h>
//                                  WebThing Includes
#include <DataBroker.h>
#include <WebUI.h>
#include <clients/BlynkMgr.h>
#include <sensors/WeatherUtils.h>
#include <clients/WeatherBlynkPublisher.h>
#include <clients/AQIBlynkPublisher.h>
//                                  WebThingApp Includes
#include <gui/Display.h>
#include <gui/ScreenMgr.h>
#include <plugins/PluginMgr.h>
#include <plugins/common/GenericPlugin.h>
#include <plugins/common/BlynkPlugin.h>
//                                  Local Includes
#include "PurpleHazeApp.h"
#include "PHSettings.h"
#include "PHWebUI.h"
#include "PHDataSupplier.h"
#include "src/screens/AppTheme.h"
//--------------- End:    Includes ---------------------------------------------


//--------------- Begin:  Includes ---------------------------------------------
//                                  Core Libraries
//                                  Third Party Libraries
//                                  WebThing Includes
//                                  Local Includes
//--------------- End:    Includes ---------------------------------------------


/*------------------------------------------------------------------------------
 *
 * Constants
 *
 *----------------------------------------------------------------------------*/

// CUSTOM: Update these strings for your app:
static const char* VersionString = "0.0.9";
static const char* AppName = "PurpleHaze";
static const char* AppPrefix = "PH-";


/*------------------------------------------------------------------------------
 *
 * Private Utility Functions
 *
 *----------------------------------------------------------------------------*/

Plugin* pluginFactory(const String& type) {
  Plugin *p = NULL;

  // CUSTOM: Choose which plugins you'd like to load
  if      (type.equalsIgnoreCase("generic")) { p = new GenericPlugin(); }
  else if (type.equalsIgnoreCase("blynk"))   { p = new BlynkPlugin();   }  

  if (p == NULL) {
    Log.warning("Unrecognized plugin type: %s", type.c_str());
  }
  return p;
}

/*------------------------------------------------------------------------------
 *
 * Class function to create and start the PurpleHazeApp singleton
 *
 *----------------------------------------------------------------------------*/

static PHSettings theSettings;  // Allocate storage for the app settings

void PurpleHazeApp::create() {
  // BOILERPLATE
  PluginMgr::setFactory(pluginFactory);
  PurpleHazeApp* app = new PurpleHazeApp(&theSettings);

  app->begin();
}


/*------------------------------------------------------------------------------
 *
 * PurpleHazeApp Public Functions
 *
 *----------------------------------------------------------------------------*/


PurpleHazeApp::PurpleHazeApp(PHSettings* settings) :
    WTAppImpl(AppName, AppPrefix, VersionString, settings)
{
  // CUSTOM: Perform any object initialization here
  // None needed in this case
}


/*------------------------------------------------------------------------------
 *
 * Mandatory WTAppImpl virtual functions
 *
 *----------------------------------------------------------------------------*/

void PurpleHazeApp::app_registerDataSuppliers() {
  // BOILERPLATE
  DataBroker::registerMapper(PHDataSupplier::dataSupplier, PHDataSupplier::PHPrefix);
}

void PurpleHazeApp::app_initWebUI() {
  // BOILERPLATE
  PHWebUI::init();
}

void PurpleHazeApp::app_loop() {
  // CUSTOM: Perform any app-specific periodic activity
  // Note that app_conditionalUpdate() is called for you automatically on a
  // periodic basis, so no need to do that here.

#if defined(HAS_AQI_SENSOR)
  aqiMgr.loop();
#endif
}

void PurpleHazeApp::app_initClients() {
  // CUSTOM: If your app has any app-specific clients, initilize them now

  ScreenMgr.showActivityIcon(AppTheme::Color_UpdatingRates);

  prepBlynk();

  ScreenMgr.hideActivityIcon();
}

void PurpleHazeApp::app_conditionalUpdate(bool force) {
  // CUSTOM: Update any app-specific clients

  (void)force;
  #if defined(HAS_AQI_SENSOR)
    static uint32_t lastTimestamp = 0;

    const AQIReadings& aqiReadings = aqiMgr.getLastReadings();
    if (aqiReadings.timestamp != lastTimestamp) {
      busyIndicator->setColor(0, 255, 0);
      uint16_t quality = aqiMgr.derivedAQI(aqiReadings.env.pm25);
      qualityIndicator->setColor(aqiMgr.colorForQuality(quality));
      lastTimestamp = aqiReadings.timestamp;
      busyIndicator->off();
    }
  #endif

  #if defined(HAS_WEATHER_SENSOR)
    weatherMgr.takeReadings(force);
  #endif

  BlynkMgr::publish();
}

Screen* PurpleHazeApp::app_registerScreens() {
  // CUSTOM: Associate a confirm/cancel buttons with the reboot screen
  screens.rebootScreen->setButtons(hwConfig.advanceButton, hwConfig.previousButton);

  // CUSTOM: Register any app-specific Screen objects
  return appScreens.registerScreens(&theSettings, pluginMgr, aqiMgr, weatherMgr);
}

/*------------------------------------------------------------------------------
 *
 * Functions for use by other parts of the app
 *
 *----------------------------------------------------------------------------*/

  void PurpleHazeApp::setIndicatorBrightness(uint8_t b) {
    if (indicators) indicators->setBrightness((b*255L)/100);
  }

/*------------------------------------------------------------------------------
 *
 * Optional WTAppImpl virtual functions
 *
 *----------------------------------------------------------------------------*/

void PurpleHazeApp::app_configureHW() {
  // CUSTOM: IConfigure/nitialize any app-specific hardware here
  // At this point, the settings have been read, but almost nothing
  // else has been done.

  configureDisplay(); // Sets display parameters
  configurePins();
  configureIndicators();
  prepSensors();
  
  // CUSTOM: Register any physical buttons that are connected
  for (int i = 0; i < hwConfig.nPhysicalButtons; i++) {
    uint8_t pin = hwConfig.physicalButtons[i];
    if (pin != UNUSED_PIN) {
      WebThing::buttonMgr.addButton(pin);
    }
  }

  // Technically this should be done later, since ScreenMgr.init() hasn't been
  // called yet.
  ScreenMgr.setSequenceButtons(hwConfig.advanceButton, hwConfig.previousButton);
}

/*------------------------------------------------------------------------------
 *
 * Protected functions
 *
 *----------------------------------------------------------------------------*/

void PurpleHazeApp::configModeCallback(const String &ssid, const String &ip) {
  busyIndicator->setColor(0, 0, 255);
  WTAppImpl::configModeCallback(ssid, ip);
}


/*------------------------------------------------------------------------------
 *
 * Private functions
 *
 *----------------------------------------------------------------------------*/

void PurpleHazeApp::prepBlynk() {
  BlynkMgr::init(phSettings->blynkAPIKey);

  // ----- Register the BME Publisher
  #if defined(HAS_WEATHER_SENSOR)
    WeatherBlynkPublisher* bp = new WeatherBlynkPublisher(&weatherMgr);
    BlynkMgr::registerPublisher(bp);
  #endif

  // ----- Register the AQI Publisher
  #if defined(HAS_AQI_SENSOR)
    AQIBlynkPublisher* ap = new AQIBlynkPublisher(&aqiMgr);
    BlynkMgr::registerPublisher(ap);
  #endif
}  

//
// ----- Hardware Configuration
//

void PurpleHazeApp::prepSensors() {
  #if defined(HAS_AQI_SENSOR)
    streamToSensor.begin();
    if (!aqiMgr.init(streamToSensor.s, sensorIndicator)) {
      Log.error("Unable to connect to Air Quality Sensor!");
      qualityIndicator->setColor(255, 0, 0);
      sensorIndicator->setColor(255, 0, 0);
      busyIndicator->setColor(255, 0, 0);
    }
  #endif

  #if defined(HAS_WEATHER_SENSOR)
    auto weatherBusyCallBack = [this](bool busy) {
      if (busy) busyIndicator->setColor(0, 255, 0);
      else busyIndicator->off();
    };

    WeatherUtils::configureAvailableSensors(weatherMgr);
    weatherMgr.init(
      phSettings->weatherSettings.tempCorrection,
      phSettings->weatherSettings.humiCorrection,
      WebThing::settings.elevation,
      weatherBusyCallBack);
  #endif
}

void PurpleHazeApp::configureDisplay() {
  // Set the display options before we fire up the display!
  Display.setDeviceOptions(&hwConfig.displayDeviceOptions);
}

void PurpleHazeApp::configurePins() {
  // Initialize the synthetic grounds
  for (int i = 0; i < hwConfig.nSyntheticGrounds; i++) {
    uint8_t pin = hwConfig.syntheticGrounds[i];
    if (pin != UNUSED_PIN) {
      pinMode(pin, OUTPUT);
      digitalWrite(pin, LOW);      
    }
  }

  if (hwConfig.corePins.scl != UNUSED_PIN && hwConfig.corePins.sda != UNUSED_PIN) {
    // Override the deault I2C Pins
    Wire.begin(SDA_PIN, SCL_PIN);
  }
}

void PurpleHazeApp::configureIndicators() {
  if (NEOPIXEL_PIN == UNUSED_PIN) {
    indicators = NULL;
    qualityIndicator = new Indicator();
    sensorIndicator = new Indicator();
    busyIndicator = new Indicator();
  } else {
    indicators = new NeoPixelIndicators(NEOPIXEL_PIN, 3, NEOPIXEL_TYPE);
    indicators->begin();
    indicators->setBrightness(phSettings->iBright);
    NeoPixelIndicator* npi;
    npi = new NeoPixelIndicator(); npi->begin(indicators, 0); qualityIndicator = npi;
    npi = new NeoPixelIndicator(); npi->begin(indicators, 1); sensorIndicator = npi;
    npi = new NeoPixelIndicator(); npi->begin(indicators, 2); busyIndicator = npi;
  }
  qualityIndicator->setColor(0x969697);  // No data available yet
  sensorIndicator->off();  // No data available yet
  busyIndicator->off();  // No data available yet
}

