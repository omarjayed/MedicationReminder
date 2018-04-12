#include <ArduinoJson.h>
#include <Wire.h>
#include <RTClib.h>
#include <avr/sleep.h>
#include <LCD.h>
#include <LiquidCrystal_SR.h>
#include <LiquidCrystal.h>
#define   SER_BAUD 9600

int ledPin = 13;
int switchPin = 8;
int wakePin = 2;
int BlPin = 4;
LiquidCrystal_SR lcd(5,6,7);

unsigned int wakeUpTime = 0;
unsigned int nowTime;

int count = 0;
int led[4][2] = {0};
int pinStart = 10;
char json[] = "{\"napa\":[{\"0137\":\"1\"},{\"600\":\"2\"},{\"1800\":\"1\"},{\"quantity\":\"4\"}],\"z-max\":[{\"0134\":\"0\"},{\"600\":\"1\"},{\"1800\":\"0\"},{\"quantity\":\"1\"}]}";

struct timePill {
  int tym;
  int pills;
};

struct medicine {
  String medName;
  timePill tymAndPill[5];
  int quantity;
};

medicine med[10];

// melody for the buzzer
int melody[] = {
  262, 196, 196, 220, 196, 0, 247, 262
};
int noteDurations[] = {
  4, 8, 8, 4, 4, 4, 4, 4
};

RTC_DS1307 rtc;



//rtc initialization test
void initPeripherals(int RTCmode = SquareWave1HZ, bool autoSetTime = false) {
  if (!rtc.begin() && !rtc.isrunning()) {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("RTC ERROR!");
    lcd.setCursor(0,1);
    lcd.print("Device Halting...");
    Serial.println("RTC ERROR!\nDevice Halting...");
    while (1);
  }
  else if( rtc.readSqwPinMode() != SquareWave1HZ) {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("RTC FOUND!");
    lcd.setCursor(0,1);
    lcd.print("Pulse: 1 Hz");
    Serial.println("RTC FOUND!\nPulse MODE : 1 Hz ");
    rtc.writeSqwPinMode(SquareWave1HZ);
  }
  else if (autoSetTime) {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("RTC FOUND!");
    lcd.setCursor(0,1);
    lcd.print("Pulse: 1 Hz");
    delay(1000);
    lcd.setCursor(0,2);
    lcd.print("Time set: Auto");
    Serial.println("RTC OK!\nPulse mode : 1 Hz\nTime Auto Set : ON");
     
     rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }  
  else {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("RTC OK!");
    lcd.setCursor(0,1);
    lcd.print("Pulse: 1 Hz");
    delay(1000);
    lcd.setCursor(0,2);
    lcd.print("Time OK");
    Serial.println("RTC OK!\nTIME OK!\nPulse mode : 1 Hz");
  }
}

void setup() {
  // put your setup code here, to run once:
  while (!Serial); // for Leonardo/Micro/Zero
  Serial.begin(SER_BAUD);
  pinMode(BlPin, OUTPUT);
  lcd.begin(16,2);
  digitalWrite(BlPin, HIGH);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Hello");
  lcd.setCursor(0,1);
  lcd.print("Batman");
  delay(1000);
  
  pinMode(wakePin, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);
  pinMode(switchPin, INPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);

  Serial.println("Initializing...");
  lcd.clear();
  lcd.print("Initializing...");
  delay(1000);
  initPeripherals(SquareWave1HZ, true);
  //delay(2000);
  wakeNote();
  
  readData(); // reads data fom file and initializes the struct(s)
  //timeSort(); // sorts the wakeUpTime
  readTime(); // reads time from RTC
  wakeUpTime = nowTime;
  Serial.println("\nWake UP: ");
  Serial.println(wakeUpTime);
  delay(1000);
  digitalWrite(BlPin, LOW);
  if(nowTime == wakeUpTime) {
    handler(); // handles operations related to checking and alarm
  }
   else {
    Serial.println("Setup to sleep");
    delay(1000);
    sleep(); // initiates sleep mode and wakes up
   }
}

void loop() {
  // put your main code here, to run repeatedly:
  if(count == 50){
    count= 0;
    readTime();
    delay(500);
  }
  if(nowTime != wakeUpTime) {
    sleep();
  } else {
      wakeNote(); // indicates that the system woke up
      digitalWrite(ledPin, HIGH);
      delay(1000);
  
//    while(med[i].medName != NULL) { 
//      Serial.println(med[i].medName);
//      j = 0;
//      while( med[i].tymAndPill[j].tym) {
//        Serial.println( med[i].tymAndPill[j].tym);
//        Serial.println( med[i].tymAndPill[j].pills);
//        j++;
//      }
//      Serial.print("Quantity: ");
//      Serial.println(med[i].quantity);
//      i++;
//    }
    digitalWrite(ledPin, LOW);
    delay(1000);
    handler();
  }
}



// Reads data and initializes the structure
void readData() {
  int i = 0;
  DynamicJsonBuffer jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(json);
  
  for (auto kv: root){ // iterator for the medicine name
    med[i].medName=kv.key;  // parsed medicine name
    
    JsonArray& tymPill = kv.value; // parsed information array of the "medicine"
    
    int arraySize= tymPill.size();
    
    for(int p=0;  p < arraySize ; p++) {
      JsonObject& tym = tymPill[p]; //object within the array
      for(auto tp:tym){ // iterator for array-object
        if(p == arraySize - 1) {
          med[i].quantity = tp.value.as<int>(); // initializes the inventory quantity
          break;
        }
        String temptym = tp.key; // parsed the medication time
        med[i].tymAndPill[p].tym = temptym.toInt(); // converts the string into time
        med[i].tymAndPill[p].pills = tp.value.as<int>(); // parsed the number of the pills to take
      }
    }
    led[i][0] = pinStart;
    pinStart++;
    i++;
  }
  return;
}



// sorts the next wakeUpTime
void timeSort() {
  bool nextDay = true;
  int tempTime = 2400;

  for(int i=0; med[i].medName != NULL; i++) {
    for(int j = 0; med[i].tymAndPill[j].tym; j++) {
      if(med[i].tymAndPill[j].tym <= wakeUpTime) {
        continue;
      }
       else {
        if(med[i].tymAndPill[j].tym <= tempTime) {
          tempTime = med[i].tymAndPill[j].tym;
          nextDay = false;
        }
      }
    }
  }
  
  
  if(nextDay == true) {
    wakeUpTime = 0;
    timeSort();
  } else {
    wakeUpTime = tempTime;
    Serial.print("Next wake up: ");
    Serial.println(wakeUpTime);
  }
}


// handles the LCD and alarm outputs
void handler(){
  
  String temp[11];
  temp[0] = "Wake Up: "+String(wakeUpTime);
  bool ringAlarm = false;
  for(int i=0; med[i].medName != NULL; i++) {
    for(int j = 0; med[i].tymAndPill[j].tym; j++) {
      if(med[i].tymAndPill[j].tym == wakeUpTime) {
        ringAlarm = true;
        if(med[i].quantity > 0) {
          temp[i+1] = med[i].medName+" "+med[i].tymAndPill[j].pills;
          med[i].quantity -= med[i].tymAndPill[j].pills;
          if(med[i].quantity < 0){
            med[i].quantity = 0;
          }
        } else {
          temp[i+1] += med[i].medName+" "+med[i].tymAndPill[j].pills+" None left";
        }
        led[i][1] = 1;
      }
    }
  }
  Serial.println();
  Serial.print(wakeUpTime);
  Serial.println(" Hour(s)");
  for(int i=0; temp[i]!= NULL; i++) {
    Serial.println(temp[i]);
  }
  
  if(ringAlarm){
    alarmDisplay(1, temp);
  }
  timeSort();
//  delay(3000);
  check();
  return;
}




//
void alarmDisplay(int alarm, String temp[10]){
  // keep ringing the alarm until "ok" button is pressed
  // Display the message in the LCD
  // keep rotating the stream until ok button is pressed
  // after pressing the ok button clear the screen and return
  bool buttonPress = false;
  int i;
  digitalWrite(BlPin, HIGH);
  while(!buttonPress){
      for(int i = 0; led[i][0]; i++){
        if(led[i][1] == 1) {
          digitalWrite(led[i][0], HIGH);
        }
      }
      for(int thisNote = 0,i = 0; temp[i]!= NULL && thisNote < alarm; i+=2,thisNote++){
        int noteDuration = 1000 / noteDurations[1];
        tone(9, melody[1], noteDuration);
        int pauseBetweenNotes = noteDuration * 1.30;
        delay(pauseBetweenNotes);
        noTone(9);
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print(temp[i]);
        delay(250);
        lcd.setCursor(0,1);
        lcd.print(temp[i+1]);
        delay(250);
        lcd.setCursor(0,2);
        lcd.print(temp[i+2]);
        delay(250);
      }
      buttonPress = digitalRead(switchPin);
  }
  lcd.clear();
  for(int i = 0; led[i][0]; i++){
    if(led[i][1] == 1) {
      digitalWrite(led[i][0], LOW);
      led[i][1] = 0;
    }
  }
  digitalWrite(BlPin, LOW);
  return;
}




// reads the current time from RTC
void readTime(){
  DateTime now = rtc.now();
  nowTime = 0;
  nowTime = now.hour();
  nowTime *= 100;
  nowTime += now.minute();
  Serial.println("\nNow: ");
  Serial.println(nowTime);
  delay(1000);
  //Serial.println();
}

// Initiates sleep mode and wakes up the arduino
void sleep(){
  count++;
//  Serial.println("Entering sleep function...");
//  delay(1000);
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);   // sleep mode is set here  
//  Serial.println("Sleep mode selected...");
//  delay(1000);
  sleep_enable();          // enables the sleep bit in the mcucr register  
//  Serial.println("sleep enabled...");
//  delay(1000);
  //readTime();
  attachInterrupt(digitalPinToInterrupt(wakePin), wakeUpNow, RISING); // use interrupt 0 (pin 2) and run function 
//  Serial.println("Going to sleep...");
//  delay(1000);
  sleep_mode();            // here the device is actually put to sleep!!  
  
  // THE PROGRAM CONTINUES FROM HERE AFTER WAKING UP 
  sleep_disable();
  detachInterrupt(digitalPinToInterrupt(wakePin)); 
  loop();
}

void wakeUpNow() {  
}


//checks the quantity of the inventory and gives alarm if the storage is low
void check(){
  lcd.clear();
  for(int i=0; med[i].medName != NULL; i++) {
    if(med[i].quantity <= 3) {
      Serial.println();
      String temp[10] = "";
      temp[0] = "Warning";
      temp[1] = "Low Storage";
      temp[2]= med[i].medName+" left "+med[i].quantity;
      
      alarmDisplay(2, temp);
      //Serial.println(temp1);
      delay(1000);
    }
  }
  return;
}


void wakeNote(){
  // open note
  for (int thisNote = 0; thisNote < 8; thisNote++) {
    // to calculate the note duration, take one second
    // divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int noteDuration = 1000 / noteDurations[thisNote];
    tone(9, melody[thisNote], noteDuration);

    // to distinguish the notes, set a minimum time between them.
    // the note's duration + 30% seems to work well:
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    // stop the tone playing:
    noTone(9);
  }
  return;
}

