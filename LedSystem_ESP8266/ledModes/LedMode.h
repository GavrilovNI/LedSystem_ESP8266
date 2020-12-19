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

  virtual void Update(AsyncWebServerRequest *request) { }

  virtual void Draw(Leds* leds) const
  {
    for(int i=startId; i < count; i++)
    {
      (*leds)[i] = GetPixel(i);
    }
  }
  virtual CRGB GetPixel(int id) const = 0;

  virtual LedMode& operator++() {}
  virtual LedMode& operator--() {};
};



#endif // LED_MODE_H
