/*
 *  File:   Counter.ino
 *
 *  Author: @Humoba
 *  Date:   2018-07-11
 *
 *  Description:
 *  When the countdown is equal to zero, turn off or turn on the relay connected to pin 8. Can be control some lamp,
 *  fan or enything (10 Ampers max).
 *
 */
 
/***************************************************************************************************************************/
 
/*
 *  Libraries  
 */
#include <Wire.h>
#include "rgb_lcd.h"


/*
 *  Variables  
 */
unsigned long time;
unsigned int hours = 0;
unsigned int minutes = 0;
unsigned int seconds = 0;

bool configuring = false;
bool counting = false;

//  Option button
const int button1 = A0;
//  Change/Select button
const int button2 = A1;
//  Pin relay
const int relay = 8;

byte option = 0;

String text_display = "Set Hours:";

rgb_lcd lcd;

/*
 *  Setup function  
 */
void setup() {
    // set up the LCD's number of columns and rows:
    lcd.begin(16, 2);
    // Print a message to the LCD.
    lcd.clear();
    lcd.noBlinkLED();
    print_time(hours, minutes, seconds, "Set timer:");
    
    time = millis();

    pinMode(relay, OUTPUT);
    digitalWrite(relay, LOW);
}

/*
 *  Loop function  
 */
void loop() {

  //Every second
  if(millis() - time > 1000 && counting) {
   print_time(hours, minutes, seconds, "Time:");
   
   if(hours == 0 && minutes == 0 && seconds == 0) {
    timeIsUp();
   }
   else
    digitalWrite(relay, LOW);

   //Time control
   if(seconds > 0){
      seconds--;
    }
    else{
      if(minutes > 0) {
        minutes--;
        seconds = 59;
      }
      else {
        if(hours > 0){
          hours--;
          minutes = 59;
          seconds = 59;
        }
      }
    }
  }

  //  Showing menu
  if(!counting) {
    //Selector
    if(digitalRead(button1)){
      if(configuring){
        option++;
        if(option == 5)
          option = 0;
        switch(option){
          case 0:
            text_display = "Set Hours:";
            break;
          case 1:
            text_display = "Set Minutes:";
            break;
          case 2:
            text_display = "Set Seconds:";
            break;
          case 3:
            text_display = "Start!";
            break;
          case 4:
            text_display = "Reset";
            break;
        }
        print_time(hours, minutes, seconds, text_display);
      }
      else {
        configuring = true;
        option = 0;
        print_time(hours, minutes, seconds, "Set Hours:");
      }
      delay(350);
    }

    //  Settings
    if(digitalRead(button2)){
      if(configuring){
        switch(option){
          case 0:
            hours++;
            //  max: 12 hour
            if(hours == 13)
              hours = 0;
            break;
          case 1:
            minutes++;
            if(minutes == 60)
              minutes = 0;
            break;
          case 2:
            seconds++;
            if(seconds == 60)
              seconds = 0;
            break;
          case 3:
            counting = true;
            configuring = false;
            //  Reset the menu to use it in the next time
            option = 0;
            text_display = "Time:";
            digitalWrite(relay, LOW);
            break;
          case 4:
            //  Reset the menu
            hours = 0;
            minutes = 0;
            seconds = 0;
            break;
        }
        print_time(hours, minutes, seconds, text_display);
      }
      else {
        configuring = true;
        option = 0;
        print_time(hours, minutes, seconds, "Set Hours:");
      }
      delay(350);
    }
  }
}

void print_time(int hours, int minutes, int seconds, String text) {
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(text);
  lcd.setCursor(0,1);
  lcd.display();
  lcd.setRGB(51, 170, 51);
  lcd.noBlinkLED();
  time = millis();

  //Print with time format
  if (hours >= 10 )
    lcd.print(hours);
  else {
    lcd.print("0" + String(hours));
  }
  lcd.print(':');
  if (minutes >= 10 )
    lcd.print(minutes);
  else
    lcd.print("0" + String(minutes));
  lcd.print(':');
  if (seconds >= 10 )
    lcd.print(seconds);
  else
    lcd.print("0" + String(seconds));
}

void timeIsUp() {
  digitalWrite(relay, HIGH);
  counting = false;
  lcd.setRGB(200, 0, 0);
  lcd.blinkLED();
  lcd.clear();
  lcd.print("Time Is Up!");
  lcd.setCursor(0,2);
  lcd.print("Turning off...");
  lcd.display();
  delay(5000);
  lcd.setRGB(0, 0, 0);
  lcd.clear();
  lcd.noDisplay();
  lcd.noBlinkLED();

}

/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
