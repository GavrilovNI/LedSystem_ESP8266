#ifndef LAYER_MASK_H
#define LAYER_MASK_H

#include <vector>
#include <algorithm>
#include "LedMask.h"

class LayerMask: public LedMask
{
	
  protected:
  std::vector<LedMask*> layers;
  
  virtual bool IsMasked(int id) const override
  {
	  //Serial.println("ismasked ");
	for(int i=0; i< layers.size(); i++)
    {
	  //Serial.println("id: "+String(id)+" i: "+String(i)+" "+String(layers[i]->IsUnderMask(id)));
	  if(layers[i]->IsUnderMask(id))
		return true;
    }
	return false;
  }
  
  virtual uint32_t GetMaskLocal(int id) const override
  {
	uint32_t result=0;
	for(int i=0; i< layers.size(); i++)
	{
		uint32_t currLayerMask = layers[i]->GetMask(id);
		if(currLayerMask > result)
			result = currLayerMask;
	}
	
	return result;
  }
  
  public:

  LayerMask(float startId = 0, float count = LED_COUNT):LedMask(startId, count)
  {
    
  }
  
  int GetLayersCount() const
  {
    return layers.size();
  }
  void AddLayer(LedMask* mode)
  {
    layers.push_back(mode);
  }

  void RemoveLayer(int id)
  {
    layers.erase(layers.begin()+id);
  }
  
  virtual LedMask& operator+=(const float& value) override
  {
	for(int i=0; i< layers.size(); i++)
	{
	  (*(layers[i]))+=value;
	}
  }
};

#endif // LAYER_MASK_H