#ifndef __msgeq7__
#define __msgeq7__

#define resetPulseWidth 100
#define strobePulseWidth 18
#define resetToStrobeDelay 72
#define outputSettlingTime 36
#define strobeToStrobe 72
#define numberOfFrequencies 7

#include <Arduino.h>

class msgeq7
{
private:
      byte msg7AnalogInputPort;
      byte msg7ResetPort;
      byte msg7StrobePort;
      boolean isInitialized;
      int frequencyValue[numberOfFrequencies];

      void reset();
      void measureFrequency(byte frequencyNumber);
      void chanceFrequency();

public:
    msgeq7(byte strobePort ,byte resetPort,byte inputPort);

    void readFrequencies();
    int getFrequency(byte frequencyNumber);
};
#endif
