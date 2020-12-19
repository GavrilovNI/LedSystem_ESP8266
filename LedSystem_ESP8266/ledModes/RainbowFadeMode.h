#ifndef RAINBOW_FADE_MODE_H
#define RAINBOW_FADE_MODE_H

#include <ESPAsyncWebServer.h>
#include <pixeltypes.h>
#include "LedMode.h"

class RainbowFadeMode : public RainbowMode
{
  public:

  RainbowFadeMode(int startId = 0, int count = LED_COUNT) : RainbowMode(startId, count)
  {
  }
  
  virtual CRGB GetPixel(int id) const override
  {
    return getRainbow(hue);
  }
};

#endif // RAINBOW_FADE_MODE_H
