#ifndef GROW_MODE_H
#define GROW_MODE_H

#include <ESPAsyncWebServer.h>
#include <pixeltypes.h>
#include <algorithm>
#include "SplitMode.h"

class GrowMode : public SplitMode
{

  public:

  GrowMode(CRGB color0, CRGB color1=CRGB::Black, int startId = 0, int count = LED_COUNT) : SplitMode(color0, color1, startId, count)
  {
  }
  GrowMode(AsyncWebServerRequest *request, int startId = 0, int count = LED_COUNT) : SplitMode(request, startId, count)
  {
  }
  
  virtual LedMode& operator++() override
  {
	SplitMode::operator++();
	if(currId==0)
	{
	  std::swap(colors[0], colors[1]);
	}
    return *this;
    
  }
  virtual LedMode& operator--() override
  {
    SplitMode::operator--();
	if(currId==count)
	{
	  std::swap(colors[0], colors[1]);
	}
    return *this;
  }
};

#endif // GROW_MODEH
