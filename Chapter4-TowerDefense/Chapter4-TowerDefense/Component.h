#pragma once
#include <cstdint>

class Component
{
public:
	Component(class Actor* owner, int updateorder = 100);
	virtual ~Component();
	virtual void Update(float deltatime);
	virtual void ProcessInput(const uint8_t* keystate) {}

	int GetUpdateOrder() { return mUpdateOrder; }
private:
	class Actor* mOwner;
	int mUpdateOrder;
};

