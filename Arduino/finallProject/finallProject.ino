#include <ArduinoJson.h>
#include <SPI.h>
#include <SD.h>

File myFile;
int csPin = 10;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("Initializing card...");
  pinMode(csPin, OUTPUT);
  digitalWrite(csPin, HIGH);
  
//  timeSort();
//  if(now == wakeUpTime) {
//    work();
//  }
//  else(
//    sleep();}
}

void loop() {
  // put your main code here, to run repeatedly:
  readFile();
  //sleep();
}


// reads the file from SD card
void readFile() {
  if (!SD.begin(4)) {
    Serial.println(SD.begin(4));
    return;
  }
  Serial.println("initialization done.");
  
  // open the file for reading:
  myFile = SD.open("medicine.txt");
  if (myFile) {
    Serial.println("medicine.txt:");

    // read from the file until there's nothing else in it:
    while (myFile.available()) {
      Serial.write(myFile.read());
    }
    // close the file:
    myFile.close();
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening medicine.txt");
  }

  timeSort();
}


// Sorts the time and initialzes the next wake up itme
void timeSort(){
  
}


// calcultes the data and sets the output
void work(){
  
}


// 

