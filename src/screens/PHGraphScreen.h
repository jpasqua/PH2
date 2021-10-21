#include <gui/devices/DeviceSelect.h>
#if DEVICE_TYPE == DEVICE_TYPE_OLED

#ifndef PHGraphScreen_h
#define PHGraphScreen_h

//--------------- Begin:  Includes ---------------------------------------------
//                                  Core Libraries
#include <Arduino.h>
//                                  Third Party Libraries
//                                  Local Includes
#include <gui/Display.h>
#include <screens/oled/GraphScreen.h>
//--------------- End:    Includes ---------------------------------------------


template <typename MgrType, typename RangeType>
class PHDataProvider : public GraphDataProvider {
public:
  PHDataProvider() = default;

  virtual uint16_t count() const {
    return _mgr.sizeOfRange(_range);
  };

  virtual void getXLabels(String& xMin, String& xMax) const {
    time_t start, end;
    _mgr.getTimeRange(_range, start, end);
    int32_t tzOffset = WebThing::getGMTOffset();

    WebThing::TimeFormatOptions tfo {true, true, false, false};
    xMin = WebThing::formattedTime(start+tzOffset, tfo);
    xMax = WebThing::formattedTime(end+tzOffset, tfo);
  }

  virtual String getXLegend() {
    switch (_range) {
      case RangeType::Range_1Hour: return String("Hour");
      case RangeType::Range_1Day: return String("Day");
      case RangeType::Range_1Week:
      default: return String("Week");
    }
  }

  void setRange(RangeType r) { _range = r; }

protected:
  RangeType _range;
  MgrType _mgr;
};

class AQIDataProvider : public PHDataProvider<AQIMgr, AQIMgr::HistoryRange> {
  virtual String getYLegend() { return "AQI"; }
  virtual float getVal(uint16_t index) const {
    return _mgr.aqiFromHistory(_range, index);
  }
};

class WeatherDataProvider : public PHDataProvider<WeatherMgr, WeatherMgr::HistoryRange> {
  virtual String getYLegend() { return "Temp"; }
  virtual float getVal(uint16_t index) const {
    return _mgr.tempFromHistory(_range, index);
  }
};

class AQIGraphScreen : public GraphScreen {
public:
  AQIGraphScreen();

private:
  AQIDataProvider provider;
};

class WeatherGraphScreen : public GraphScreen {
public:
  WeatherGraphScreen();

private:
  WeatherDataProvider provider;
};


#endif  // PHGraphScreen_h
#endif