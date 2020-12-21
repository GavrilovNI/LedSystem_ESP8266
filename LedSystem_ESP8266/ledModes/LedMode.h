#ifndef LED_MODE_H
#define LED_MODE_H

#include <ESPAsyncWebServer.h>
#include <pixeltypes.h>
#include "../leds.h"

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

  virtual void Draw(Leds* leds) const
  {
	int endId = GetEndId();
	
    /*for(int i=startId; i < endId; i++)
    {
      (*leds)[i] = GetPixel(swaped ? endId-(i-startId)-1 : i);
    }*/
	
	if(swaped)
	{
	  for(int i=0, id=startId; i < count; i++, id++)
      {
        (*leds)[id] = GetPixel(endId-i-1);
      }
	}
	else
	{
	  for(int i=startId; i < endId; i++)
      {
        (*leds)[i] = GetPixel(i);
      }
	}	
  }
  virtual CRGB GetPixel(int id) const = 0;

  virtual LedMode& operator++() {}
  virtual LedMode& operator--() {};
};



#endif // LED_MODE_H
