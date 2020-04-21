#include "MoveComponent.h"
#include "Actor.h"


MoveComponent::MoveComponent(Actor * owner, int updateorder)
	:Component(owner,updateorder),
	mAngularSpeed(0.0f),
	mFowardSpeed(0.0f)
{
}

void MoveComponent::Update(float deltatime)
{
	if (!Math::NearZero(mAngularSpeed))
	{
		float rot = mOwner->GetRotation();
		rot += mAngularSpeed * deltatime;
		mOwner->SetRotation(rot);
	}
	if (!Math::NearZero(mFowardSpeed))
	{
		Vector2 pos = mOwner->GetPosition();
		pos += mOwner->GetForward()*mFowardSpeed*deltatime;
		mOwner->SetPosition(pos);
	}
}
