# msgeq7-arduino-library
A minimalistic  MSGEQ7 spectrum analyzer library. Read the frequencies of the MSGEQ7-IC with your arduino.

Installation Guide:
Download the zip file and unzip it in your arduino library directory. 
Then rename the directory from "msgeq7lib-master" to "msgeq7lib" and have fun.

Code Example:
```cpp
#include <msgeq7.h>
#include <SoftwareSerial.h>

#define strobe 7
#define reset 8
#define input A4

//init
msgeq7 myMsgeq7 = msgeq7(strobe,reset,input);

void setup()
{
  Serial.begin(9600);
}

void loop()
{
  //first read the currently frequencies from the IC
  myMsgeq7.readFrequencies();


  //get One Frequency
  int aFrequency = myMsgeq7.getFrequency(0); //65Hz

  /*
  int aFrequency = myMsgeq7.getFrequency(1);// 160Hz
  int aFrequency = myMsgeq7.getFrequency(2);// 400Hz
  int aFrequency = myMsgeq7.getFrequency(3);// 1kHz
  int aFrequency = myMsgeq7.getFrequency(4);// 2.5kHz
  int aFrequency = myMsgeq7.getFrequency(5);// 6.2kHz
  int aFrequency = myMsgeq7.getFrequency(6);// 16kHz
  */

  //Read all Frequencies
  for(int i = 0; i < 7; i++)
  {
    int value = myMsgeq7.getFrequency(i);
    Serial.print(String(value)+"\t");
  }
  Serial.println(" ");
  delay(50);
}
