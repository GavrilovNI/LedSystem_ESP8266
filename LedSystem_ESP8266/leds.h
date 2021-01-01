#ifndef LEDS_H
#define LEDS_H

#define FASTLED_ESP8266_RAW_PIN_ORDER
//#define FASTLED_ESP8266_NODEMCU_PIN_ORDER
//#define FASTLED_ESP8266_D1_PIN_ORDER

#define FASTLED_ALL_PINS_HARDWARE_SPI
#define ESP8266_SPI

#include <FastLED.h>

#include "utils.h"

#define LED_COUNT 120
#define LED_DT 3
#define DEFAULT_BRIGHTNESS 50


class Leds
{
  protected:
  struct CRGB* _leds;
  int count;
  
  public:
  Leds(int count = LED_COUNT)
  {
    this->count = count;
    _leds = new CRGB[count];
    this->fill(CRGB::Black);
    LEDS.addLeds<WS2811, LED_DT, GRB>(_leds, count);
    LEDS.setBrightness(DEFAULT_BRIGHTNESS);
    this->Show();
  }
  ~Leds()
  {
    delete[] _leds;
  }

  CRGB& operator[](int index)
  {
    return this->_leds[index];
  } 

  int GetCount() const
  {
    return count;
  }

  void fill(CRGB color, int startId, int count)
  {
    for (int i = startId ; i < startId + count; i++ )
    {
      this->_leds[i] = color;
    }
  }
  void fill(CRGB color)
  {
    fill(color, 0, count);
  }

  void Show() const
  {
    LEDS.show();
  }
};




#endif // LEDS_H
