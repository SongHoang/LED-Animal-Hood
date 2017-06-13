/*

Source code for the LED Animal Spirit Hood

Version 0.1

Author: Song Hoang

Uses Arduino Uno, TLC5947 12-bit PWM, and LSM303C Accelerometer

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

int LEDValue[8][3];

int doNotWrite = 0;

double bpm = 0;

void setup(){

  Serial.begin(9600);
  tlc.begin();
  lcd.begin(16, 2);

    for(int i = 0; i < 8; i++){
    LEDValue[i][0] = 4000;
    LEDValue[i][1] = 1000;
    LEDValue[i][2] = 4000;
    tlc.setLED(i, LEDValue[i][0], LEDValue[i][1] , 
        LEDValue[i][2]);
  }
}


void loop(){

  //displayText();
  //rainbow(10);
  quickFlash();

}

void test(){

}
void check(){

    if(millis() >= ledMillis)
    {
      ledMillis = 469 - (millis() - ledMillis) + millis();
      quickFlash();
    }
}
// Fades LED brightness to 0, then back to normal again, similar to a music beat
// Adjust variables named smooth, power1, and delayTime to adjust Flash
void quickFlash(){
	bpm++;
	displayText(bpm);
  doNotWrite = 1;
  int smooth = 9; // 1/smooth, determines how many times led strength 
          // will be divided in half before reaching 0
  int savedMillis;
  double power1 = 0.1;
  double power = 1 - power1;
  int delayTime = 2.7; // in milliseconds


  for(int i = 1; i < smooth + 1; i++){
    for(int j = 0; j < 8; j++){

      tlc.setLED(j, LEDValue[j][0] * power, LEDValue[j][1] * power, 
        LEDValue[j][2] * power);
      delay(delayTime);
      
    }
    power = power - power1;
    tlc.write();
    
  }

  // Then briefly go into darkness
  //darkness();
  delay(delayTime);

  // Go back to full brightness of color

  for(int i = smooth; i > 0; i--){
    for(int j = 0; j < 8; j++){

      tlc.setLED(j, LEDValue[j][0] * power, LEDValue[j][1] * power, 
        LEDValue[j][2] * power);
      delay(delayTime);
      
    }
    power = power + power1;
    tlc.write();
    
  }

  doNotWrite = 0;


}

// Set all LED's to 0, or no light
void darkness(){
  for(int i = 0; i < 8; i++){
    tlc.setLED(i, 0, 0, 0);
  }
  tlc.write();
}


void writeLED(int ledNum, int r, int g, int b){

  LEDValue[ledNum][0] = r;
  LEDValue[ledNum][1] = g;
  LEDValue[ledNum][2] = b;
  if(!doNotWrite){
    tlc.setLED(ledNum, r, g, b);
    tlc.write();
  }

}

void displayText(double num){
  lcd.setCursor(0,1);
  lcd.print(num);
  
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
    check();
  }
  for(int i = 216; i > 8; i--){
    for(int j = 0; j < 8; j++){
    // Last number determines distance of color between LED's
      Wheel(j, (i * 16) + (j * 80)); 
      tlc.write();
      //delay(interval);
    }
    check();
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
    writeLED(ledn, 3*WheelPos, 4095 - 3*WheelPos, 0);
  } else if(WheelPos < 2731) {
    WheelPos -= 1365;
    writeLED(ledn, 4095 - 3*WheelPos, 0, 3*WheelPos);
  } else {
    WheelPos -= 2731;
    writeLED(ledn, 0, 3*WheelPos, 4095 - 3*WheelPos);
  }
}