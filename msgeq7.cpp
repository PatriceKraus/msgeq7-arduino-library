#include "msgeq7.h"

msgeq7::msgeq7(byte strobePort ,byte resetPort,byte inputPort)
{
  this->msg7StrobePort = strobePort;
  this->msg7ResetPort = resetPort;
  this->msg7AnalogInputPort = inputPort;
  this->isInitialized = false;
  this->isFiltered = false;
  this->numberOfFrequencies = 7;

  pinMode(msg7ResetPort, OUTPUT);
  pinMode(msg7StrobePort, OUTPUT);
  pinMode(msg7AnalogInputPort, INPUT);

  for (size_t i = 0; i < numberOfFrequencies; i++)
  {
    frequencyValue[i] = 0;
  }
}

byte msgeq7::getNumberOfFrequencies()
{
  return numberOfFrequencies;
}

void msgeq7::setNumberOfFrequencies(byte newCount)
{
  if(newCount <= maxNumberOfFrequencies && newCount >= minNumberOfFrequencies)
    numberOfFrequencies = newCount;
}

void msgeq7::readFrequencies()
{
  reset();
  for (int i = 0; i < numberOfFrequencies; i++)
  {
    measureFrequency(i);
    chanceFrequency();
  }

  if(!isInitialized)
    isInitialized = true;

  isFiltered = false;
}

int msgeq7::getFrequency(byte frequencyNumber)
{
  if(isInitialized)
  {
    return frequencyValue[frequencyNumber];
  }
  else
  {
    return 0;
  }
}

double msgeq7::getFrequencyVoltage(byte frequencyNumber)
{
  return (double)frequencyValue[frequencyNumber] * 5 / 1023;
}

int msgeq7::getVolume()
{
  int vol = frequencyValue[0];
  for(int i = 1; i< numberOfFrequencies;i++)
    vol += frequencyValue[i];

  return vol/numberOfFrequencies;
}

void msgeq7::filter()
{
  int maxValue = frequencyValue[0];
  int minValue = frequencyValue[0];
  for (int i = 1; i < numberOfFrequencies; i++)
  {
    maxValue = max(maxValue, frequencyValue[i]);
    minValue = min(minValue, frequencyValue[i]);
  }

  if (minValue < minFrequencyValue || maxValue > maxFrequencyValue)
  {
    for (int i = 0; i < numberOfFrequencies; i++)
    {
      if (frequencyValue[i] < minFrequencyValue)
        frequencyValue[i] = 0;

      if (frequencyValue[i] > maxFrequencyValue)
        frequencyValue[i] = maxFrequencyValue;
    }
  }
  isFiltered = true;
}

void msgeq7::reset()
{
  digitalWrite (msg7ResetPort,  HIGH);
  digitalWrite (msg7StrobePort, HIGH);
  delayMicroseconds(resetPulseWidth / 2);
  digitalWrite (msg7StrobePort, LOW);
  delayMicroseconds(resetPulseWidth / 2);
  digitalWrite (msg7ResetPort,  LOW);
  digitalWrite (msg7StrobePort, HIGH);
  delayMicroseconds(resetToStrobeDelay);
}

void msgeq7::measureFrequency(byte frequencyNumber)
{
  digitalWrite (msg7StrobePort, LOW);
  delayMicroseconds(outputSettlingTime * 2);
  frequencyValue[frequencyNumber] = analogRead(msg7AnalogInputPort);;
}

void msgeq7::chanceFrequency()
{
  digitalWrite (msg7StrobePort, HIGH);
  delayMicroseconds(resetToStrobeDelay);
  digitalWrite (msg7StrobePort, LOW);
}
