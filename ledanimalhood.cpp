/*

Source code for the LED Animal Spirit Hood

Version 0.1

Author: Song Hoang

Uses Arduino Uno, TLC5947, and Accelerometer

*/


#include "Adafruit_TLC5947.h"

// Makes a TLC LED Driver object to manipulate led's with
Adafruit_TLC5947 tlc = Adafruit_TLC5947(1,5,4,6);

void setup(){

	Serial.begin(9600);
	tlc.begin();

	
}


void loop(){
	rainbow(10);
}

// LED's in the sleeves undergo a rainbow color effect, displaying all colors
void rainbow(int wait){

	for (int i = 0; i < 4096; i++){
		for (int j = 0; j < 8; j++){

			  if(i < 1365) {
			    tlc.setLED(j, 3*i, 4095 - 3*i, 0);
			  } 
			  else if(i < 2731) {
			    i -= 1365;
			    tlc.setLED(j, 4095 - 3*i, 0, 3*i);
			  } 
			  else {
			    i -= 2731;
			    tlc.setLED(j, 0, 3*i, 4095 - 3*i);
			  }

		}
	}
	tlc.write();
	delay(wait);

}