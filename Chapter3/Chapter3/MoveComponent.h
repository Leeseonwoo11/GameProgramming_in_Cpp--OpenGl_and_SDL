#pragma once
#include "Component.h"

class MoveComponent :public Component
{
public:
	MoveComponent(class Actor* owner, int updateOrder = 10);

	void Update(float deltatime) override;
	
	float GetAngularSpeed() const { return mAngularSpeed; }
	float GetForwardSpeed() const { return mForwardSpeed; }
	void SetAngularSpeed(float angularspeed) { mAngularSpeed = angularspeed; }
	void SetForwardSpeed(float forwardspeed) { mForwardSpeed = forwardspeed; }
private:
	float mAngularSpeed;
	float mForwardSpeed;
};

