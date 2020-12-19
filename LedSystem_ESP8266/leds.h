#ifndef LEDS_H
#define LEDS_H

#define FASTLED_ESP8266_RAW_PIN_ORDER
//#define FASTLED_ESP8266_NODEMCU_PIN_ORDER
//#define FASTLED_ESP8266_D1_PIN_ORDER

#define FASTLED_ALL_PINS_HARDWARE_SPI
#define ESP8266_SPI

#include <FastLED.h>
#include <ESPAsyncWebServer.h>

#include "utils.h"

#define LED_COUNT 120
#define LED_DT 3
#define DEFAULT_BRIGHTNESS 50

CHSV getRainbow(int startHue, int maxCount = 1, int num = 0)
{
  return CHSV(((int)(startHue + 255.0 * num / maxCount)) % 256, 255, 255);
}

class Leds
{
  protected:
  CRGB* leds;
  int count;
  
  public:
  Leds(int count = LED_COUNT)
  {
    this->count = count;
    leds = new CRGB[count];
    FastLED.addLeds<WS2811, LED_DT, GRB>(leds, count);
    FastLED.setBrightness(DEFAULT_BRIGHTNESS);
    this->fill(CRGB::Black);
    this->Show();
  }
  ~Leds()
  {
    delete[] leds;
  }

  CRGB& operator[](int index) const
  {
    return leds[index];
  } 

  void fill(CRGB color, int startId, int count)
  {
    for (int i = startId ; i < startId + count; i++ )
    {
      this->leds[i] = color;
    }
  }
  void fill(CRGB color)
  {
    fill(color, 0, count);
  }

  void Show() const
  {
    FastLED.show();
  }
};




#endif // LEDS_H
