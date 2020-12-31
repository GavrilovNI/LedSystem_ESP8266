#ifndef LED_MASK_H
#define LED_MASK_H

#include <vector>
#include "../leds.h"

class LedMask
{
	protected:
	float startId;
	float count;

	LedMask(float startId = 0, float count = LED_COUNT)
	{
		this->startId = startId;
		this->count = count;
	}
	virtual bool IsMasked(int id) const = 0;
	
	virtual uint32_t GetMaskLocal(int id) const = 0;

	public:

	bool swaped = false;

	float GetStartId() { return startId; }
	float GetCount() { return count; }

	float GetEndId() const { return startId + count; }

	bool IsUnderMask(int id)
	{
		if(id<startId)
			return false;
		if(id>=GetEndId())
			return false;
		  
		return IsMasked(swaped?GetEndId()-(id-startId)-1:id);
	}
	
	uint32_t GetMask(int id)
	{
		if(id<startId || id>=GetEndId())
			return 0;
		
		return GetMaskLocal(swaped?GetEndId()-(id-startId)-1:id);
	}

	virtual LedMask& operator+=(const float& value) { return *this; }
	LedMask& operator-=(const float& value) { *(this)+=-value; return *this; }
};



#endif // LED_MASK_H
