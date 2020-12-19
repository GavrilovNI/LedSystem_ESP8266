#ifndef SIMPLE_MODE
#define SIMPLE_MODE

#include <ESPAsyncWebServer.h>
#include <pixeltypes.h>
#include "ColoredMode.h"

class SimpleMode : public ColoredMode
{
  public:
  
  SimpleMode(CRGB color, int startId = 0, int count = LED_COUNT) : ColoredMode(color, startId, count)
  {
  }
  SimpleMode(AsyncWebServerRequest *request, int startId = 0, int count = LED_COUNT) : ColoredMode(request, startId, count)
  {
  }

  virtual CRGB GetPixel(int id) const override
  {
    return GetColor(0);
  }
};

#endif // SIMPLE_MODE
