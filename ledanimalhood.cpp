/*

Source code for the LED Animal Spirit Hood

Version 0.1

Author: Song Hoang

Uses Arduino Uno, TLC5947, and Accelerometer

*/


#include "Adafruit_TLC5947.h"
#include <LiquidCrystal.h>

// Makes a TLC LED Driver object to manipulate led's with
Adafruit_TLC5947 tlc = Adafruit_TLC5947(1,5,4,6);
LiquidCrystal lcd(7, 8, 9, 10, 11 , 12);

unsigned long ledMillis = 469;
unsigned long currentMillis = 0;
unsigned long previousMillis = 0;

int LEDMode = 0;


void setup(){

  Serial.begin(9600);
  tlc.begin();
  lcd.begin(16, 2);
}


void loop(){

/*
  if(millis() >= ledMillis)
  {
    ledMillis = 469 - (millis() - ledMillis) + millis();
    blink();
  }
*/
  //displayText();
  rainbow(1);

}

void displayText(){
  lcd.setCursor(0,1);
  lcd.print("RAVE MODE");
  lcd.setCursor(0,1);
  lcd.print(" RAVE MODE");
}

// LED's in the sleeves undergo a rainbow color effect, displaying all colors
void rainbow(int interval){
 // On for loop, i < num, num is dependent on distance number below
  for(int i = 8; i < 216; i++){
    for(int j = 0; j < 8; j++){
    // Last number determines distance of color between LED's
      Wheel(j, (i * 16) + (j * 80)); 
      tlc.write();
      //delay(interval);
    }
  }
  for(int i = 216; i > 8; i--){
    for(int j = 0; j < 8; j++){
    // Last number determines distance of color between LED's
      Wheel(j, (i * 16) + (j * 80)); 
      tlc.write();
      //delay(interval);
    }
  }


}


void blink(){

  if(LEDMode == 0){
   tlc.setLED(1, 0, 4095, 0);
   tlc.write();
   LEDMode = 1;
  }

  else if(LEDMode == 1){
  tlc.setLED(1, 0, 0, 4095);
  tlc.write();
  LEDMode = 0;
  }

}


void Wheel(uint8_t ledn, uint16_t WheelPos) {
  if(WheelPos < 1365) {
    tlc.setLED(ledn, 3*WheelPos, 4095 - 3*WheelPos, 0);
  } else if(WheelPos < 2731) {
    WheelPos -= 1365;
    tlc.setLED(ledn, 4095 - 3*WheelPos, 0, 3*WheelPos);
  } else {
    WheelPos -= 2731;
    tlc.setLED(ledn, 0, 3*WheelPos, 4095 - 3*WheelPos);
  }
}