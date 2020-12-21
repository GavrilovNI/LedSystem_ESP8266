#ifndef SPLIT_MASK_H
#define SPLIT_MASK_H

#include "LedMask.h"

class SplitMask : public LedMask
{
  protected:
  int currId;
	
  public:
  SplitMask(int startId = 0, int count = LED_COUNT) : LedMask(startId, count)
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
  
  
  virtual bool IsMasked(int id) const override
  {
	//Serial.print("id: "+String(id));
	//Serial.println(" currId: "+String(currId));
	  
	return id<currId;
  }

  virtual LedMask& operator++() override
  {
	if(currId >= GetEndId())
	  currId = startId;
	else
	  currId++;
    return *this;
  }
  virtual LedMask& operator--() override
  {
	if(currId <= startId)
	  currId = GetEndId();
	else
	  currId--;
    return *this;
  }
};



#endif // SPLIT_MASK_H
