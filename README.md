# WhosTheRat
WhosTheRat Arduino Card Game

![alt tag](https://raw.githubusercontent.com/blacksanta69/WhosTheRat/master/multiplexing_bb.jpg)
Connections are only indicated for first LED bargraph.

Inspired by http://playground.arduino.cc/Learning/4051 and https://www.arduino.cc/en/Tutorial/ShiftOut

This sketch is for a 6 player card game. Each player gets a range and a health score. The range and health are illustrated on a 10 led bargraph with 4 pins on the left and 4 on the right indicating health and range. When the players health is diminished, a red LED turns on indicating death.

Connecting a 220 ohm resistor per LED would be a wise decision to extend the life of the LEDs.


The game uses:
- 6x 10 LED bargraph displays
- 6x 74HC595 Shift Registers (or similar)
- 2x 74HC4051 Multiplexers (or similar)
- 12x 10kOhm potentiometers

The 2 mux (multiplexers) read the values of the 12 pots.

The 6 Shift Registers are connected (series/ daisy-chained) to turn on/ off 8 of the 10 LED bargraphs.

Pins 22 to 27 on the mega are used for turning the kill LEDs on and off.