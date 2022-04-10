# ESP-Rider_L298N_version

ESP-Rider with L298N for a Fusion Generator look.  12 volt and USB output from a 3amp power pack is recommended for meccano motors such as illustrated. (pic to come)

Uses an ESP32 Dev kit module to control a small cars direction and speed with a L298N H-bridge.

The code uses PWM and channels.

This will set up a wifi access point on the car which will allow a user to log in and access 192.168.4.1 which is the car's control html page.

Esp32 must be set up in preferences in Arduino IDE.  https://randomnerdtutorials.com/installing-the-esp32-board-in-arduino-ide-windows-instructions/


Library required:   
                 ServoESP32 

Parts required:       
                 ESP32 devkit or similar.    
                 L298N H-bridge.     
                 Small 9g servo, two wires are switched to plug in the esp32 dev kit module.      
                 USB charging block with two outputs. A 12 volt 3 amp power pack was adequate.   
                 Buck converter.   
                 Electric motor of your choice.   
                 
This works best on an Android phone.  Computer screens are okay.  Apple units, shrug.

Can steer either with the slider bar or by tapping the left or right buttons.  Speed/Direction control buttons must be held.
                 
