## Smart Water Level Monitor  
A simple IoT project using an ESP32, HC-SR04 ultrasonic sensor, LED, and 7-segment display to monitor water level in a tank and alert when the level is low.  

### Features  
-  Real-time water level measurement using ultrasonic sensor (HC-SR04)
-  Displays water level (in %) on a 7-segment display
-  LED warning light if level is below threshold
-  Simple, low-cost components
-  Compatible with Arduino IDE


### Components Used
| Component   | Quantity |
|-------------|----------|
| ESP32       | 1        | 
| HC-SR04     | 1        |
|7-Segment Display (5161AS, Common Anode) |	1 |
|LED	        | 1        |
|220Ω Resistors	| 8+     |
|Breadboard & Jumper Wires |	1 set |
|Optional: Water container or tank for testing	| 1-2 |


### How It Works
-  The HC-SR04 measures the distance from the sensor to the water surface.
-  The distance is subtracted from the tank height to calculate the current level.
-  The result is shown on a 7-segment display (tens digit only).
-  If water level is below 20%, the LED lights up as a warning.
