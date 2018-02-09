# WPro58 for Eachine PRO58 and Quanum HB5808 modules.
STM32 MCU based diversity module firmware.
This firmware code is based on [Shea Ivey's rx5808-pro](https://github.com/sheaivey/rx5808-pro-diversity) 


### Pinouts and programming.
To program your PRO58 or HB5808 module you have to use ST-link programmer.
Connect 5V power and GND via Fatshark connector and GND, SWDIO and SWDCLK to ST-Link via SWD connector.
Remember that module draws 360 mA of current, power it with efficient power supply!
To edit/build code you need to download [SW4STM32](http://www.openstm32.org/HomePage) IDE (Eclipse based)

#### Eachine PRO58 pinouts
![Pinouts](docs/PRO58_pinouts.jpg)

___

#### Quanum HB5808 pinouts
![Pinouts](docs/HB5808_pinouts.jpg)

___

### Big thanks to
- **mihero** for contributing
- **20Sy** for HB5808 module ;)

___

<a rel="license" href="http://creativecommons.org/licenses/by-nc-sa/4.0/"><img alt="Creative Commons License" style="border-width:0" src="https://i.creativecommons.org/l/by-nc-sa/4.0/88x31.png" /></a><br />This work is licensed under a <a rel="license" href="http://creativecommons.org/licenses/by-nc-sa/4.0/">Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License</a>.
