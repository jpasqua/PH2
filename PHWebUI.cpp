/*
 * PHWebUI:
 *    Implements a simple WebUI for PurpleHaze
 *                    
 * NOTES:
 *
 */


//--------------- Begin:  Includes ---------------------------------------------
//                                  Core Libraries
//                                  Third Party Libraries
#include <ArduinoLog.h>
#include <BPABasics.h>
#include <Output.h>
//                                  WebThing Includes
#include <sensors/AQIMgr.h>
#include <sensors/WeatherMgr.h>
//                                  WebThingApp Includes
#include <WebUIHelper.h>
//                                  Local Includes
#include "src/hardware/HWConfig.h"
#include "PurpleHazeApp.h"
#include "PHWebUI.h"
//--------------- End:    Includes ---------------------------------------------



// ----- BEGIN: WebUI namespace
namespace PHWebUI {

  // ----- BEGIN: PHWebUI::Internal
  namespace Internal {
    const __FlashStringHelper* APP_MENU_ITEMS = FPSTR(
      "<a class='w3-bar-item w3-button' href='/ChartPage.html'>"
      "<i class='fa fa-bar-chart'></i> Charts</a>"
      "<a class='w3-bar-item w3-button' href='/displayPHConfig'>"
      "<i class='fa fa-cog'></i> Configure Purple Haze</a>"
      "<a class='w3-bar-item w3-button' href='/presentScreenConfig'>"
      "<i class='fa fa-window-restore'></i> Configure Screens</a>");


    constexpr uint32_t BusyColor = 0xff88ff;
    void showBusyStatus(bool busy) {
      if (busy) phApp->busyIndicator->setColor(BusyColor);
      else phApp->busyIndicator->off();
      WebUIHelper::showBusyStatus(busy);
    }
  }
  // ----- END: PHWebUI::Internal


  // ----- BEGIN: PHWebUI::Mappers
  namespace Mappers {
    bool hasAQI(const String& key, String& val) {
      #if defined(HAS_AQI_SENSOR)
        if (key == "HAS_AQI") { val = "true"; return true; }
      #else
        if (key == "HAS_AQI") { val = "false"; return true; }
      #endif
      return false;
    }

    bool mapAQISettings(const String& key, String& val) {
      #if defined(HAS_AQI_SENSOR)
        char gr = key[2] - '0';  // GraphRange. Returns '\0' if no such index
        if      (key == "PM10_CLR")  val = phSettings->aqiSettings.chartColors.pm10;
        else if (key == "PM25_CLR")  val = phSettings->aqiSettings.chartColors.pm25;
        else if (key == "PM100_CLR") val = phSettings->aqiSettings.chartColors.pm100;
        else if (key == "AQI_CLR")   val = phSettings->aqiSettings.chartColors.aqi;
        else if (gr == phSettings->aqiSettings.graphRange && key.startsWith("AG")) val = "selected";
        else return false;
        return true;
      #else
        (void)key; (void)val; // Avoid compiler warning
        return false;
      #endif
    }

    bool mapAQIReadings(const String& key, String& val) {
      #if defined(HAS_AQI_SENSOR)
        const AQIReadings& aqiReadings = phApp->aqiMgr.getLastReadings();
        if (key == "AQI")      val.concat(phApp->aqiMgr.derivedAQI(aqiReadings.env.pm25));
        else if (key == "PM10STD")  val.concat(aqiReadings.standard.pm10);
        else if (key == "PM25STD")  val.concat(aqiReadings.standard.pm25);
        else if (key == "PM100STD") val.concat(aqiReadings.standard.pm100);
        else if (key == "PM10ENV")  val.concat(aqiReadings.env.pm10);
        else if (key == "PM25ENV")  val.concat(aqiReadings.env.pm25);
        else if (key == "PM100ENV") val.concat(aqiReadings.env.pm100);
        else if (key == "P03")      val.concat(aqiReadings.particles_03um);
        else if (key == "P05")      val.concat(aqiReadings.particles_05um);
        else if (key == "P10")      val.concat(aqiReadings.particles_10um);
        else if (key == "P25")      val.concat(aqiReadings.particles_25um);
        else if (key == "P50")      val.concat(aqiReadings.particles_50um);
        else if (key == "P100")     val.concat(aqiReadings.particles_100um);
        else if (key == "TMST")     val = Output::formattedTime(Basics::wallClockFromMillis(aqiReadings.timestamp));
        else if (key == "MA10")     val.concat(round(phApp->aqiMgr.pm25env_10min.getAverage()));
        else if (key == "MA30")     val.concat(round(phApp->aqiMgr.pm25env_30min.getAverage()));
        else if (key == "MA1H")     val.concat(round(phApp->aqiMgr.pm25env_1hr.getAverage()));
        else if (key == "MA6H")     val.concat(round(phApp->aqiMgr.pm25env_6hr.getAverage()));
        else return false;
        return true;
      #else
        (void)key; (void)val; // Avoid compiler warning
        return false;
      #endif
    }

    bool hasWeather(const String& key, String& val) {
      #if defined(HAS_WEATHER_SENSOR)
        if (key == "HAS_TEMP") { val = "true"; return true; }
      #else
        if (key == "HAS_TEMP") { val = "false"; return true; }
      #endif
      return false;
    }


    bool mapWeatherSettings(const String& key, String& val) {
      char gr = key[2] - '0';  // GraphRange. Returns '\0' if no such index
      #if defined(HAS_WEATHER_SENSOR)
        if (key == "TMP_CLR")   val = phSettings->weatherSettings.chartColors.temp;
        else if (key == "TEMP_CORRECT") val.concat(phSettings->weatherSettings.tempCorrection);
        else if (key == "HUMI_CORRECT") val.concat(phSettings->weatherSettings.humiCorrection);
        else if (key == "TEMP_CLR")     val = phSettings->weatherSettings.chartColors.temp;
        else if (key == "AVG_CLR")      val = phSettings->weatherSettings.chartColors.avg;
        else if (gr == phSettings->weatherSettings.graphRange && key.startsWith("WG")) val = "selected";
        else return false;
        return true;
      #else
        (void)key; (void)val; // Avoid compiler warning
        return false;
      #endif
    }

#define TEMP_TO_STRING(t, s) {                                    \
  if (isnan(wReadings.temp)) s = "N/A";                           \
  else s = String(Output::temp(t), 1) + Output::tempUnits();      \
}
#define HUMI_TO_STRING(h, s) {                                    \
  if (isnan(wReadings.humidity)) s = "N/A";                       \
  else s = String(wReadings.humidity, 1) + '%';                   \
}
#define BARO_TO_STRING(b, s) {                                    \
  if (isnan(wReadings.pressure)) s = "N/A";                       \
  else s = String(Output::baro(b), 1) + Output::baroUnits();      \
}

    bool mapWeatherReadings(const String& key, String& val) {
      #if defined(HAS_WEATHER_SENSOR)
        const WeatherReadings& wReadings = phApp->weatherMgr.getLastReadings();
        if (key == "TEMP")  { TEMP_TO_STRING(wReadings.temp, val); }
        else if (key == "HUMI") { HUMI_TO_STRING(wReadings.humidity, val); }
        else if (key == "BARO") { BARO_TO_STRING(wReadings.pressure, val); }
        else if (key == "RELP") { BARO_TO_STRING(wReadings.relPressure, val); }
        else if (key == "HTIN") { TEMP_TO_STRING(wReadings.heatIndex, val); }
        else if (key == "DWPT") { TEMP_TO_STRING(wReadings.dewPointTemp, val); }
        else if (key == "DPSP") { TEMP_TO_STRING(wReadings.dewPointSpread, val); }
        else if (key == "W_TMST")
          val = Output::formattedTime(Basics::wallClockFromMillis(wReadings.timestamp));
        else return false;
        return true;
      #else
        (void)key; (void)val; // Avoid compiler warning
        return false;
      #endif
    }

    bool mapPHSettings(const String& key, String& val) {
      if      (key == "DESC")       val = WebThing::encodeAttr(phSettings->description);
      else if (key == "BLYNK_KEY")  val = phSettings->blynkAPIKey;
      else if (key == "I_BRIGHT")   val.concat(phSettings->iBright);
      else if (key == "USE_METRIC") val = WebUI::checkedOrNot[phSettings->uiOptions.useMetric];
      else if (key == "USE_24HOUR") val = WebUI::checkedOrNot[phSettings->uiOptions.use24Hour];
      else if (key == "LAT")        val = WebThing::settings.latAsString();
      else if (key == "LNG")        val = WebThing::settings.lngAsString();
      else if (key == "GMAPS_KEY")  val = WebThing::settings.googleMapsKey;
      else if (key.equals(F("CITYID"))) {
        if (wtApp->settings->owmOptions.enabled) val = wtApp->settings->owmOptions.cityID;
        else val.concat("5380748");  // Palo Alto, CA, USA
      }
      else if (key.equals(F("WEATHER_KEY"))) val = wtApp->settings->owmOptions.key;
      else if (key.equals(F("UNITS"))) val = wtApp->settings->uiOptions.useMetric ? "metric" : "imperial";
      else if (key == "VLTG") {
        float voltage = WebThing::measureVoltage();
        if (voltage == -1) val = "N/A";
        else val = (String(voltage, 2) + "V");
      }
      else return false;
      return true;
    }
  }
  // ----- END: PHWebUI::Mappers


  // ----- BEGIN: PHWebUI::Endpoints
  namespace Pages {

    // Displays the home page which shows the last set of weather readings.
    //
    // Form:
    //    GET  /displayHomePage
    //
    void displayHomePage() {
      auto mapper =[](const String &key, String& val) -> void {
        if (Mappers::mapPHSettings(key, val)) return;
        if (Mappers::hasWeather(key, val)) return;
        if (Mappers::hasAQI(key, val)) return;
        if (Mappers::mapWeatherReadings(key, val)) return;
        if (Mappers::mapAQIReadings(key, val)) return;
      };

      WebUI::wrapWebPage("/", "/HomePage.html", mapper);
    }

    void displayChartPage() {
      auto mapper =[](const String &key, String& val) -> void {
        if (Mappers::hasAQI(key, val)) return;
        if (Mappers::hasWeather(key, val)) return;
        if (Mappers::mapAQISettings(key, val)) return;
        if (Mappers::mapWeatherSettings(key, val)) return;
        if (Mappers::mapPHSettings(key, val)) return;
      };

      WebUI::wrapWebPage("/displayChartPage", "/ChartPage.html", mapper);
    }

    // Displays a form allowing the user to update the PurpleHaze settings.
    //
    // Form:
    //    GET  /displayPHConfig
    //
    void displayPHConfig() {
      auto mapper =[&](const String &key, String& val) -> void {
        if (Mappers::hasAQI(key, val)) return;
        if (Mappers::hasWeather(key, val)) return;
        if (Mappers::mapAQISettings(key, val)) return;
        if (Mappers::mapWeatherSettings(key, val)) return;
        if (Mappers::mapPHSettings(key, val)) return;
      };

      WebUI::wrapWebPage("/displayPHConfig", "/ConfigForm.html", mapper);
    }
  }   // ----- END: PHWebUI::Pages


  namespace Endpoints {
#if defined(HAS_AQI_SENSOR)
    void getHistory() {
      auto action = []() {
        String rangeArg = WebUI::arg("range");
        AQIMgr::HistoryRange range;
        bool combined = false;

        if (rangeArg.equalsIgnoreCase("hour")) range = AQIMgr::HistoryRange::Range_1Hour;
        else if (rangeArg.equalsIgnoreCase("day")) range = AQIMgr::HistoryRange::Range_1Day;
        else if (rangeArg.equalsIgnoreCase("week")) range = AQIMgr::HistoryRange::Range_1Week;
        else combined = true;

        auto provider = [=](Stream& s) -> void {
          if (combined) phApp->aqiMgr.emitHistoryAsJson(s);
          else phApp->aqiMgr.emitHistoryAsJson(range, s);
        };

        WebUI::sendArbitraryContent("application/json", -1, provider);
      };

      WebUI::wrapWebAction("/getHistory", action, false);
    }
#else
    void getHistory() { WebUI::redirectHome(); }
#endif

#if defined(HAS_WEATHER_SENSOR)
    void getWeatherHistory() {
      auto action = []() {
        String rangeArg = WebUI::arg("range");
        WeatherMgr::HistoryRange range;
        bool combined = false;

        if (rangeArg.equalsIgnoreCase("hour")) range = WeatherMgr::HistoryRange::Range_1Hour;
        else if (rangeArg.equalsIgnoreCase("day")) range = WeatherMgr::HistoryRange::Range_1Day;
        else if (rangeArg.equalsIgnoreCase("week")) range = WeatherMgr::HistoryRange::Range_1Week;
        else combined = true;

        auto provider = [=](Stream& s) -> void {
          if (combined) phApp->weatherMgr.emitHistoryAsJson(s);
          else phApp->weatherMgr.emitHistoryAsJson(range, s);
        };

        WebUI::sendArbitraryContent("application/json", -1, provider);
      };

      WebUI::wrapWebAction("/getWeatherHistory", action, false);
    }
#else
    void getWeatherHistory() { WebUI::redirectHome(); }
#endif

#if defined(HAS_AQI_SENSOR)
    void getAQI() {
      auto action = []() {
        String result;
        result.reserve(300);
        const AQIReadings& aqiReadings = phApp->aqiMgr.getLastReadings();
        phApp->aqiMgr.aqiAsJSON(
          phApp->aqiMgr.derivedAQI(aqiReadings.env.pm25),
          aqiReadings.timestamp, result);
        WebUI::sendStringContent("application/json", result);
      };

      WebUI::wrapWebAction("/getHistory", action, false);
    }
#else
    void getAQI() { WebUI::redirectHome(); }
#endif

    // Handler for the "/updatePHConfig" endpoint. This is invoked as the target
    // of the form presented by "/displayPHConfig". It updates the values of the
    // corresponding settings and writes the settings to EEPROM.
    //
    // TO DO: Call a function to let the main app know that settings may have changed
    //        so that it can take any appropriate actions
    //
    // Form:
    //    GET /updatePHConfig?description=DESC&iBright=INT&...
    //
    void updatePHConfig() {
      auto action = []() {
        phSettings->description = WebUI::arg("description");
        phSettings->blynkAPIKey = WebUI::arg("blynkAPIKey");
        phSettings->iBright = (constrain(WebUI::arg("iBright").toInt(), 0, 100));
        phSettings->uiOptions.useMetric = WebUI::hasArg(F("metric"));
        phSettings->uiOptions.use24Hour = WebUI::hasArg(F("is24hour"));
#if defined(HAS_AQI_SENSOR)
        phSettings->aqiSettings.chartColors.pm10 = WebUI::arg("pm10Color");
        phSettings->aqiSettings.chartColors.pm25 = WebUI::arg("pm25Color");
        phSettings->aqiSettings.chartColors.pm100 = WebUI::arg("pm100Color");
        phSettings->aqiSettings.chartColors.aqi = WebUI::arg("aqiColor");
        phSettings->aqiSettings.graphRange = WebUI::arg("aqiGraphRange").toInt();
        phApp->appScreens.aqiGraphScreen->selectBuffer(phSettings->aqiSettings.graphRange);
#endif
#if defined(HAS_WEATHER_SENSOR)
        phSettings->weatherSettings.tempCorrection = WebUI::arg("tempCorrection").toFloat();
        phSettings->weatherSettings.humiCorrection = WebUI::arg("humiCorrection").toFloat();
        phSettings->weatherSettings.chartColors.temp = WebUI::arg("tempColor");
        phSettings->weatherSettings.chartColors.avg = WebUI::arg("avgColor");
        phSettings->weatherSettings.graphRange = WebUI::arg("weatherGraphRange").toInt();
        phApp->appScreens.weatherGraphScreen->selectBuffer(phSettings->weatherSettings.graphRange);
#endif
        phSettings->write();

        // The description MAY have changed. Update the title just in case
        phApp->setIndicatorBrightness(phSettings->iBright);
        WebUI::setTitle(phSettings->description+" ("+WebThing::settings.hostname+")");
        WebUI::redirectHome();
      };

      WebUI::wrapWebAction("/updatePHConfig", action, false);
    }
  }   // ----- END: PHWebUI::Endpoints

  void init() {
    WebUIHelper::init(Internal::APP_MENU_ITEMS);

#if defined(HAS_AQI_SENSOR)
    WebUI::Dev::addButton({"View AQI History", "getHistory", nullptr, nullptr});
#endif
#if defined(HAS_WEATHER_SENSOR)
      WebUI::Dev::addButton({"View Weather History", "getWeatherHistory", nullptr, nullptr});
#endif

    WebUI::registerBusyCallback(Internal::showBusyStatus);
      // We override the default since we want to update the indicator icon in
      // addition to showing the activity icon on the display.

    WebUI::registerHandler("/",                   Pages::displayHomePage);
    WebUI::registerHandler("/ChartPage.html",     Pages::displayChartPage);
    WebUI::registerHandler("/displayPHConfig",    Pages::displayPHConfig);

    WebUI::registerHandler("/updatePHConfig",     Endpoints::updatePHConfig);
    WebUI::registerHandler("/getHistory",         Endpoints::getHistory);
    WebUI::registerHandler("/getWeatherHistory",  Endpoints::getWeatherHistory);
    WebUI::registerHandler("/getAQI",             Endpoints::getAQI);
  }


}
// ----- END: PHWebUI
