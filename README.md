# Word-Clock with Android App for color changing (WIP)

<img src="https://github.com/MarcelScherer/Word-Clock/blob/master/Docu/IMG_20190517_214902.jpg" alt="drawing" width="600"/>

in addition, the following external sources are used in this project ... <br>
[NTPClinet for get actual time](https://github.com/arduino-libraries/NTPClient) <br>
[RTClib for offline function](https://github.com/adafruit/RTClib) <br>
[NeoPixelBus for control ws2812b strip](https://github.com/Makuna/NeoPixelBus) <br>
letterboxes.scad (I don't know the original link, sorry ...)

# Hardware

First you have to print the [STL files](https://github.com/MarcelScherer/Word-Clock/tree/master/Hardware) on a 3D printer. If you want to adjust the size, you can also create your own STL files with the help of the [script](https://github.com/MarcelScherer/Word-Clock/blob/master/Hardware/letterboxes.scad) and openSCAD. <br>
<img src="https://github.com/MarcelScherer/Word-Clock/blob/master/Docu/box.PNG" alt="drawing" width="250"/>
<img src="https://github.com/MarcelScherer/Word-Clock/blob/master/Docu/letter.PNG" alt="drawing" width="250"/> <br>
<br>
After Printing the boxes and letters ... <br>
<img src="https://github.com/MarcelScherer/Word-Clock/blob/master/Docu/IMG_20190319_211238.jpg" alt="drawing" width="250"/>
<img src="https://github.com/MarcelScherer/Word-Clock/blob/master/Docu/IMG_20190419_222750.jpg" alt="drawing" width="250"/>  <br>

... then you have to put the letters in the boxes.  <br>
<img src="https://github.com/MarcelScherer/Word-Clock/blob/master/Docu/IMG_20190419_222646.jpg" alt="drawing" width="400"/>

After that you have to fix the led ws2812b stripes ... <br>
<img src="https://github.com/MarcelScherer/Word-Clock/blob/master/Docu/IMG_20190325_195632.jpg" alt="drawing" width="250"/>
<img src="https://github.com/MarcelScherer/Word-Clock/blob/master/Docu/IMG_20190325_195642.jpg" alt="drawing" width="250"/>  <br>

# Software

You have to compile and flash the sorce (I do it with Visual Studio Code) an an ESP32. You can change the pinning for the led-strip in the cfg.h file. I use the pinning ... <br>

| Pin      |   LED-Strip   | Color | 
|----------|:-------------:|-------|  
| P15      |  Led Pin      | green |
| GND      |  Ground       | white |
| 5V       |  Power Supply | red   |

| Pin      | RTC           | Color | 
|----------|:-------------:|-------|  
| P21      |  SDA          | green |
| P22      |  Ground       | yellow|
| 5V       |  Power Supply | red   |
| GND      |  GND          | blue  |

you can/shold config also in the cfg.h file ... <br>

| define             | function                              | 
|--------------------|---------------------------------------|
|INTERRUPT_PRESCALER | 1000.000 ticks per second             |
|                    | depend on board                       |
|INTERRUPT_TICK      | how many ticks to next interrupt      |
|WIFIPWD             | wifi password                         |
|WLAN_SSID           | wifi ssid                             |
|UPDATE_TIME_VAL     | ntc update time interval in hours     |
|USE_RTC             | if you use a RTC or not true/false    |
|DEBUG               | if you activate debug information     |
|                    | over seral output true/false          |
|LED_PIN             | Pin for led strip                     |
|NUM_OF_PIXEL        | number of led pixel 121 (11 * 11)     |
|MAX_BRIGHTNESS_ON   | muximum brightness for led's where on |
|MAX_BRIGHTNESS_OFF  | muximum brightness for led's where off|
|TIME_DISPLAY_OFF    | time for display off                  |
|TIME_DISPLAY_ON     | time for display on                   |

if TIME_DISPLAY_OFF and TIME_DISPLAY_ON are same, the leds are allways on <br>

<img src="https://github.com/MarcelScherer/Word-Clock/blob/master/Docu/front_1.jpg" alt="drawing" width="500"/> 
<img src="https://github.com/MarcelScherer/Word-Clock/blob/master/Docu/rear_1.jpg" alt="drawing" width="500"/> 

# Android App

The android app find the word-clock with the mDNS "Word_Clock.local" <br>

<img src="https://github.com/MarcelScherer/Word-Clock/blob/master/Docu/Screenshot_20190530-212208.png" alt="drawing" width="300"/>
