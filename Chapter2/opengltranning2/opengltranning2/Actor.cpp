#include "Actor.h"
#include "Game.h"
#include "Component.h"
#include <algorithm>


Actor::Actor(Game * game):mState(EActive),mPosition(Vector2::Zero),mScale(1.0f),mRotation(0.0f),mGame(game)
{
	mGame->AddActor(this);
}

Actor::~Actor()
{
	mGame->RemoveActor(this);
	while (!mComponents.empty())
	{
		delete mComponents.back();
	}
}

void Actor::Update(float deltatime)
{
	if (mState == EActive)
	{
		UpdateComponent(deltatime);
		UpdateActor(deltatime);
	}
}

void Actor::UpdateComponent(float deltatime)
{
	for (auto comp : mComponents)
	{
		comp->Update(deltatime);
	}
}

void Actor::UpdateActor(float deltatime)
{

}

void Actor::AddComponent(Component * component)
{
	int myOrder = component->GetUpdateOrder();
	auto iter = mComponents.begin();
	for (; iter != mComponents.end(); iter++)
	{
		if (myOrder < (*iter)->GetUpdateOrder()) //������� �׿��ִ� ������Ʈ�鿡�� ������ ã�´�.
		{
			break; // ã���� break;
		}
	}
	mComponents.insert(iter, component); //ã�� �ڸ��� ����
}

void Actor::RemoveComponent(Component * component)
{
	auto iter = std::find(mComponents.begin(), mComponents.end(),component);
	if (iter != mComponents.end())
	{
		mComponents.erase(iter);
	}
}
