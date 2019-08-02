# OS-Project1819

## Controller w/flex sensors

What do you need:
* a computer (lol)
* an arduino (or another microcontroller, we used an ATMEGA2560)
* 5 flex sensors (we used one for each finger, but it's customizable changing a little bit source code)
* a breadboard
* 5 10k-resistors (optional)
* 2 jumper wires
	
How to connect flex sensors to your arduino:
* connect arduino 5V pin to breadboard positive line with a jumper wire
* connect arduino GND pin (any) to breadboard negative line with a jumper wire
* connect each sensor signal pin to arduino pins from A0 to A4
* connect each sensor positive pin to breadboard positive line
* (optional) connect a 10k-resistor in series to each sensor signal pin 
* connect each sensor negative pin to breadboard negative line

To trigger on pc the actions received from arduino, we decided to use XDO library that allows to trigger both mouse and keyboard interrupts.
We also wrote a shell for users, with every command that can be useful to interact and configure the controller.

To launch the code, follow these steps:

1)To upload code in your arduino, go to /Arduino folder, open a terminal and type:
* make
* make main.hex 

2)To launch the shell go to /sw_pc folder, open a terminal and type:
* make
* ./ghost
  Be sure that arduino is connected to your pc and that the dedicated code has been uploaded into it (step 1).

For more commands and how to use them type "help" or "h" in the shell.

Enjoy!
