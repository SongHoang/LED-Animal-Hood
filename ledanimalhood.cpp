/*

Source code for the LED Animal Spirit Hood

Version 0.1

Author: Song Hoang

Uses Arduino Uno, TLC5947 12-bit PWM, and LSM303C Accelerometer

To-Do 
-  Create quickFlash version 2, decrease brightness according to log(n) versus
version 1 where it's decreasing by 10% every stage
DONE - Create method to read input from accelerometer and detects sudden change in sensor movement
- Rewrite quickFlash to use millis() instead of delay()
- Optimize code to use smaller variables to save memory (u_int8, etc...)

*/


#include "Adafruit_TLC5947.h"
#include <LiquidCrystal.h>

#include "Wire.h"
#include "SparkFunIMU.h"
#include "SparkFunLSM303C.h"
#include "LSM303CTypes.h"

// Makes a TLC LED Driver object to manipulate led's with
Adafruit_TLC5947 tlc = Adafruit_TLC5947(1,5,4,6);
LiquidCrystal lcd(7, 8, 9, 10, 11 , 12);
LSM303C imu;


unsigned long ledMillis = 469;
unsigned long currentMillis = 0;
unsigned long previousMillis = 0;

int LEDMode = 0;

int LEDValue[8][3];

int doNotWrite = 0;

double bpm = 0;

int accValues [100][3];
int accPosition = 0;

void setup(){

  Serial.begin(115200);
  tlc.begin();
  lcd.begin(16, 2);
  imu.begin(MODE_I2C,MAG_DO_40_Hz,MAG_FS_16_Ga,MAG_BDU_ENABLE,MAG_OMXY_HIGH_PERFORMANCE,
                      MAG_OMZ_HIGH_PERFORMANCE,MAG_MD_CONTINUOUS,ACC_FS_2g,
                      ACC_BDU_ENABLE,ACC_X_ENABLE|ACC_Y_ENABLE|ACC_Z_ENABLE,ACC_ODR_100_Hz);

    for(int i = 0; i < 8; i++){
      LEDValue[i][0] = 4000;
      LEDValue[i][1] = 1000;
      LEDValue[i][2] = 4000;
      tlc.setLED(i, LEDValue[i][0], LEDValue[i][1] , 
          LEDValue[i][2]);

    }

    for(int i = 0; i < 100; i++){
      accValues[i][0] = 0;
      accValues[i][1] = 0;
      accValues[i][2] = 0;
    }
}


void loop(){

  //displayText();
  //rainbow(10);
  //quickFlash();
  saveAccVal();
  checkForJolt();

}

// Reads and saves Accelerometer values to accValues array
void saveAccVal(){

  if(accPosition == 101){
    accPosition = 0;
  }

  accValues[accPosition][0] = imu.readAccelX();
  accValues[accPosition][1] = imu.readAccelY();
  accValues[accPosition][2] = imu.readAccelZ();

  accPosition++;

  if(accPosition == 101){
    accPosition = 0;
  }
}

// Check accValues array for Jolt and return 1 if true, 0 otherwise
// thres determines sensitivty to jolt, period determines time range of comparision
int checkForJolt(){

  int val;
  int thres = 200;
  int period = 5;


  if(accValues[accPosition][2] > thres){

    if(accPosition < period){
      val = 100 - accPosition;
    }
    else{
      val = accPosition - period;
    }

    if(accValues[val][2] < -thres){
      quickFlash();
      return 1;
    }
    else{
      return 0;
    }

  }

  else if(accValues[accPosition][2] < -thres){

    if(accPosition < period){
      val = 100 - accPosition;
    }
    else{
      val = accPosition - period;
    }

    if(accValues[val][2] > thres){
      quickFlash();
      return 1;
    }
    else{
      return 0;
    }

  }

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
  int smooth = 9; 
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