#ifndef LED_MODE
#define LED_MODE

#include <ESPAsyncWebServer.h>
#include <pixeltypes.h>

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

  virtual bool HasColor() const { return false; }

  virtual void Draw() const
  {
    for(int i=startId; i < count; i++)
    {
      leds[i] = GetPixel(i);
    }
  }
  virtual CRGB GetPixel(int id) const = 0;

  virtual LedMode& operator++() {}
  virtual LedMode& operator--() {};
};



#endif // LED_MODE
