#ifndef GROW_N_BACK_MODE_H
#define GROW_N_BACK_MODE_H

#include <ESPAsyncWebServer.h>
#include <pixeltypes.h>
#include <algorithm>
#include "GrowMode.h"
#include "../utils.h"

class GrowNBackMode : public GrowMode
{
  protected:

  public:
  bool growing=true;

  GrowNBackMode(CRGB color0, CRGB color1=CRGB::Black, int startId = 0, int count = LED_COUNT) : GrowMode(color0, color1, startId, count)
  {
  }
  GrowNBackMode(AsyncWebServerRequest *request, int startId = 0, int count = LED_COUNT) : GrowMode(request, startId, count)
  {
  }
  
  virtual LedMode& operator++() override
  {
	int endId = GetEndId();
	  
	if(growing)
	{
	  if(currId >= endId)
	  {
		currId = endId-1;
		growing = !growing;
	  }
	  else
	  {
	    currId++;
	  }
	}
	else
	{
	  if(currId <= startId)
	  {
		currId = startId + 1;
		growing = !growing;
	  }
	  else
	  {
	    currId--;
	  }
	}
    return *this;
  }
  virtual LedMode& operator--() override
  {
	int endId = GetEndId();
	
    if(!growing)
	{
	  if(currId >= endId)
	  {
		currId = endId-1;
		growing = !growing;
	  }
	  else
	  {
	    currId++;
	  }
	}
	else
	{
	  if(currId <= startId)
	  {
		currId = startId + 1;
		growing = !growing;
	  }
	  else
	  {
	    currId--;
	  }
	}
    return *this;
  }
};

#endif // GROW_N_BACK_MODE_H
