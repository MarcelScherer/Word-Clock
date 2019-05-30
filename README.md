# Word-Clock with Android App for color changing (WIP)

<img src="https://github.com/MarcelScherer/Word-Clock/blob/master/Docu/Screenshot_20190530-212208.png" alt="drawing" width="100"/>
<img src="https://github.com/MarcelScherer/Word-Clock/blob/master/Docu/IMG_20190517_214902.jpg" alt="drawing" width="400"/>


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

You have to compile and flash the sorce (I do it with Visual Studio Code) an an ESP32. You can change the pinning in the cfg.h file. I use the pinning ... <br>

| Pin      |   function    | 
|----------|:-------------:|
| 15       |  Led Pin      | 
| GND      |  Ground       | 
| 5V       |  Power Supply | 

# Android App

The android app find the word-clock with the mDNS "Word_Clock.local" <br>

<img src="https://github.com/MarcelScherer/Word-Clock/blob/master/Docu/Screenshot_20190530-212208.png" alt="drawing" width="300"/>
