#ifndef LAYER_MASK_H
#define LAYER_MASK_H

#include <vector>
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
  
  public:

  LayerMask(int startId = 0, int count = LED_COUNT):LedMask(startId, count)
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
  
  

  virtual LedMask& operator++() override
  {
	for(int i=0; i< layers.size(); i++)
    {
      ++(*(layers[i]));
    }
	
    return *this;
  }
  virtual LedMask& operator--() override
  {
	for(int i=0; i< layers.size(); i++)
    {
      --(*(layers[i]));
    }
	
    return *this;
  }
};

#endif // LAYER_MASK_H