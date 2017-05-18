#include "msgeq7.h"

msgeq7::msgeq7(byte strobePort ,byte resetPort,byte inputPort)
{
  this->msg7StrobePort = strobePort;
  this->msg7ResetPort = resetPort;
  this->msg7AnalogInputPort = inputPort;
  this->isInitialized = false;
  this->isFiltered = false;
  this->numberOfFrequencies = 7;
  this->currentlyFilterProgramm = 0;
  this->diffValueForExpFloatAvg = 0;
  this->index = 0;


  pinMode(msg7ResetPort, OUTPUT);
  pinMode(msg7StrobePort, OUTPUT);
  pinMode(msg7AnalogInputPort, INPUT);

  for (size_t j = 0; j < floatingAverageLenght; j++)
  {
    for (size_t i = 0; i < numberOfFrequencies; i++)
    {
      frequencyValue[j][i] = 0;
    }
    this->diffValueForExpFloatAvg += floatingAverageLenght-j;
  }
}

int msgeq7::getMaxFrequency()
{
  int tmp =0;
  for (size_t i = 0; i < numberOfFrequencies; i++)
  {
    tmp = max(frequencyValue[index][i],tmp);
  }
  return tmp;
}

int msgeq7::getMinFrequency()
{
  int tmp=maxFrequencyValue;
  for (size_t i = 0; i < numberOfFrequencies; i++)
  {
    tmp = min(frequencyValue[index][i],tmp);
  }
  return tmp;
}


byte msgeq7::getNumberOfFrequencies()
{
  return numberOfFrequencies;
}

void msgeq7::setFilterProgramm(byte number)
{
  currentlyFilterProgramm = number;
}

void msgeq7::setNumberOfFrequencies(byte newCount)
{
  if(newCount <= maxNumberOfFrequencies && newCount >= minNumberOfFrequencies)
    numberOfFrequencies = newCount;
}

void msgeq7::readFrequencies()
{
  isFiltered = false;
  if(currentlyFilterProgramm != 0)
    index = (index+1) % floatingAverageLenght;

  reset();
  for (int i = 0; i < numberOfFrequencies; i++)
  {
    measureFrequency(i);
    chanceFrequency();
  }
  filter();

  if(!isInitialized)
    isInitialized = true;
}

int msgeq7::getFrequency(byte frequencyNumber)
{
  if(isInitialized)
  {
    return frequencyValue[index][frequencyNumber];
  }
  else
  {
    return 0;
  }
}

double msgeq7::getFrequencyVoltage(byte frequencyNumber)
{
  return (double)frequencyValue[index][frequencyNumber] * 5 / 1023;
}

int msgeq7::getVolume()
{
  int vol = frequencyValue[index][0];
  for(int i = 1; i< numberOfFrequencies;i++)
    vol += frequencyValue[index][i];

  return vol/numberOfFrequencies;
}

void msgeq7::filter()
{

/*
  for (int i = 0; i < numberOfFrequencies; i++)
  {
    if (frequencyValue[index][i] < minFrequencyValue)
      frequencyValue[index][i] = 0;

    if (frequencyValue[index][i] > maxFrequencyValue)
      frequencyValue[index][i] = maxFrequencyValue;
  }
*/
    switch(currentlyFilterProgramm)
    {
        case 1: //exponential floating average
          for(int i = 0; i< numberOfFrequencies;i++)
          {
            int tmp =0;
            for (size_t j = 1; j < floatingAverageLenght; j++)
            {
             tmp += frequencyValue[(index-j)%floatingAverageLenght][i]*(floatingAverageLenght-j);
            }
            frequencyValue[index][i] = (tmp+ (frequencyValue[index][i]*floatingAverageLenght))/diffValueForExpFloatAvg;
          }
        break;
        default: //Programm Zero, don't manipulate the values
        break;

    }

    for (int i = 0; i < numberOfFrequencies; i++)
    {
      if (frequencyValue[index][i] < minFrequencyValue)
        frequencyValue[index][i] = map(frequencyValue[index][i],0,150,0,60);

        if(frequencyValue[index][i] < 20 || getVolume() < 50)
            frequencyValue[index][i] = 0;
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
  frequencyValue[index][frequencyNumber] = analogRead(msg7AnalogInputPort);;
}

void msgeq7::chanceFrequency()
{
  digitalWrite (msg7StrobePort, HIGH);
  delayMicroseconds(resetToStrobeDelay);
  digitalWrite (msg7StrobePort, LOW);
}
