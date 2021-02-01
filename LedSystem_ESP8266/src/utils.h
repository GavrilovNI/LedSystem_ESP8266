#ifndef UTILS_H
#define UTILS_H

#include <Arduino.h>
#include <pixeltypes.h>
#include <map>

void printArgs(const std::map<String, String>* args)
{
  int i=0;
  Serial.print("Args(");
  Serial.print(args->size());
  Serial.println("):");
  for(auto it = args->begin(); it!=args->end(); it++, i++)
  {
    Serial.print(i);
    Serial.print(": ");
    Serial.print(it->first);
    Serial.print("=");
    Serial.println(it->second);
  }
}


int StrToInt(String value)
{
  if(value.length()>0 && value[0]=='-')
  {
    return -value.substring(1, value.length()).toInt();
  }
  return value.toInt();
}

int htmlColorToInt(String color)
{
  char charColor[9];
  String strColor = "0x"+color.substring(1, color.length());
  strColor.toCharArray(charColor, 9);
  return (int)strtol(charColor, NULL, 16);
}


String CRGB2Str(CRGB color)
{
  return "("+String(color.r)+", "+String(color.g)+", "+String(color.b)+")";
}

int clamp(int value, int _min, int _max)
{
  if(value < _min)
    return _min;
  if(value > _max)
    return _max;
  return value;
}


#endif // UTILS_H
