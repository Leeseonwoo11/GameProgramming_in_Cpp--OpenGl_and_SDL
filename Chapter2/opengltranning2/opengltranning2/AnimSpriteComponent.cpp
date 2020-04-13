#include "AnimSpriteComponent.h"
#include "Math.h"
#include "Ship.h"
#include <iostream>
AnimSpriteComponent::AnimSpriteComponent(Actor * owner, int drawOrder)
	:SpriteComponent(owner, drawOrder),
	mcurrFrame(0.0f),
	mAnimFPS(24.0f),
	bIsActice(false)
{
}

void AnimSpriteComponent::Update(float deltatime)
{
	SpriteComponent::Update(deltatime);

	if (mAnimTextures.size() > 0 && bIsActice)
	{
		mcurrFrame += mAnimFPS * deltatime;
		while (mcurrFrame >= mAnimTextures.size())
		{
			mcurrFrame -= mAnimTextures.size();
		}
		SetTexture(mAnimTextures[static_cast<int>(mcurrFrame)]);
	}
}

void AnimSpriteComponent::SetAnsimTextures(const std::vector<SDL_Texture*>& textures)
{
	mAnimTextures = textures;
	if (mAnimTextures.size() > 0)
	{
		mcurrFrame = 0.0f;
		SetTexture(mAnimTextures[0]);
	}

}

void AnimSpriteComponent::SetRunActive(bool b)
{
	bIsActice = b;
	bIsDraw = b;
}
