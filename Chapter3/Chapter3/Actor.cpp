#include "Actor.h"
#include "Game.h"
#include "Component.h"
#include <algorithm>


Actor::Actor(Game * game): 
	mState(EActive),
	mPosition(Vector2::Zero),
	mRotation(0.0f),
	mScale(1.0f),
	mGame(game)
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
		UpdateComponents(deltatime);
		UpdateActor(deltatime);
	}
}

void Actor::UpdateComponents(float deltatime)
{
	for (auto comp : mComponents)
	{
		comp->Update(deltatime);
	}
}

void Actor::UpdateActor(float deltatime)
{
}

void Actor::ProcessInput(const uint8_t * keyState)
{
	if (mState == EActive)
	{
		for (auto comp : mComponents)
		{
			comp->ProcessInput(keyState);
		}
		ActorInput(keyState);
	}
}

void Actor::ActorInput(const uint8_t * KeyState)
{
}

void Actor::AddComponent(Component * component)
{
	int myOrder = component->GetUpdateOrder();
	auto iter = mComponents.begin();
	for (; iter != mComponents.end(); iter++)
	{
		if (myOrder < (*iter)->GetUpdateOrder())
		{
			break;
		}
	}
	mComponents.insert(iter, component);
}

void Actor::RemoveComponent(Component * component)
{
	auto iter = std::find(mComponents.begin(), mComponents.end(), component);
	if (iter != mComponents.end())
	{
		mComponents.erase(iter);
	}
}