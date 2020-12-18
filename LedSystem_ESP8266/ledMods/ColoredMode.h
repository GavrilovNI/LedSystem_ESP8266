#ifndef COLORED_MODE
#define COLORED_MODE

#include <ESPAsyncWebServer.h>
#include <pixeltypes.h>
#include "LedMode.h"


class ColoredMode: public LedMode
{
  protected:
  CRGB color;

  public:
  ColoredMode(CRGB color, int startId = 0, int count = LED_COUNT) : LedMode(startId, count)
  {
    this->color=color;
  }
  ColoredMode(AsyncWebServerRequest *request, int startId = 0, int count = LED_COUNT) : LedMode(startId, count)
  {
    this->Update(request);
  }

  virtual void Update(AsyncWebServerRequest *request) override
  {
    if (request->hasParam("color"))
    {
      color = CRGB(htmlColorToInt(request->getParam("color")->value()));
    }
  }
  
  virtual bool HasColor() const override { return true; }
  
  CRGB GetColor() const { return color; }
  void SetColor(CRGB value) { color = value; }
};



#endif // COLORED_MODE
