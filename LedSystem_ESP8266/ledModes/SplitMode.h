#ifndef SPLIT_MODE
#define SPLIT_MODE

#include <ESPAsyncWebServer.h>
#include <pixeltypes.h>
#include "ColoredMode.h"

class SplitMode : public ColoredMode
{
  protected:
  int currId = 0;
  public:

  SplitMode(CRGB color1, CRGB color2=CRGB::Black, int startId = 0, int count = LED_COUNT) : ColoredMode(std::vector<CRGB>{color1, color2}, startId, count)
  {
  }
  SplitMode(AsyncWebServerRequest *request, int startId = 0, int count = LED_COUNT) : ColoredMode(request, startId, count)
  {
	  this->Update(request);
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
	  currId=0;
    return *this;
    
  }
  virtual LedMode& operator--() override
  {
    currId--;
	if(currId<0)
	  currId=count;
    return *this;
  }
  
  virtual CRGB GetPixel(int id) const override
  {
    return GetColor(id<currId?0:1);
  }
};

#endif // SPLIT_MODE
