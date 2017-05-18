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
  myMsgeq7.setFilterProgramm(1); // Use Filter if you want. At the moment, just one Filter.
  //myMsgeq7.setFilterProgramm(0); // disable Filter
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
