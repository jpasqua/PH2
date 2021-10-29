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
      "<i class='fa fa-cog'></i> Configure PH</a>"
      "<a class='w3-bar-item w3-button' href='/presentScreenConfig'>"
      "<i class='fa window-restore'></i> Configure Screens</a>");


    constexpr uint32_t BusyColor = 0xff88ff;
    void showBusyStatus(bool busy) {
      if (busy) phApp->busyIndicator->setColor(BusyColor);
      else phApp->busyIndicator->off();
      WebUIHelper::showBusyStatus(busy);
    }
  }
  // ----- END: PHWebUI::Internal


  // ----- BEGIN: PHWebUI::Endpoints
  namespace Pages {

    // Displays the home page which shows the last set of weather readings.
    //
    // Form:
    //    GET  /displayHomePage
    //
    void displayHomePage() {
      auto mapper =[](const String &key, String& val) -> void {
        const AQIReadings& aqiReadings = phApp->aqiMgr.getLastReadings();
        if      (key == "LAT")      val = WebThing::settings.latAsString();
        else if (key == "LNG")      val = WebThing::settings.lngAsString();
        else if (key == "AQI")      val.concat(phApp->aqiMgr.derivedAQI(aqiReadings.env.pm25));
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
      };

      WebUI::wrapWebPage("/", "/HomePage.html", mapper);
    }

    void displayChartPage() {
      auto mapper =[](const String &key, String& val) -> void {
        if      (key == "PM10_CLR")  val = phSettings->chartColors.pm10;
        else if (key == "PM25_CLR")  val = phSettings->chartColors.pm25;
        else if (key == "PM100_CLR") val = phSettings->chartColors.pm100;
        else if (key == "AQI_CLR")   val = phSettings->chartColors.aqi;
        else if (key == "TMP_CLR")   val = phSettings->weatherSettings.chartColors.temp;
        else if (key == "USE_METRIC")val = phSettings->uiOptions.useMetric;
      };

      WebUI::wrapWebPage("/displayChartPage", "/ChartPage.html", mapper);
    }

    // Displays a form allowing the user to update the PurpleHaze settings.
    //
    // Form:
    //    GET  /displayPHConfig
    //
    void displayPHConfig() {
      auto mapper =[](const String &key, String& val) -> void {
        if      (key == "DESC")       val = WebThing::encodeAttr(phSettings->description);
        else if (key == "BLYNK_KEY")  val = phSettings->blynkAPIKey;
        else if (key == "I_BRIGHT")   val.concat(phSettings->iBright);
        else if (key == "PM10_CLR")   val = phSettings->chartColors.pm10;
        else if (key == "PM25_CLR")   val = phSettings->chartColors.pm25;
        else if (key == "PM100_CLR")  val = phSettings->chartColors.pm100;
        else if (key == "AQI_CLR")    val = phSettings->chartColors.aqi;
        else if (key == "USE_METRIC") val = WebUI::checkedOrNot[phSettings->uiOptions.useMetric];
        else if (key == "USE_24HOUR") val = WebUI::checkedOrNot[phSettings->uiOptions.use24Hour];
#if defined(HAS_WEATHER_SENSOR)
        else if (key == "WTHR_VIS")   val = "true";
        else if (key == "TEMP_CORRECT") val.concat(phSettings->weatherSettings.tempCorrection);
        else if (key == "HUMI_CORRECT") val.concat(phSettings->weatherSettings.humiCorrection);
        else if (key == "TEMP_CLR")   val = phSettings->weatherSettings.chartColors.temp;
        else if (key == "AVG_CLR")    val = phSettings->weatherSettings.chartColors.avg;
#else
        else if (key == "WTHR_VIS")   val = "false";
#endif
      };

      WebUI::wrapWebPage("/displayPHConfig", "/ConfigForm.html", mapper);
    }
  }   // ----- END: PHWebUI::Pages


  namespace Endpoints {
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
    void getWeatherHistory() {
      WebUI::redirectHome();
    }
#endif

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
        phSettings->chartColors.pm10 = WebUI::arg("pm10Color");
        phSettings->chartColors.pm25 = WebUI::arg("pm25Color");
        phSettings->chartColors.pm100 = WebUI::arg("pm100Color");
        phSettings->chartColors.aqi = WebUI::arg("aqiColor");
        phSettings->weatherSettings.chartColors.temp = WebUI::arg("tempColor");
        phSettings->weatherSettings.chartColors.avg = WebUI::arg("avgColor");
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

    WebUI::Dev::addButton({"View AQI History", "getHistory", nullptr, nullptr});
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
