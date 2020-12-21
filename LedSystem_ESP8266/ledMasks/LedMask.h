#ifndef LED_MASK_H
#define LED_MASK_H

#include <vector>
#include "../leds.h"

class LedMask
{
  protected:
  int startId;
  int count;

  LedMask(int startId = 0, int count = LED_COUNT)
  {
    this->startId = startId;
    this->count = count;
  }
  virtual bool IsMasked(int id) const = 0;
  
  public:
  
  bool swaped = false;
  
  int GetStartId() { return startId; }
  int GetCount() { return count; }
  
  int GetEndId() const { return startId + count; }

  bool IsUnderMask(int id)
  {
	if(id<startId)
	  return false;
	if(id>=GetEndId())
	  return false;
	  
	return IsMasked(swaped?GetEndId()-(id-startId)-1:id);
  }

  virtual LedMask& operator++() { return *this; }
  virtual LedMask& operator--() { return *this; };
};



#endif // LED_MASK_H
