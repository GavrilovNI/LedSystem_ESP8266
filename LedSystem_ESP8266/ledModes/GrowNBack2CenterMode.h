#ifndef GROW_N_BACK_2_CENTER_MODE_H
#define GROW_N_BACK_2_CENTER_MODE_H

#include <algorithm>
#include "LayerMode.h"
#include "GrowNBackMode.h"

class GrowNBack2CenterMode : public LayerMode
{
  public:

  GrowNBack2CenterMode(CRGB color0, CRGB color1=CRGB::Black, int startId = 0, int count = LED_COUNT) : LayerMode(startId, count)
  {
	  GrowNBackMode* gnb0 = new GrowNBackMode(color0, color1, startId, count/2+count%2);
	  GrowNBackMode* gnb1 = new GrowNBackMode(color0, color1, startId+count/2, count/2+count%2);
	  gnb0->swaped=!gnb1->swaped;
	  //gnb0->SetCurrId(count/2+count%2);
	  AddLayer(gnb0);
	  AddLayer(gnb1);
  }
  GrowNBack2CenterMode(AsyncWebServerRequest *request, int startId = 0, int count = LED_COUNT) : LayerMode(startId, count)
  {
	  GrowNBackMode* gnb1 = new GrowNBackMode(request, startId+count/2, count/2+count%2);
	  GrowNBackMode* gnb0 = new GrowNBackMode(request, startId, count/2+count%2);
	  
	  /*CRGB color0 = gnb0->GetColor(0);
	  gnb0->SetColor(0, gnb0->GetColor(1));
	  gnb0->SetColor(1, color0);
	  
	  gnb0->SetCurrId(count/2+count%2);*/
	  
	  gnb0->swaped=!gnb1->swaped;
	  
	  AddLayer(gnb0);
	  AddLayer(gnb1);
	  
  }
  
  virtual void Update(AsyncWebServerRequest *request) override
  {
	LayerMode::Update(request);
	
	GrowNBackMode* gnb0 = (GrowNBackMode*)layers[0];
	GrowNBackMode* gnb1 = (GrowNBackMode*)layers[1];
	/*CRGB color0 = gnb0->GetColor(0);
	gnb0->SetColor(0, gnb0->GetColor(1));
	gnb0->SetColor(1, color0);
	gnb0->SetCurrId(count/2+count%2-layers[1].GetCurrId());
	
	layers[0]->growing = lay layers[1]->growing;*/
	
	gnb0->swaped=!gnb1->swaped;
  }
  
  ~GrowNBack2CenterMode()
  {
	  for(int i=0; i<layers.size() && i<2; i++)
	  {
		  delete layers[i];
	  }
  }
};

#endif // GROW_N_BACK_2_CENTER_MODE_H
