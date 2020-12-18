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

int max_bright = 51;

struct CRGB leds[LED_COUNT];

void fill(CRGB color, int startId = 0, int count = LED_COUNT)
{
  for (int i = startId ; i < startId + LED_COUNT; i++ )
  {
    leds[i] = color;
  }
}

CHSV getRainbow(int startHue, int maxCount = 1, int num = 0)
{
  return CHSV(((int)(startHue + 255.0 * num / maxCount)) % 256, 255, 255);
}


void setupLeds()
{
  LEDS.setBrightness(max_bright);
  LEDS.addLeds<WS2811, LED_DT, GRB>(leds, LED_COUNT);
  fill(CRGB::HTMLColorCode::Black);
  LEDS.show();
}


#endif // LEDS_H
