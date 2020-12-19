#ifndef SPLIT_MODE_H
#define SPLIT_MODE_H

#include <ESPAsyncWebServer.h>
#include <pixeltypes.h>
#include "ColoredMode.h"

class SplitMode : public ColoredMode
{
  protected:
  int currId;
  public:

  SplitMode(CRGB color1, CRGB color2=CRGB::Black, int startId = 0, int count = LED_COUNT) : ColoredMode(std::vector<CRGB>{color1, color2}, startId, count)
  {
	  currId = startId;
  }
  SplitMode(AsyncWebServerRequest *request, int startId = 0, int count = LED_COUNT) : ColoredMode(request, startId, count)
  {
	  currId = startId;
	  this->Update(request);
  }
  
  void SetCurrId(int id)
  {
    currId=id;
    if(currId<startId)
	  currId=startId;
	else if(currId>startId+count)
	  currId=startId+count;
  }
  
  virtual void Update(AsyncWebServerRequest *request) override
  {
	ColoredMode::Update(request);
	while(colors.size()<2)
	{
	  colors.push_back(CRGB::Black);
	}
  }
  
  virtual LedMode& operator++() override
  {
	currId++;
	if(currId>count)
	  currId=startId;
    return *this;
    
  }
  virtual LedMode& operator--() override
  {
    currId--;
	if(currId<startId)
	  currId=count;
    return *this;
  }
  
  virtual CRGB GetPixel(int id) const override
  {
    return GetColor(id<currId?0:1);
  }
};

#endif // SPLIT_MODE_H
