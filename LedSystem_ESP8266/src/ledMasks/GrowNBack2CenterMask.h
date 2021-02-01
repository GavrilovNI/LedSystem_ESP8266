#ifndef GROW_N_BACK_2_CENTER_MASK_H
#define GROW_N_BACK_2_CENTER_MASK_H

#include "LayerMask.h"

class GrowNBack2CenterMask : public LayerMask
{
  
	
  public:
  
  
  GrowNBack2CenterMask(float startId = 0, float count = LED_COUNT) : LayerMask(startId, count)
  {
    /*GrowNBackMask* gnb0 = new GrowNBackMask(startId, count/2+count%2);
	GrowNBackMask* gnb1 = new GrowNBackMask(startId+count/2, count/2+count%2);*/
	
	GrowNBackMask* gnb0 = new GrowNBackMask(startId, count/2);
	GrowNBackMask* gnb1 = new GrowNBackMask(startId+count/2, count/2);
	
	gnb0->swaped=!gnb1->swaped;
	AddLayer(gnb0);
	AddLayer(gnb1);
  }
  ~GrowNBack2CenterMask()
  {
	  for(int i=0; i<layers.size() && i<2; i++)
	  {
		  delete layers[i];
	  }
  }
  
  
};



#endif // GROW_N_BACK_2_CENTER_MASK_H
