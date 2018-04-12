// Date and time functions using a DS1307 RTC connected via I2C and Wire lib
#include <Wire.h>
#include "RTClib.h"
#define   SER_BAUD 9600


RTC_DS1307 rtc;
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

void initPeripherals(int RTCmode = SquareWave1HZ, bool autoSetTime = false)
{
  while (!Serial); // for Leonardo/Micro/Zero
  
  Serial.begin(SER_BAUD);
  
  if (!rtc.begin() && !rtc.isrunning()) 
  {
    Serial.println("RTC ERROR!\nDevice Halting...");
    while (1);
  }
  else if( rtc.readSqwPinMode() != SquareWave1HZ)
  {
    Serial.println("RTC FOUND!\nPulse MODE : 1 Hz ");
    rtc.writeSqwPinMode(SquareWave1HZ);
  }
  else if (autoSetTime)
  {
     Serial.println("RTC OK!\nPulse mode : 1 Hz\nTime Auto Set : ON");
     
     rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }  
  else
  {
     Serial.println("RTC OK!\nTIME OK!\nPulse mode : 1 Hz");
  }
}


bool setTime(bool set = true, int YY=2017, int MM=1, int DD=1, int hh=0, int mm=0, int ss=0)
{
  if(set) 
  {
    if(!rtc.isrunning())
    {
     Serial.println("RTC ERROR!");    
     return false;
    }
    else
    {
      rtc.adjust(DateTime(YY, MM, DD, hh, mm, ss));
      Serial.println("TIME SET: \n Year: "+String(YY)+ 
                                  " Month: "+String(MM)+ 
                                  " Day: "+String(DD)+ 
                                  "\n Hour: "+String(hh)+ 
                                  " Minute: "+String(mm)+
                                  " Second: "+String(ss)); 
      return true;
    }
  }
  return false;
}

void setup () 
{

    initPeripherals(); // SquareWave1HZ, true, compile time timeset

    if(setTime(true, 2017, 12, 12, 12 ,12 ,12 ))
    Serial.println("Timeset Success");
    
   

}

void loop () {
    DateTime now = rtc.now();
    Serial.print(now.year(), DEC);
    Serial.print('/');
    Serial.print(now.month(), DEC);
    Serial.print('/');
    Serial.print(now.day(), DEC);
    Serial.print(" (");
    Serial.print(daysOfTheWeek[now.dayOfTheWeek()]);
    Serial.print(") ");
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    Serial.print(now.minute(), DEC);
    Serial.print(':');
    Serial.print(now.second(), DEC);
    Serial.println();
    delay(1000);
}
