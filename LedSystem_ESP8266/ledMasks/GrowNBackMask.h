#ifndef GROW_N_BACK_MASK_H
#define GROW_N_BACK_MASK_H

#include <cmath>
#include <algorithm>
#include "SplitMask.h"

class GrowNBackMask : public SplitMask
{
  
	
  public:
  bool growing=true;
  
  
  GrowNBackMask(float startId = 0, float count = LED_COUNT) : SplitMask(startId, count)
  {
    
  }
  
  
  virtual LedMask& operator+=(const float& value) override
  {
	float lValue=value;
	
	bool g;
	if(value>0)
	{
		g = growing;
	}
	else
	{
		g = !growing;
		lValue = -lValue;
	}	
	
	
	while(lValue!=0)
	{
		float _max, _min;
		float _sign;
		if(g)
		{
			_max = GetEndId();
			_min = currId;
			_sign = 1;
		}
		else
		{
			_max = currId;
			_min = GetStartId();
			_sign = -1;
		}	
		
		float _diff = _max - _min;
		
		if(lValue >= _diff)
		{
			lValue -= _diff;
			currId += _sign * _diff;
			g = !g;
		}
		else
		{
			currId += _sign * lValue;
			lValue = 0;
		}	
	}
	
	growing = value > 0 ? g : !g;
	
    return *this;
  }
  
  
};



#endif // GROW_N_BACK_MASK_H
