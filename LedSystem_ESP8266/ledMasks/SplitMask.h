#ifndef SPLIT_MASK_H
#define SPLIT_MASK_H

#include <cmath>
#include "LedMask.h"

class SplitMask : public LedMask
{
  protected:
  float currId;
  
  
  virtual bool IsMasked(int id) const override
  {
	//Serial.print("id: "+String(id));
	//Serial.println(" currId: "+String(currId));
	  
	return id<currId;
  }
  
  virtual uint32_t GetMaskLocal(int id) const override
  {
	if(id >= currId)
		return 0;
	
	if(id < floor(currId)) 
	  return 255;
	  
	return (uint32_t)((currId - id)*255);
  }
  
	
  public:
  SplitMask(float startId = 0, float count = LED_COUNT) : LedMask(startId, count)
  {
    currId = startId;
  }
  
  void SetCurrId(int id)
  {
    currId=id;
    if(currId<startId)
	  currId=startId;
	else if(currId>GetEndId())
	  currId=GetEndId();
  }
  
  int GetCurrId() { return currId; }
  
  
  
  virtual LedMask& operator+=(const float& value) override
  {
	currId+=value;
	
	while(currId > GetEndId())
		currId -= GetCount();
	
	while(currId < GetStartId())
		currId += GetCount();
	
    return *this;
  }
};



#endif // SPLIT_MASK_H
