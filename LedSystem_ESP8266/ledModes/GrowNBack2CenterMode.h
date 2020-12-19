#ifndef GROW_N_BACK_2_CENTER_MODE_H
#define GROW_N_BACK_2_CENTER_MODE_H

#include "LayerMode.h"
#include "GrowNBack.h"

class GrowNBack2CenterMode : public LayerMode
{
  public:

  GrowNBack2CenterMode(CRGB color0, CRGB color1=CRGB::Black, int startId = 0, int count = LED_COUNT) : LayerMode(color0, color1, startId, count)
  {
	  LedMode* gnb = new GrowNBack(color1, color0, startId, count/2+count%2);
	  gnb->SetCurrId(count/2+count%2);
	  AddLayer(gnb);
	  AddLayer(new GrowNBack(color0, color1, startId+count/2, count/2+count%2));
  }
  GrowNBack2CenterMode(AsyncWebServerRequest *request, int startId = 0, int count = LED_COUNT) : LayerMode(request, startId, count)
  {
	  LedMode* gnb2 = new GrowNBack(request, startId+count/2, count/2+count%2)
	  LedMode* gnb1 = new GrowNBack(gnb2->GetColor(1), gnb2->GetColor(0), startId, count/2+count%2);
	  gnb1->SetCurrId(count/2+count%2);
	  AddLayer(gnb1);
	  AddLayer(gnb2);
	  
  }
  
  ~GrowNBack2CenterMode()
  {
	  for(int i=0; i<layers.size() && i<2; i++)
	  {
		  delete layers[i];
	  }
  }
  
  virtual LedMode& operator++() override
  {
	if(growing)
	{
	  currId++;
	  if(currId>count)
	  {
        currId=count-1;
		growing=!growing;
	  }
	}
	else
	{
	  currId--;
	  if(currId<0)
	  {
        currId=1;
		growing=!growing;
	  }
	}
    return *this;
  }
  virtual LedMode& operator--() override
  {
    if(!growing)
	{
	  currId++;
	  if(currId>count)
	  {
        currId=count-1;
		growing=!growing;
	  }
	}
	else
	{
	  currId--;
	  if(currId<0)
	  {
        currId=1;
		growing=!growing;
	  }
	}
    return *this;
  }
};

#endif // GROW_N_BACK_2_CENTER_MODE_H
