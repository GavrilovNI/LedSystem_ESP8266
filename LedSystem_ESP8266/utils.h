#ifndef UTILS_H
#define UTILS_H

#include <ESPAsyncWebServer.h>
#include <map>

std::map<String, String> getRequestArgs(AsyncWebServerRequest* request)
{
  std::map<String, String> result;
  for(size_t i = 0; i < request->params(); i++)
  {
    AsyncWebParameter* param = request -> getParam(i);
    result.insert(std::pair<String, String>(param->name(), param->value()));
  }

  return result;
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
