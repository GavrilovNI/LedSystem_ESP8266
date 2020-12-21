#ifndef GROW_N_BACK_MASK_H
#define GROW_N_BACK_MASK_H

#include "SplitMask.h"

class GrowNBackMask : public SplitMask
{
  
	
  public:
  bool growing=true;
  
  
  GrowNBackMask(int startId = 0, int count = LED_COUNT) : SplitMask(startId, count)
  {
    
  }
  
  virtual LedMask& operator++() override
  {
	const int endId = GetEndId();
	
	  
	if(growing)
	{
	  if(currId >= endId)
	  {
		currId = endId-1;
		growing = !growing;
	  }
	  else
	  {
	    currId++;
	  }
	}
	else
	{
	  if(currId <= startId)
	  {
		currId = startId + 1;
		growing = !growing;
	  }
	  else
	  {
	    currId--;
	  }
	}
	
    return *this;
  }
  
  virtual LedMask& operator--() override
  {
	int endId = GetEndId();
	  
	if(!growing)
	{
	  if(currId >= endId)
	  {
		currId = endId-1;
		growing = !growing;
	  }
	  else
	  {
	    currId++;
	  }
	}
	else
	{
	  if(currId <= startId)
	  {
		currId = startId + 1;
		growing = !growing;
	  }
	  else
	  {
	    currId--;
	  }
	}
    return *this;
  }
};



#endif // GROW_N_BACK_MASK_H
