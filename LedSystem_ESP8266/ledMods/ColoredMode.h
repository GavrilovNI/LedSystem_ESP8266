#ifndef COLORED_MODE
#define COLORED_MODE

#include <ESPAsyncWebServer.h>
#include <pixeltypes.h>
#include <vector>
#include "LedMode.h"


class ColoredMode: public LedMode
{
  protected:
  std::vector<CRGB> colors;

  public:
  ColoredMode(CRGB color, int startId = 0, int count = LED_COUNT) : LedMode(startId, count)
  {
    colors.push_back(color);
  }
  ColoredMode(std::vector<CRGB> colors, int startId = 0, int count = LED_COUNT) : LedMode(startId, count)
  {
    this->colors = colors;
  }
  ColoredMode(AsyncWebServerRequest *request, int startId = 0, int count = LED_COUNT) : LedMode(startId, count)
  {
    this->Update(request);
  }

  virtual void Update(AsyncWebServerRequest *request) override
  {
	colors.clear();
    if (request->hasParam("color"))
    {
      colors.push_back(CRGB(htmlColorToInt(request->getParam("color")->value())));
    }
	else if (request->hasParam("color0"))
    {
      colors.push_back(CRGB(htmlColorToInt(request->getParam("color0")->value())));
	  
	  
	  int num = 1;
	  String paramName = "color"+String(num);
	  while(request->hasParam(paramName))
	  {
	    colors.push_back(CRGB(htmlColorToInt(request->getParam(paramName)->value())));
		paramName = "color"+String(++num);
	  }
    }
	
  }
  
  
  int GetColorsCount() const { return colors.size(); }
  CRGB GetColor(int id) const { return colors[id]; }
  void SetColor(int id, CRGB value)
  {
	if(id>colors.size())
	  colors.push_back(value);
	else
	  colors[id] = value;
  }
  void AddColor(CRGB value)
  {
	colors.push_back(value);
  }
  void RemoveColor(int id)
  {
	colors.erase(colors.begin()+id);
  }
  void ClearColors()
  {
	colors.clear();
  }
};



#endif // COLORED_MODE
