# Air Quality Monitor

This project is a low-cost air-quality monitor built and tested by students in Georgetown University's STIA 315: International Air Quality Lab class in Spring 2018. The project uses open hardware and software, and is intended for use cases where people are interested in learning more about their ambient air quality without having having access to professional-grade air-quality monitors. As a low-cost device, this air quality monitor is not fully calibrated and cannot be used to replace professional-grade air-quality monitoring; however, it can measure trends in air pollution over time and help provide a better understanding of local air quality.

## Monitor hardware

The monitor is built from the following hardware components:

* [Adafruit Feather HUZZAH with ESP8266](https://www.adafruit.com/product/2821)
* [Adafruit FeatherWing OLED](https://www.adafruit.com/product/2900)
* DHT11 temperature-humidity sensor (various suppliers)
* [PPD42NS dust sensor](https://www.seeedstudio.com/Grove-Dust-Sensor（PPD42NS）-p-1050.html)
* [Adafruit LiPo battery](https://www.adafruit.com/product/328)
* [Adafruit 6V/2W solar panel](https://www.adafruit.com/product/200)
* [Adafruit USB/DC/Solar LiPo charger](https://www.adafruit.com/product/390)
* [Large plastic enclosure](https://www.adafruit.com/product/905)
* Various 3D-printed components available on [Thingiverse](https://www.thingiverse.com/thing:2857819)

## Monitor software

This repo includes code that can be edited and uploaded to the Feather HUZZAH via the [Arduino IDE](https://www.arduino.cc/en/Main/Software). The Feather transmits measurement data via WiFi to [ThingSpeak](https://thingspeak.com), an IoT cloud platform that provides data storage and visualization.

## Other

Contributions, comments and issue notifications are welcome. STIA 315 is one of the [Centennial Labs](https://sfs.georgetown.edu/sfs-centennial-labs/) of the Georgetown University [School of Foreign Service](http://sfs.georgetown.edu), taught by Prof. Colin McCormick.
