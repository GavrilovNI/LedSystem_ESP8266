#ifndef LAYER_MODE_H
#define LAYER_MODE_H

#include <vector>
#include "LedMode.h"

class LayerMode: public LedMode
{
	
  protected:
  std::vector<LedMode*> layers;
  
  public:

  LayerMode(int startId = 0, int count = LED_COUNT):LedMode(startId, count)
  {
    this->startId = startId;
    this->count = count;
  }
  
  int GetLayersCount()
  {
    return layers.size();
  }
  void AddLayer(LedMode* mode)
  {
    layers.push_back(mode);
  }

  void RemoveLayer(int id)
  {
    layers.erase(layers.begin()+id);
  }
  
  virtual void Update(AsyncWebServerRequest *request) override
  {
	for(int i=0; i< layers.size(); i++)
    {
      layers[i]->Update(request);
    }
  }

  virtual void Draw(Leds* leds) const override
  {
    for(int i=0; i< layers.size(); i++)
    {
      layers[i]->Draw(leds);
    }
  }
  
  virtual CRGB GetPixel(int id)
  {
	CRGB result=CRGB::Black;
	/*for(int i=0; i< layers.size(); i++)
    {
	  if(layers[i]->startId<=id && layers[i]->count>id-layers[i]->startId)
		result = layers[i]->GetPixel(id);
    }*/
	
	return result;
  }
  
  virtual LedMode& operator++() override
  {
	for(int i=0; i< layers.size(); i++)
    {
      ++(*(layers[i]));
    }
  }
  virtual LedMode& operator--() override
  {
	for(int i=0; i< layers.size(); i++)
    {
      --(*(layers[i]));
    }
  }
};

#endif // LAYER_MODE_H