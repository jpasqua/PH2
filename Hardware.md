# Defining your Hardware

Some of the hardware components in your device are known to the WebThing/WebThingApp infrastructure and some are not. For example, if you're building a WebThingApp, then you have a display and WebThingApp needs to know about it. You may also have a temperature sensor that you want handled by one of the built-in WebThing classes. Alternatively, you may have motor controllers or other devices that are completely unknown to WebThing/WebThingApp.

Whatever the case may be, you need to configure and initialize all of your hardware as well as the ESP pins that talk to them. That is, you may need to set certain pins as INPUT or OUTPUT, configure the I2C interface, etc.

## Displays

Configuring your display is a multistep process and depends what sort of display you're using. At the moment (2021/10/21) WebThingApp supports two general classes of display:

* OLED:
	* A small 128*64 monochrome display with no input capability of its own.
	* Driven by the "ThingPulse OLED SSD1306" [library](https://github.com/ThingPulse/esp8266-oled-ssd1306).
	* Supports several variations of display and display driver.
* Touch:
	* A 320x240 color display with touch input capability
	* Driven by the ["TFT_eSPI"](https://github.com/Bodmer/TFT_eSPI) library from Bodmer.
	* Supports many variations of display and display driver.

### Selecting a display

Selecting your display requires you to specify both the general class of display you're using and the specific display/driver.

**Choose the general display type**:

* Inside the WebThingApp library, edit the file: /src/gui/devices/DeviceSelect.h
* You will see two #defines. Uncomment the one associated with your display, and comment out the other. For example, to select the OLED display type you should have:

````
	// #define DEVICE_TYPE DEVICE_TYPE_TOUCH
	#define DEVICE_TYPE DEVICE_TYPE_OLED
````

**Choose the specific display/driver**:

For ***Touch*** displays follow the instructions in MMM to configure `User_Setup_Select.h` to select the specific display and driver type in use.

For ***OLED*** displays:

In your app you need to call Display.setDeviceOptions and pass in a DisplayDeviceOptions object. It defines which driver to use, which pins are conencted to it, and its address.


Buttons

WebThing

WebThingApp

App