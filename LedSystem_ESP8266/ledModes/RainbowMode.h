#ifndef RAINBOW_MODE_H
#define RAINBOW_MODE_H

#include <pixeltypes.h>
#include <cmath>
#include "LedMode.h"

class RainbowMode : public LedMode
{
  protected:
  float hue=0;
  
  public:

  RainbowMode(int startId = 0, int count = LED_COUNT) : LedMode(startId, count)
  {
  }
  
  
  static CHSV GetRainbow(float startHue, int startId = 0, int count = 1, int id = 0)
  {
    return CHSV(((int)(startHue + 255.0 * (id - startId) / (count))) % 256, 255, 255);
  }
  
  
  virtual LedMode& operator+= (const float& value) override
  {
	hue = fmod(hue - value, 256);
	if(hue < 0)
		hue = 256 + hue;
  }

  virtual CRGB GetPixel(int id) const override
  {
    return RainbowMode::GetRainbow(hue, startId, count, id);
  }
};

#endif // RAINBOW_MODE_H
