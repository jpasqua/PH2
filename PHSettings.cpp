/*
 * PHSettings
 *    Handle reading and writing settings information to the file system
 *    in JSON format.
 *
 * NOTES:
 *
 */

//--------------- Begin:  Includes ---------------------------------------------
//                                  Core Libraries
#include <Arduino.h>
#include <FS.h>
//                                  Third Party Libraries
#include <ArduinoLog.h>
#include <ArduinoJson.h>
//                                  Local Includes
#include "PHSettings.h"
//--------------- End:    Includes ---------------------------------------------


void WeatherSettings::fromJSON(const JsonDocument &doc) {
  tempCorrection = doc["wthr"]["tempCorrection"];
  humiCorrection = doc["wthr"]["humiCorrection"];
  chartColors.temp = String(doc["wthr"]["chartColors"]["temp"]|"#4e7a27");
  chartColors.avg = String(doc["wthr"]["chartColors"]["avg"]|"#ff00ff");
  graphRange = doc["wthr"]["graphRange"];
  graphRange = (graphRange > 2) ? 2 : graphRange;
}

void WeatherSettings::toJSON(JsonDocument &doc) {
  doc["wthr"]["tempCorrection"] = tempCorrection;
  doc["wthr"]["humiCorrection"] = humiCorrection;
  doc["wthr"]["chartColors"]["temp"] = chartColors.temp;
  doc["wthr"]["chartColors"]["avg"] = chartColors.avg;
  doc["wthr"]["graphRange"] = graphRange;
}

void WeatherSettings::logSettings() {
  Log.verbose(F("Weather Settings"));
  Log.verbose(F("  tempCorrection = %F"), tempCorrection);
  Log.verbose(F("  humiCorrection = %F"), humiCorrection);
  Log.verbose(F("  chartColors.temp = %s"), chartColors.temp.c_str());
  Log.verbose(F("  chartColors.avg = %s"), chartColors.avg.c_str());
  Log.verbose(F("  Graph Range = %d"), graphRange);
}

void AQISettings::fromJSON(const JsonDocument &doc) {
  chartColors.pm10 = String(doc["aqi"]["chartColors"]["pm10"]|"#e32400");
  chartColors.pm25 = String(doc["aqi"]["chartColors"]["pm25"]|"#4e7a27");
  chartColors.pm100 = String(doc["aqi"]["chartColors"]["pm100"]|"#0042aa");
  chartColors.aqi = String(doc["aqi"]["chartColors"]["aqi"]|"#f00f88");
  graphRange = doc["aqi"]["graphRange"];
  graphRange = (graphRange > 2) ? 2 : graphRange;
}

void AQISettings::toJSON(JsonDocument &doc) {
  doc["aqi"]["chartColors"]["pm10"] = chartColors.pm10;
  doc["aqi"]["chartColors"]["pm25"] = chartColors.pm25;
  doc["aqi"]["chartColors"]["pm100"] = chartColors.pm100;
  doc["aqi"]["chartColors"]["aqi"] = chartColors.aqi;
  doc["aqi"]["graphRange"] = graphRange;
}

void AQISettings::logSettings() {
  Log.verbose(F("AQI Settings"));
  Log.verbose(F("  chartColors.pm10 = %s"), chartColors.pm10.c_str());
  Log.verbose(F("  chartColors.pm25 = %s"), chartColors.pm25.c_str());
  Log.verbose(F("  chartColors.pm100 = %s"), chartColors.pm100.c_str());
  Log.verbose(F("  chartColors.aqi = %s"), chartColors.aqi.c_str());
  Log.verbose(F("  Graph Range = %d"), graphRange);
}



PHSettings::PHSettings() {
  version = PHSettings::CurrentVersion;
  maxFileSize = 2048;
}

void PHSettings::fromJSON(const JsonDocument &doc) {
  description = doc["description"].as<String>();
  blynkAPIKey = String(doc["blynkAPIKey"]|"");
  iBright = doc[F("iBright")];

  aqiSettings.fromJSON(doc);
  weatherSettings.fromJSON(doc);
  WTAppSettings::fromJSON(doc);
}

void PHSettings::toJSON(JsonDocument &doc) {
  doc["description"] = description;
  doc["blynkAPIKey"] = blynkAPIKey;
  doc[F("iBright")] = iBright;

  aqiSettings.toJSON(doc);
  weatherSettings.toJSON(doc);
  WTAppSettings::toJSON(doc);
}

void PHSettings::logSettings() {
  Log.verbose(F("PurpleHaze Settings"));
  Log.verbose(F("  description = %s"), description.c_str());
  Log.verbose(F("  blynkAPIKey = %s"), blynkAPIKey.c_str());
  Log.verbose(F("  indicator brightness: %d"), iBright);
  aqiSettings.logSettings();
  weatherSettings.logSettings();
  WTAppSettings::logSettings();
}

