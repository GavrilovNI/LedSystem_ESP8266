#ifndef FULL_MASK_H
#define FULL_MASK_H

#include "LedMask.h"

class FullMask : public LedMask
{
  protected:
  virtual uint32_t GetMaskLocal(int id) const override
  {
	return 255;
  }
  
  public:
  FullMask(float startId = 0, float count = LED_COUNT) : LedMask(startId, count)
  {
	  
  }
  
  
};



#endif // FULL_MASK_H
