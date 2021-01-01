#ifndef COLORED_MODE_H
#define COLORED_MODE_H

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
  ColoredMode(const std::map<String, String>* args, int startId = 0, int count = LED_COUNT) : LedMode(startId, count)
  {
    this->Update(args);
  }

  virtual void Update(const std::map<String, String>* args) override
  {
	for(auto it = args->begin(); it != args->end(); it++)
	{
		String key = it->first;
		if(key.length() >= 5 && key.substring(0, 5) == "color")
		{
			int num = key.length() > 5 ? key.substring(5).toInt() : 0;
			
			while(num >= colors.size())
			{
				colors.push_back(CRGB::Black);
			}
			colors[num] = CRGB(htmlColorToInt(it->second));
		}
	}
  }
  
  
  int GetColorsCount() const { return colors.size(); }
  CRGB GetColor(int id) const { return colors[id]; }
  void SetColor(int id, CRGB value)
  {
	if(id>=colors.size())
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



#endif // COLORED_MODE_H
