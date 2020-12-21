#ifndef FULL_MASK_H
#define FULL_MASK_H

#include "LedMask.h"

class FullMask : public LedMask
{
  public:
  FullMask(int startId = 0, int count = LED_COUNT) : LedMask(startId, count)
  {
	  
  }
  
  virtual bool IsMasked(int id) const override
  {
	return true;
  }
};



#endif // FULL_MASK_H
