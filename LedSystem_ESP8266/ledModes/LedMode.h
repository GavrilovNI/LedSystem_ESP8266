#ifndef LED_MODE_H
#define LED_MODE_H

#include <ESPAsyncWebServer.h>
#include <pixeltypes.h>
#include "../leds.h"
#include "../ledMasks/LedMask.h"
#include "../ledMasks/FullMask.h"

class LedMode
{
  protected:
  int startId;
  int count;

  LedMode(int startId = 0, int count = LED_COUNT)
  {
    this->startId = startId;
    this->count = count;
  }
  
  public:
  bool swaped = false;
  
  int GetStartId() { return startId; }
  int GetCount() { return count; }
  
  int GetEndId() const { return startId + count; }

  virtual void Update(AsyncWebServerRequest *request) { }

  /*void Draw(Leds* leds, LedMask* mask) const
  {
	int endId = GetEndId();
	
	if(swaped)
	{
	  for(int i=0, id=startId; i < count; i++, id++)
      {
		if(!mask->IsUnderMask(id))
		  continue;
        (*leds)[id] = GetPixel(endId-i-1);
      }
	}
	else
	{
	  for(int i=startId; i < endId; i++)
      {
		if(!mask->IsUnderMask(i))
		  continue;
        (*leds)[i] = GetPixel(i);
      }
	}	
  }*/
  
  void Draw(Leds* leds, LedMask* mask) const
  {
	int endId = GetEndId();
	
	
	if(swaped)
	{
	  for(int i=0, id=startId; i < count; i++, id++)
      {
        (*leds)[id] = GetPixel(endId-i-1).fadeToBlackBy(255 - mask->GetMask(id));
      }
	}
	else
	{
	  for(int i=startId; i < endId; i++)
      {
        (*leds)[i] = GetPixel(i).fadeToBlackBy(255 - mask->GetMask(i));
      }
	}	
  }
  
  virtual void Draw(Leds* leds) const
  {
	FullMask mask(startId, count);
	Draw(leds, &mask);
  }
  
  virtual CRGB GetPixel(int id) const = 0;

  virtual LedMode& operator+= (const float& value) {}
};



#endif // LED_MODE_H
