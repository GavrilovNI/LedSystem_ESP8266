#ifndef GROW_MODE
#define GROW_MODE

#include <ESPAsyncWebServer.h>
#include <pixeltypes.h>
#include "LedMode.h"
#include "ColoredMode.h"

class GrowMode : public ColoredMode
{
  protected:
  int currId = 0;
  bool growing=true;

  public:

  GrowMode(CRGB color, int startId = 0, int count = LED_COUNT) : ColoredMode(color, startId, count)
  {
  }
  GrowMode(AsyncWebServerRequest *request, int startId = 0, int count = LED_COUNT) : ColoredMode(request, startId, count)
  {
  }
  
  virtual LedMode& operator++() override
  {
    currId++;
    if(currId == count)
    {
      currId = 0;
      growing=!growing;
    }
    return *this;
    
  }
  virtual LedMode& operator--() override
  {
    currId--;
    if(currId == -1)
    {
      currId = count-1;
      growing=!growing;
    }
    return *this;
  }
  
  virtual CRGB GetPixel(int id) const override
  {
    if(growing)
      return id<=currId?color:CRGB::Black;
    else 
      return id<=currId?CRGB::Black:color;
  }
};

#endif // GROW_MODE
