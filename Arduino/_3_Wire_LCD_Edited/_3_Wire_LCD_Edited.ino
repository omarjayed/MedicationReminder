#include <LCD.h>
#include <LiquidCrystal_SR.h>
#include <LiquidCrystal.h>
LiquidCrystal_SR lcd(12, 13, 8); 
            //  DATA CLOCK LATCH

//SH_CP->CLOCK->13
//ST_CP->LATCH->8
//DS->DATA->12



void setup() {  
  
 lcd.begin(16, 2); // set up the LCD's number of columns and rows: 
 lcd.clear(); // clear the screen
 
  
}

void loop() { 
  //lcd.clear(); // clear the screen
  lcd.setCursor(0, 0); // put cursor at colon 0 and row 0
  lcd.print("3-Wire LCD By"); // print a text
  lcd.setCursor(0, 1); // put cursor at colon 0 and row 1
  lcd.print("TSB by Nur MD"); // print a text
    
  }
