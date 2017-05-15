#include "msgeq7.h"

msgeq7::msgeq7(byte strobePort ,byte resetPort,byte inputPort)
{
  this->msg7StrobePort = strobePort;
  this->msg7ResetPort = resetPort;
  this->msg7AnalogInputPort = inputPort;
  this->isInitialized = false;

  pinMode(msg7ResetPort, OUTPUT);
  pinMode(msg7StrobePort, OUTPUT);
  pinMode(msg7AnalogInputPort, INPUT);

  for (size_t i = 0; i < numberOfFrequencies; i++)
  {
    frequencyValue[i] = 0;
  }
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
