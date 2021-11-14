#ifndef HWConfig_h
#define HWConfig_h

//
// A Hardware configuration is a bundle of definitions that describe a
// particular collection of hardware such as what kind of display is attached,
// which pins are being used as buttons, which pins to use for standard
// functions (e.g. SCL and SDA). Most of the definitions are runtime constants,
// but some may be compile-time constants (#define's)
//
// This file contains a number of predefined configurations. You can:
// a) choose from one of the predefined configurations
// b) add a new configuration if you think it is common
// c) modify the Custom config to match your unique configuration
//    your hardware. 
//
// This file is specific to hard3are configurations which contain a
// DEVICE_TYPE_OLED display device. It can be used as a basis for
// HWConfig files with other types of display, or no display
//

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <BPABasics.h>
#include "SensorConfig.h"
#include <gui/devices/DeviceTypes.h>


// SECTION 0: Generic definitions
#define UNUSED_PIN 255

// SECTION 1: Select the general class of Display Device we're using.
// The available types are defined in DeviceTypes.h
// This config provides options based on DEVICE_TYPE_OLED, including NONE.
#define DEVICE_TYPE DEVICE_TYPE_OLED

// SECTION 2: [BOILERPLATE] Include Display.h to get the list of specific device
// types that are available
#include <gui/Display.h>

// SECTION 3: [BOILERPLATE] Define a list of the predefined hardware configs
// that we can choose from. If you add a new configuration, list it here.
#define Config_Custom         1
#define Config_EmbeddedOLED   2
#define Config_D1Mini         3
#define Config_ESP32Mini      4
#define Config_ESP32WithOLED  5
#define Config_D1Mini_MOCK    6
#define Config_D1Mini_MOCK_ND 7
#define Config_PH2_BOARD      8

// SECTION 4: [CUSTOMIZE] Choose a specific configuration
#define SelectedConfig Config_PH2_BOARD

// SECTION 5: The definitions of the available configurations
// Add new configs below if you add an option
#if (SelectedConfig == Config_D1Mini)
  /*------------------------------------------------------------------------------
   *
   * Config Info for D1Mini with 1.3" SH1106 display
   *
   *----------------------------------------------------------------------------*/

  // ----- I2C Settings
  constexpr uint8_t SDA_PIN = D3;
  constexpr uint8_t SCL_PIN = D5;

  // ----- Display Type
  constexpr auto DISPLAY_DRIVER = DisplayDeviceOptions::DeviceType::SH1106;
  constexpr uint8_t DISPLAY_I2C_ADDRESS = 0x3c;

  // ----- Buttons
  constexpr uint8_t physicalButtons[] = { D4, D7 };
  constexpr uint8_t syntheticGrounds[] = { UNUSED_PIN };

  // ----- Air Quality Sensor
  #define USE_SW_SERIAL 1
  constexpr uint8_t SENSOR_RX_PIN = D6;
  constexpr uint8_t SENSOR_TX_PIN = D8;

  // ----- Indicators
  constexpr uint8_t NEOPIXEL_PIN = D2;
  constexpr neoPixelType NEOPIXEL_TYPE = NEO_GRB + NEO_KHZ800;

#elif (SelectedConfig == Config_D1Mini_MOCK)
  /*------------------------------------------------------------------------------
   *
   * Config Info for D1Mini with 1.3" SH1106 display, but with no AQ sensor
   *
   *----------------------------------------------------------------------------*/

  // ----- I2C Settings
  constexpr uint8_t SDA_PIN = D2;
  constexpr uint8_t SCL_PIN = D5;

  // ----- Display Type
  constexpr auto DISPLAY_DRIVER = DisplayDeviceOptions::DeviceType::SH1106;
  constexpr uint8_t DISPLAY_I2C_ADDRESS = 0x3c;

  // ----- Buttons
  constexpr uint8_t physicalButtons[] = { D6 };
  constexpr uint8_t syntheticGrounds[] = { D8 };

  // ----- Air Quality Sensor
  #define USE_SW_SERIAL 1
  constexpr uint8_t SENSOR_RX_PIN = UNUSED_PIN;
  constexpr uint8_t SENSOR_TX_PIN = UNUSED_PIN;

  // ----- Indicators
  constexpr uint8_t NEOPIXEL_PIN = UNUSED_PIN;

#elif (SelectedConfig == Config_D1Mini_MOCK_ND)
  /*------------------------------------------------------------------------------
   *
   * Config Info for D1Mini with no display and no AQ sensor
   *
   *----------------------------------------------------------------------------*/

  // ----- I2C Settings
  constexpr uint8_t SDA_PIN = D2;
  constexpr uint8_t SCL_PIN = D5;

  // ----- Display Type
  constexpr auto DISPLAY_DRIVER = DisplayDeviceOptions::DeviceType::NONE;
  constexpr uint8_t DISPLAY_I2C_ADDRESS = 0x3c;

  // ----- Buttons
  constexpr uint8_t physicalButtons[] = { D6 };
  constexpr uint8_t syntheticGrounds[] = { D8 };

  // ----- Air Quality Sensor
  #define USE_SW_SERIAL 1
  constexpr uint8_t SENSOR_RX_PIN = UNUSED_PIN;
  constexpr uint8_t SENSOR_TX_PIN = UNUSED_PIN;

  // ----- Indicators
  constexpr uint8_t NEOPIXEL_PIN = UNUSED_PIN;
  constexpr neoPixelType NEOPIXEL_TYPE = NEO_GRB + NEO_KHZ800;

#elif (SelectedConfig == Config_ESP32Mini)
  /*------------------------------------------------------------------------------
   *
   * Config Info for ESP32D1Mini with 1.3" SH1106 display
   *
   *----------------------------------------------------------------------------*/

  // ----- I2C Settings
  constexpr uint8_t SDA_PIN = 21;
  constexpr uint8_t SCL_PIN = 22;

  // ----- Display Type
  constexpr auto DISPLAY_DRIVER = DisplayDeviceOptions::DeviceType::SH1106;
  constexpr uint8_t DISPLAY_I2C_ADDRESS = 0x3c;

  // ----- Buttons
  constexpr uint8_t physicalButtons[] = { 13 };
  constexpr uint8_t syntheticGrounds[] = { UNUSED_PIN };

  // ----- Air Quality Sensor
  constexpr uint8_t SENSOR_RX_PIN = 16;
  constexpr uint8_t SENSOR_TX_PIN = 17;

  // ----- Indicators
  constexpr uint8_t NEOPIXEL_PIN = 21;
  constexpr neoPixelType NEOPIXEL_TYPE = NEO_GRB + NEO_KHZ800;

#elif (SelectedConfig == Config_ESP32WithOLED)
  /*------------------------------------------------------------------------------
   *
   * Config Info for ESP32 with embedded 0.96" OLED
   *
   *----------------------------------------------------------------------------*/

  // ----- I2C Settings
  constexpr uint8_t SDA_PIN = 5;
  constexpr uint8_t SCL_PIN = 4;

  // ----- Display Type
  constexpr auto DISPLAY_DRIVER = DisplayDeviceOptions::DeviceType::SSD1306;
  constexpr uint8_t DISPLAY_I2C_ADDRESS = 0x3c;
  
  // ----- Buttons
  constexpr uint8_t physicalButtons[] = { 13 };
  constexpr uint8_t syntheticGrounds[] = { UNUSED_PIN };

  // ----- Air Quality Sensor
  constexpr uint8_t SENSOR_RX_PIN = 16;
  constexpr uint8_t SENSOR_TX_PIN = 17;

  // ----- Indicators
  constexpr uint8_t NEOPIXEL_PIN = 21;
  constexpr neoPixelType NEOPIXEL_TYPE = NEO_GRB + NEO_KHZ800;

#elif (SelectedConfig == Config_EmbeddedOLED)
  /*------------------------------------------------------------------------------
   *
   * Config Info for Wemos board with embedded 0.96" OLED
   *
   *----------------------------------------------------------------------------*/

  // ----- I2C Settings
  constexpr uint8_t SDA_PIN = D1;
  constexpr uint8_t SCL_PIN = D2;

  // ----- Display Type
  constexpr auto DISPLAY_DRIVER = DisplayDeviceOptions::DeviceType::SSD1306;
  constexpr uint8_t DISPLAY_I2C_ADDRESS = 0x3c;

  // ----- Buttons
  constexpr uint8_t physicalButtons[] = { D3 };
  constexpr uint8_t syntheticGrounds[] = { UNUSED_PIN };

  // ----- Air Quality Sensor
  #define USE_SW_SERIAL 1
  constexpr uint8_t SENSOR_RX_PIN = D6;
  constexpr uint8_t SENSOR_TX_PIN = D8;

  // ----- Indicators
  constexpr uint8_t NEOPIXEL_PIN = D2;
  constexpr neoPixelType NEOPIXEL_TYPE = NEO_GRB + NEO_KHZ800;

#elif (SelectedConfig == Config_PH2_BOARD)
  /*------------------------------------------------------------------------------
   *
   * Config Info for Purple Haze v2 Board with D1 Mini
   * (No AQI at the moment)
   *
   *----------------------------------------------------------------------------*/

  // ----- I2C Settings
  constexpr uint8_t SDA_PIN = D3;
  constexpr uint8_t SCL_PIN = D5;

  // ----- Display Type
  // constexpr auto DISPLAY_DRIVER = DisplayDeviceOptions::DeviceType::NONE;
  constexpr auto DISPLAY_DRIVER = DisplayDeviceOptions::DeviceType::SH1106;
  constexpr uint8_t DISPLAY_I2C_ADDRESS = 0x3c;

  // ----- Buttons
  constexpr uint8_t physicalButtons[] = { D7, D4 };
  constexpr uint8_t syntheticGrounds[] = { UNUSED_PIN };

  // ----- Air Quality Sensor
  #define USE_SW_SERIAL 1
  constexpr uint8_t SENSOR_RX_PIN = UNUSED_PIN; // D6
  constexpr uint8_t SENSOR_TX_PIN = UNUSED_PIN; // D8

  // ----- Indicators
  constexpr uint8_t NEOPIXEL_PIN = D2;
  constexpr neoPixelType NEOPIXEL_TYPE = NEO_RGB + NEO_KHZ800;


#elif (SelectedConfig ==  Config_Custom)
  /*------------------------------------------------------------------------------
   *
   * Custom Config - put your settings here
   *
   *----------------------------------------------------------------------------*/
  // ----- I2C Settings
  constexpr uint8_t SDA_PIN = D2;  // Substitute the correct Pin for your config
  constexpr uint8_t SCL_PIN = D5;  // Substitute the correct Pin for your config

    // ----- Display Type
    // #define DISPLAY_DRIVER   SSD1306
  constexpr auto DISPLAY_DRIVER = DisplayDeviceOptions::DeviceType::SH1106;
  //constexpr auto DISPLAY_DRIVER = DisplayDeviceOptions::DeviceType::SSD1306;
  constexpr uint8_t DISPLAY_I2C_ADDRESS = 0x3c;

  // ----- Buttons
  constexpr int8_t physicalButtons[] = { D3 };
  constexpr int8_t syntheticGrounds[] = { UNUSED_PIN };

  #define USE_SW_SERIAL 1
  constexpr uint8_t SENSOR_RX_PIN = D6;
  constexpr uint8_t SENSOR_TX_PIN = D8;

  // ----- Indicators
  // If there are NeoPixels connected to use as status indicators, set the pin
  // that drives the data line. If no NeoPixels are connected, set the pin to -1
  constexpr uint8_t NEOPIXEL_PIN = D2;
  constexpr neoPixelType NEOPIXEL_TYPE = NEO_GRB + NEO_KHZ800;
#else
    #error "Please set SelectedConfig"
#endif


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
