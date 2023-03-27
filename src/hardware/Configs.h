//
// A Hardware configuration is a bundle of definitions that describe a
// particular collection of hardware; e.g., what kind of display is attached,
// which pins are being used as buttons, which pins to use for standard
// functions (e.g. SCL and SDA). Most of the definitions are runtime constants,
// but some may be compile-time constants (#define's)
//
//

#ifndef Configs_h
#define Configs_h

#if (BaseConfig == Config_PH2_Board)
  /*------------------------------------------------------------------------------
   *
   * Config Info for Purple Haze v2 Board with D1 Mini. Of course, a system of
   * discrete parts wired the same way can also use this configuration
   *
   *----------------------------------------------------------------------------*/

  // ----- I2C Settings
  constexpr uint8_t SDA_PIN = D3;
  constexpr uint8_t SCL_PIN = D5;

  // ----- Display Type
  #if defined(GUI_DSPLY)
    constexpr auto DISPLAY_DRIVER = DisplayDeviceOptions::DeviceType::SH1106;
    constexpr uint8_t DISPLAY_I2C_ADDRESS = 0x3c;
  #else
    constexpr auto DISPLAY_DRIVER = DisplayDeviceOptions::DeviceType::NONE;
    constexpr uint8_t DISPLAY_I2C_ADDRESS = 0x00;
  #endif

  // ----- Buttons
  constexpr uint8_t physicalButtons[] = { D4, D7 };
  constexpr uint8_t syntheticGrounds[] = { UNUSED_PIN };

  // ----- Air Quality Sensor
  #define USE_SW_SERIAL 1
  #if (AQI_SENSOR == PRESENT)
    #define PMS5003_READINGS PMS5003_AVAIL_READINGS
    constexpr uint8_t SENSOR_RX_PIN = D6;
    constexpr uint8_t SENSOR_TX_PIN = D8;
  #elif (AQI_SENSOR == MOCK)
    #define PMS5003_READINGS PMS5003_AVAIL_READINGS
    constexpr uint8_t SENSOR_RX_PIN = UNUSED_PIN;
    constexpr uint8_t SENSOR_TX_PIN = UNUSED_PIN;
  #else
    constexpr uint8_t SENSOR_RX_PIN = UNUSED_PIN;
    constexpr uint8_t SENSOR_TX_PIN = UNUSED_PIN;
  #endif

  #if (BME_SENSOR == PRESENT)
    #define BME280_READINGS (BME280_AVAIL_READINGS)
    constexpr uint8_t BME_I2C_ADDR = 0x76;
  #elif (BME_SENSOR == MOCK)
    #define BME280_READINGS (BME280_AVAIL_READINGS)
    constexpr uint8_t BME_I2C_ADDR = 0x00;
  #else
    constexpr uint8_t BME_I2C_ADDR = 0x00;
  #endif

  // ----- Indicators
  constexpr uint8_t NEOPIXEL_PIN = D2;
  constexpr neoPixelType NEOPIXEL_TYPE = NEO_RGB + NEO_KHZ800;


#elif (BaseConfig == Config_PH1_Board)
  /*------------------------------------------------------------------------------
   *
   * Config Info for Purple Haze v1 Board with D1 Mini. This board does not allow
   * for a display or temperature sensor so these are hard-coded off.
   * Of course, a system of discrete parts wired the same way can also use
   * this configuration
   *
   *----------------------------------------------------------------------------*/

  // ----- I2C Settings
  constexpr uint8_t SDA_PIN = D3;
  constexpr uint8_t SCL_PIN = D5;

  // ----- Display Type
  #if defined(GUI_DSPLY)
    #error("No display may be attached in this configuration")
  #else
    constexpr auto DISPLAY_DRIVER = DisplayDeviceOptions::DeviceType::NONE;
    constexpr uint8_t DISPLAY_I2C_ADDRESS = 0x00;
  #endif

  // ----- Buttons
  constexpr uint8_t physicalButtons[] = { UNUSED_PIN, UNUSED_PIN };
  constexpr uint8_t syntheticGrounds[] = { UNUSED_PIN };

  // ----- Air Quality Sensor
  #define USE_SW_SERIAL 1
  #if (AQI_SENSOR == PRESENT)
    #define PMS5003_READINGS PMS5003_AVAIL_READINGS
    constexpr uint8_t SENSOR_RX_PIN = D6;
    constexpr uint8_t SENSOR_TX_PIN = D8;
  #elif (AQI_SENSOR == MOCK)
    #define PMS5003_READINGS PMS5003_AVAIL_READINGS
    constexpr uint8_t SENSOR_RX_PIN = UNUSED_PIN;
    constexpr uint8_t SENSOR_TX_PIN = UNUSED_PIN;
  #else
    constexpr uint8_t SENSOR_RX_PIN = UNUSED_PIN;
    constexpr uint8_t SENSOR_TX_PIN = UNUSED_PIN;
  #endif

  #if (BME_SENSOR == MOCK)
    #define BME280_READINGS (BME280_AVAIL_READINGS)
    constexpr uint8_t BME_I2C_ADDR = 0x00;
  #else
    constexpr uint8_t BME_I2C_ADDR = 0x00;
  #endif

  // ----- Indicators
  constexpr uint8_t NEOPIXEL_PIN = D2;
  constexpr neoPixelType NEOPIXEL_TYPE = NEO_RGB + NEO_KHZ800;


#elif (BaseConfig == Config_D1Mini_Enc)
  /*------------------------------------------------------------------------------
   *
   * Config Info for D1Mini with 1.3" SH1106 display in a small enclosure which
   * limits what may be connected and how. This configuration does not support
   * a physical AQI sensor (though you can mock one).
   *
   *----------------------------------------------------------------------------*/

  // ----- I2C Settings
  constexpr uint8_t SDA_PIN = D2;
  constexpr uint8_t SCL_PIN = D5;

  // ----- Display Type
  #if defined(GUI_DSPLY)
    constexpr auto DISPLAY_DRIVER = DisplayDeviceOptions::DeviceType::SH1106;
    constexpr uint8_t DISPLAY_I2C_ADDRESS = 0x3c;
  #else
    constexpr auto DISPLAY_DRIVER = DisplayDeviceOptions::DeviceType::NONE;
    constexpr uint8_t DISPLAY_I2C_ADDRESS = 0x00;
  #endif

  // ----- Buttons
  constexpr uint8_t physicalButtons[] = { D6, UNUSED_PIN };
  constexpr uint8_t syntheticGrounds[] = { D8 };

  // ----- Air Quality Sensor
  #define USE_SW_SERIAL 1
  constexpr uint8_t SENSOR_RX_PIN = UNUSED_PIN;
  constexpr uint8_t SENSOR_TX_PIN = UNUSED_PIN;
  #if (AQI_SENSOR == PRESENT)
    #error("An AQI Sensor may not be used in this configuration")
  #elif (AQI_SENSOR == MOCK)
    #define PMS5003_READINGS PMS5003_AVAIL_READINGS
  #endif

  #if (BME_SENSOR == PRESENT)
    #define BME280_READINGS (BME280_AVAIL_READINGS)
    constexpr uint8_t BME_I2C_ADDR = 0x76;
  #elif (BME_SENSOR == MOCK)
    #define BME280_READINGS (BME280_AVAIL_READINGS)
    constexpr uint8_t BME_I2C_ADDR = 0x00;
  #endif

  #if (DHT22_SENSOR == PRESENT)
    #define DHT22_READINGS (DHT22_AVAIL_READINGS)
    constexpr uint8_t DHT22_PIN = D5;
  #endif

  // ----- Indicators
  constexpr uint8_t NEOPIXEL_PIN = UNUSED_PIN;
  constexpr neoPixelType NEOPIXEL_TYPE = 0;     // Not Used

#elif (BaseConfig == Config_D1Mini_JAWS)
  /*------------------------------------------------------------------------------
   *
   * Config Info for D1Mini with no display
   *
   *----------------------------------------------------------------------------*/

  // ----- I2C Settings
  constexpr uint8_t SDA_PIN = D2;
  constexpr uint8_t SCL_PIN = D1;

  // ----- Display Type
  #if defined(GUI_DSPLY)
    constexpr auto DISPLAY_DRIVER = DisplayDeviceOptions::DeviceType::SH1106;
    constexpr uint8_t DISPLAY_I2C_ADDRESS = 0x3c;
  #else
    constexpr auto DISPLAY_DRIVER = DisplayDeviceOptions::DeviceType::NONE;
    constexpr uint8_t DISPLAY_I2C_ADDRESS = 0x00;
  #endif

  // ----- Buttons
  constexpr uint8_t physicalButtons[] = { UNUSED_PIN, UNUSED_PIN };
  constexpr uint8_t syntheticGrounds[] = { UNUSED_PIN };

  // ----- Air Quality Sensor
  #define USE_SW_SERIAL 1
  constexpr uint8_t SENSOR_RX_PIN = UNUSED_PIN;
  constexpr uint8_t SENSOR_TX_PIN = UNUSED_PIN;
  #if (AQI_SENSOR == PRESENT)
    #error("An AQI Sensor may not be used in this configuration")
  #elif (AQI_SENSOR == MOCK)
    #define PMS5003_READINGS PMS5003_AVAIL_READINGS
  #endif

  #if (BME_SENSOR == PRESENT)
    #define BME280_READINGS (BME280_AVAIL_READINGS)
    constexpr uint8_t BME_I2C_ADDR = 0x76;
  #elif (BME_SENSOR == MOCK)
    #define BME280_READINGS (BME280_AVAIL_READINGS)
    constexpr uint8_t BME_I2C_ADDR = 0x00;
  #endif

  #if (DHT22_SENSOR == PRESENT)
    #define DHT22_READINGS (DHT22_AVAIL_READINGS)
    constexpr uint8_t DHT22_PIN = D5;
  #endif

  // ----- Indicators
  constexpr uint8_t NEOPIXEL_PIN = UNUSED_PIN;
  constexpr neoPixelType NEOPIXEL_TYPE = 0;     // Not Used


#elif (BaseConfig == Config_ESP8266WithOLED)
  /*------------------------------------------------------------------------------
   *
   * Config for an ESP8266 with embedded 0.96" OLED in a small enclosure which
   * limits what may be connected and how. This configuration does not support
   * a physical AQI sensor (though you can mock one).
   *
   *----------------------------------------------------------------------------*/

  // ----- I2C Settings
  constexpr uint8_t SDA_PIN = D1;
  constexpr uint8_t SCL_PIN = D2;

  // ----- Display Type
  constexpr auto DISPLAY_DRIVER = DisplayDeviceOptions::DeviceType::SSD1306;
  constexpr uint8_t DISPLAY_I2C_ADDRESS = 0x3c;

  // ----- Buttons
  constexpr uint8_t physicalButtons[] = { D3, UNUSED_PIN };
  constexpr uint8_t syntheticGrounds[] = { UNUSED_PIN };

  // ----- Air Quality Sensor
  #define USE_SW_SERIAL 1
  constexpr uint8_t SENSOR_RX_PIN = UNUSED_PIN;
  constexpr uint8_t SENSOR_TX_PIN = UNUSED_PIN;
  #if (AQI_SENSOR == PRESENT)
    #error("An AQI Sensor may not be used in this configuration")
  #elif (AQI_SENSOR == MOCK)
    #define PMS5003_READINGS PMS5003_AVAIL_READINGS
  #endif

  #if (BME_SENSOR == PRESENT)
    #define BME280_READINGS (BME280_AVAIL_READINGS)
    constexpr uint8_t BME_I2C_ADDR = 0x76;
  #elif (BME_SENSOR == MOCK)
    #define BME280_READINGS (BME280_AVAIL_READINGS)
    constexpr uint8_t BME_I2C_ADDR = 0x00;
  #endif

  // ----- Indicators
  constexpr uint8_t NEOPIXEL_PIN = UNUSED_PIN;
  constexpr neoPixelType NEOPIXEL_TYPE = 0;     // Not Used


#elif (BaseConfig == Config_ESP32WithOLED)
  /*------------------------------------------------------------------------------
   *
   * Config for an ESP32 with embedded 0.96" OLED in a small enclosure which
   * limits what may be connected and how. This configuration does not support
   * a physical AQI sensor (though you can mock one).
   * - WEMOS LOLIN32
   *----------------------------------------------------------------------------*/

  // ----- I2C Settings
  constexpr uint8_t SDA_PIN = 5;
  constexpr uint8_t SCL_PIN = 4;

  // ----- Display Type
  constexpr auto DISPLAY_DRIVER = DisplayDeviceOptions::DeviceType::SSD1306;
  constexpr uint8_t DISPLAY_I2C_ADDRESS = 0x3c;

  // ----- Buttons
  constexpr uint8_t physicalButtons[] = { 25, 13 };
  constexpr uint8_t syntheticGrounds[] = { UNUSED_PIN };

  // ----- Air Quality Sensor
  constexpr uint8_t SENSOR_RX_PIN = UNUSED_PIN;
  constexpr uint8_t SENSOR_TX_PIN = UNUSED_PIN;
  #if (AQI_SENSOR == PRESENT)
    #error("An AQI Sensor may not be used in this configuration")
  #elif (AQI_SENSOR == MOCK)
    #define PMS5003_READINGS PMS5003_AVAIL_READINGS
  #endif

  #if (BME_SENSOR == PRESENT)
    #define BME280_READINGS (BME280_AVAIL_READINGS)
    constexpr uint8_t BME_I2C_ADDR = 0x76;
  #elif (BME_SENSOR == MOCK)
    #define BME280_READINGS (BME280_AVAIL_READINGS)
    constexpr uint8_t BME_I2C_ADDR = 0x00;
  #endif

  // ----- Indicators
  constexpr uint8_t NEOPIXEL_PIN = UNUSED_PIN;
  constexpr neoPixelType NEOPIXEL_TYPE = 0;     // Not Used


#elif (BaseConfig == Config_ESP32Mini)
  /*------------------------------------------------------------------------------
   *
   * Config for an ESP32 D1 Mini built up with discrete components
   *
   *----------------------------------------------------------------------------*/

  // ----- I2C Settings
  constexpr uint8_t SDA_PIN = 21;
  constexpr uint8_t SCL_PIN = 22;

  // ----- Display Type
  #if defined(GUI_DSPLY)
    constexpr auto DISPLAY_DRIVER = DisplayDeviceOptions::DeviceType::SH1106;
    constexpr uint8_t DISPLAY_I2C_ADDRESS = 0x3c;
  #else
    constexpr auto DISPLAY_DRIVER = DisplayDeviceOptions::DeviceType::NONE;
    constexpr uint8_t DISPLAY_I2C_ADDRESS = 0x00;
  #endif

  // ----- Buttons
  constexpr uint8_t physicalButtons[] = { 13, UNUSED_PIN };
  constexpr uint8_t syntheticGrounds[] = { UNUSED_PIN };


  // ----- Air Quality Sensor
  #if (AQI_SENSOR == PRESENT)
    #define PMS5003_READINGS PMS5003_AVAIL_READINGS
    constexpr uint8_t SENSOR_RX_PIN = 16;
    constexpr uint8_t SENSOR_TX_PIN = 17;
  #elif (AQI_SENSOR == MOCK)
    #define PMS5003_READINGS PMS5003_AVAIL_READINGS
    #define USE_SW_SERIAL 1
    constexpr uint8_t SENSOR_RX_PIN = UNUSED_PIN;
    constexpr uint8_t SENSOR_TX_PIN = UNUSED_PIN;
  #else
    constexpr uint8_t SENSOR_RX_PIN = UNUSED_PIN;
    constexpr uint8_t SENSOR_TX_PIN = UNUSED_PIN;
  #endif

  #if (BME_SENSOR == PRESENT)
    #define BME280_READINGS (BME280_AVAIL_READINGS)
    constexpr uint8_t BME_I2C_ADDR = 0x76;
  #elif (BME_SENSOR == MOCK)
    #define BME280_READINGS (BME280_AVAIL_READINGS)
    constexpr uint8_t BME_I2C_ADDR = 0x00;
  #endif

  // ----- Indicators
  constexpr uint8_t NEOPIXEL_PIN = 21;
  constexpr neoPixelType NEOPIXEL_TYPE = NEO_GRB + NEO_KHZ800;

#endif


#endif  // Configs_h
