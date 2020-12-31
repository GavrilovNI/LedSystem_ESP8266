#ifndef RUNNER_MASK_H
#define RUNNER_MASK_H

#include <cmath>
#include <algorithm>
#include "LedMask.h"
#include "../utils.h"

class RunnerMask : public LedMask
{
  protected:
  float currId;
  float period;
  float length;
  
  float getIntersection(float begin1, float end1, float begin2, float end2) const
  {
	if(begin1>=begin2 && begin1<=end2)
		return std::min(end1, end2)-begin1;
	else if(begin2>=begin1 && begin2<=end1)
		return std::min(end1, end2)-begin2;
	return 0;
  }
  
  virtual uint32_t GetMaskLocal(int id) const override
  { 
	float localId = fmod(id, period);
	  
	float begin = startId + currId;
	float end = begin + length;
	
	float result = getIntersection(begin, end, localId, localId + 1) + getIntersection(begin-period, end-period, localId, localId + 1);
	
	
	return (uint32_t)(255 * std::min((float)1, result));
  }
  
  
	
  public:
  RunnerMask(float period=5, float length=1, float startId = 0, float count = LED_COUNT) : LedMask(startId, count)
  {
	SetCurrId(0);
	SetPeriod(period);
	SetLength(length);
  }
  
  void SetCurrId(int value)
  {
    this->currId = clamp(value, 0, period-1);
  }
  void SetPeriod(int value)
  {
    this->period = std::max(1, value);
	SetLength(this->length);
	SetCurrId(this->currId);
  }
  void SetLength(int value)
  {
    this->length = clamp(value, 0, period);
  }
  
  int GetCurrId() const { return currId; }
  int GetPeriod() const { return period; }
  
  
  virtual LedMask& operator+=(const float& value) override
  {
	currId = fmod(currId + value, period);
	if(currId < 0)
		currId = period + currId;
	
    return *this;
  }
};



#endif // RUNNER_MASK_H
