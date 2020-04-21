#pragma once
#include "Component.h"
class MoveComponent :public Component
{
public:
	MoveComponent(class Actor* owner, int updateorder = 10);
	void Update(float deltatime) override;

	float GetAngularSpeed() { return mAngularSpeed; }
	float GetFowardSpeed() { return mFowardSpeed; }
	void SetAngularSpeed(float speed) { mAngularSpeed = speed; }
	void SetForwardSpeed(float speed) { mFowardSpeed = speed; }
private:
	float mAngularSpeed;
	float mFowardSpeed;
};

