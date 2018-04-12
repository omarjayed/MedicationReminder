#include <Wire.h>
#include <RTClib.h>


RTC_DS1307 rtc;
int mode_index = 0;

Ds1307SqwPinMode modes[] = {OFF, ON, SquareWave1HZ, SquareWave4kHz, SquareWave8kHz, SquareWave32kHz};


void setup() {
  // put your setup code here, to run once:
  Wire.begin();
  Serial.begin(9600);
  delay(2000);
  Serial.println("Initializing...");

  if (! rtc.begin()) {
    
    while (1)
    Serial.println("Couldn't find RTC");
  }
  Serial.println("Rtc...");
  rtc.writeSqwPinMode(modes[2]);
  Serial.println(rtc.readSqwPinMode());
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("Initializing...");
  readTime();
  delay(2000);
}

void readTime(){
  DateTime now = rtc.now();
  unsigned int nowTime = now.hour();
  nowTime *= 100;
  nowTime += now.minute();
  Serial.println();
  Serial.println(nowTime);
  Serial.println();
}
