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

#define floatingAverageLenght 10

#include <Arduino.h>

class msgeq7
{
private:

  void reset();
  void measureFrequency(byte frequencyNumber);
  void chanceFrequency();
  void filter();

  void exponentialFloatingAverageFilter();
  void lowValueReductionFilter();
  void removeLowValuesAfterFilter();

  boolean isInitialized;
  boolean isFiltered;

  byte numberOfFrequencies;
  byte msg7AnalogInputPort;
  byte msg7ResetPort;
  byte msg7StrobePort;
  byte index;
  byte currentlyFilterProgramm;
  int diffValueForExpFloatAvg;
  int frequencyValue[floatingAverageLenght][maxNumberOfFrequencies];

  float intervalVerlaufNonlinearLED;


public:
    msgeq7(byte strobePort ,byte resetPort,byte inputPort);

    void readFrequencies();
    void setNumberOfFrequencies(byte newCount);
    void setFilterProgramm(byte number);

    byte getNumberOfFrequencies();

    int getFrequency(byte frequencyNumber);
    int getFrequency(String frequency);
    int getMaxFrequency();
    int getMinFrequency();
    int getVolume();

    double getRelativeFrequency(byte frequencyNumber);
    double getFrequencyVoltage(byte frequencyNumber);

};
#endif
