#ifndef RAINBOW_MODE_H
#define RAINBOW_MODE_H

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
  
  
  static CHSV GetRainbow(int startHue, int startId = 0, int count = 1, int id = 0)
  {
    return CHSV(((int)(startHue + 255.0 * (id - startId) / (count))) % 256, 255, 255);
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
    return RainbowMode::GetRainbow(hue, startId, count, id);
  }
};

#endif // RAINBOW_MODE_H
