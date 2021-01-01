#ifndef SIMPLE_MODE_H
#define SIMPLE_MODE_H

#include <pixeltypes.h>
#include "ColoredMode.h"

class SimpleMode : public ColoredMode
{
  public:
  
  SimpleMode(CRGB color, int startId = 0, int count = LED_COUNT) : ColoredMode(color, startId, count)
  {
  }
  SimpleMode(const std::map<String, String>* args, int startId = 0, int count = LED_COUNT) : ColoredMode(args, startId, count)
  {
  }

  virtual CRGB GetPixel(int id) const override
  {
    return GetColor(0);
  }
};

#endif // SIMPLE_MODE_H
