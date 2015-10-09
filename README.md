# DuinoTagV2
Arduino lasertag, based on the instructable here: http://www.instructables.com/id/Duino-Tagger/ .  The code on the page is outdated, so I rewrote it myself.

Currently it is very buggy and poorly structured, but functions at a basic tag game.  My ultimate goal is to make a library that anyone with general programming knowledge and some cash can make a lasertag game out of.

If you would like to develop for this, there are 5 things you will need:

A good IDE, or at least a functional one.  I use <a href="http://dalpix.com/mariamole">this one.</a>

<a href="https://bitbucket.org/teckel12/arduino-toneac/wiki/Home">Tone AC Library</a>

<a href="https://github.com/adafruit/Adafruit_NeoPixel">Neopixel Library</a>

<a href="https://github.com/z3t0/Arduino-IRremote/releases">IRremote Library (I use version 1.0)</a>

You will also have to replace the IRremoteInt.h with the one included in Custom IR.


If you need to test transmision and such:

At least 2 Arduinos (to send and recieve). <a href="http://www.aliexpress.com/item/4PCS-2pcsATmega328-Mini-USB-Board-2pcs-USB-Cable-Nano-3-0-Atmel-ATmega328/1990724032.html">Example</a>

56khz IR receivers and Diodes (you can find them at mouser or digikey)

TODO: Document code

