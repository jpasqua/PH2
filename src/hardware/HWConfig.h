#ifndef HWConfig_h
#define HWConfig_h

//
// A Hardware configuration is a bundle of definitions that describe a
// particular collection of hardware such as what kind of display is attached,
// which pins are being used as buttons, which pins to use for standard
// functions (e.g. SCL and SDA). Most of the definitions are runtime constants,
// but some may be compile-time constants (#define's)
//
// Update Section 3 of this file to specify the HW Configuration you are using.
// You do so by choosing the base configuration and options (e.g. is there a
// weather sensor attached).
//
// The pre-defined configurations are defined in Configs.h. If you need a new
// configuration, define it there.
//
// This file is specific to hardware configurations which contain a
// DEVICE_TYPE_OLED display device. It can be used as a basis for
// HWConfig files with other types of display, or no display
//

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <BPABasics.h>
#include "SensorConfig.h"
#include <gui/devices/DeviceTypes.h>


// SECTION 0: [BOILERPLATE] Generic definitions
#define UNUSED_PIN 255

#define PRESENT     1
#define MOCK        2


// SECTION 1: [BOILERPLATE] Select the general class of Display Device we're using.
// The available types are defined in DeviceTypes.h. This config provides
// options based on DEVICE_TYPE_OLED, including NONE.
#define DEVICE_TYPE DEVICE_TYPE_OLED
#include <gui/Display.h>


// SECTION 2: [BOILERPLATE] Define a list of the predefined hardware configs
// that we can choose from. If you add a new configuration, list it here.
#define Config_PH2_Board        1
#define Config_D1Mini_Enc       2
#define Config_ESP8266WithOLED  3
#define Config_PH1_Board        4
#define Config_ESP32Mini        5
#define Config_D1Mini_JAWS      6


//------------------------------------------------------------------------------
// --------------------- SECTION 3: CUSTOMIZE THIS SECTION ONLY ----------------
//------------------------------------------------------------------------------
// Choose a base configuration and the specific devices
// that are present on your particular board. For the BME and AQI sensors you
// may choose PRESENT, MOCK, or leave it undefined.

#define BaseConfig  Config_D1Mini_Enc
#define GUI_DSPLY   PRESENT
#define BME_SENSOR  PRESENT

// #define BaseConfig  Config_D1Mini_JAWS
// #define BME_SENSOR  PRESENT


// SECTION 4: [BOLERPLATE] The definitions of the available configurations and dependent definitions
#include "Configs.h"

#if defined(BME280_READINGS) || defined (DHT22_READINGS) || defined(DS18B20_READINGS)
  #define HAS_WEATHER_SENSOR
#endif

#if defined(PMS5003_READINGS)
  #define HAS_AQI_SENSOR
#endif


#if !defined(HAS_WEATHER_SENSOR) && !defined(HAS_AQI_SENSOR)
  #error("No sensors defined")
#endif


// SECTION 5: Declare and Initialize the HWConfig object

class HWConfig {
public:
  struct CorePins {
    uint8_t scl;
    uint8_t sda;
  };

  const CorePins corePins;
  const DisplayDeviceOptions displayDeviceOptions;
  const uint8_t* physicalButtons;
  const uint8_t  nPhysicalButtons;
  const uint8_t* syntheticGrounds;
  const uint8_t  nSyntheticGrounds;

  const uint8_t advanceButton;
  const uint8_t previousButton;
};

constexpr HWConfig hwConfig {
  { SCL_PIN, SDA_PIN },
  { DISPLAY_DRIVER, SCL_PIN, SDA_PIN, DISPLAY_I2C_ADDRESS },
  physicalButtons, countof(physicalButtons),  
  syntheticGrounds, countof(syntheticGrounds),
  physicalButtons[0], physicalButtons[1]
};

#endif  // HWConfig_h
