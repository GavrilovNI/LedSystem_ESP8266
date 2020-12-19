#ifndef RAINBOW_MODE
#define RAINBOW_MODE

#include <ESPAsyncWebServer.h>
#include <pixeltypes.h>
#include "LedMode.h"

class RainbowMode : public LedMode
{
  protected:
  int hue=0;
  
  public:

  RainbowMode(int startId = 0, int count = LED_COUNT) : LedMode(startId, count)
  {
  }
  
  virtual LedMode& operator++() override
  {
    hue=(hue+255)%256; //hue--
    return *this;
    
  }
  virtual LedMode& operator--() override
  {
    hue=(hue+1)%256; //hue++
    return *this;
  }

  virtual CRGB GetPixel(int id) const override
  {
    return getRainbow(hue, count, id);
  }
};

#endif // RAINBOW_MODE
