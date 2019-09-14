# Angle Sensor
*****Workings of a Physical and Digital Protractor *****

Steps:  
1. Install USB BDM drivers for the computer.
2. Install and Setup CodeWarrior and MATLAB.  
3. Move the jumper on the USBDM to 3V, connect the EsduinoXtreme to the PC through COM3 
4. If COM3 is already in use, select another port and change the Matlab code to correspond to that port. 
5. On the Esduino, move the jumpers JB3 to the right and JB4 to the left to enable AN7. 
6. Use the diagram below for an Arduino which is similar to Esduino and follow the wiring configurations. 
7. Run the code on Code Warrior with libraries SCI.h and SCI.C included and Matlab to get a graph of angle vs time. 
        Make sure the Matlab code is stored in the same path as the CodeWarrior Code.  
8. The button has 3 modes off, Mode 0 and Mode 1.
        Initially the system is in the off state so Matlab does not display anything. 
        In mode 0 the LED’s display the degree in BCD. 
        Alternately, In mode 1, the LED’s light up in bar format in interval of 10 degrees.  
 
 
 ![Circuit](https://user-images.githubusercontent.com/24720856/64912008-ff989780-d6f6-11e9-9794-6e7216cf1aca.png)

 

 
 
Sincerely,
	Pavneet Gill

	
