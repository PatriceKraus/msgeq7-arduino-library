#ifndef __msgeq7__
#define __msgeq7__

#define resetPulseWidth 100
#define strobePulseWidth 18
#define resetToStrobeDelay 72
#define outputSettlingTime 36
#define strobeToStrobe 72

#define minFrequencyValue 120
#define maxFrequencyValue 1023
#define maxNumberOfFrequencies 7
#define minNumberOfFrequencies 1

#include <Arduino.h>

class msgeq7
{
private:
      byte numberOfFrequencies;
      byte msg7AnalogInputPort;
      byte msg7ResetPort;
      byte msg7StrobePort;
      boolean isInitialized;
      boolean isFiltered;
      int frequencyValue[maxNumberOfFrequencies];


      void reset();
      void measureFrequency(byte frequencyNumber);
      void chanceFrequency();

public:
    msgeq7(byte strobePort ,byte resetPort,byte inputPort);

    void readFrequencies();
    void filter();
    int getFrequency(byte frequencyNumber);
    int getFrequency(String frequency);
    double getFrequencyVoltage(byte frequencyNumber);
    int getVolume();
    byte getNumberOfFrequencies();
    void setNumberOfFrequencies(byte newCount);

};
#endif
