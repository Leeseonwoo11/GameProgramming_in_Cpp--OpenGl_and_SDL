#pragma once
#include "SpriteComponent.h"
#include <vector>

class AnimSpriteComponent :public SpriteComponent
{
public:
	AnimSpriteComponent(class Actor* owner, int drawOrder = 100);
	void Update(float deltatime) override;
	void SetAnsimTextures(const std::vector<SDL_Texture*>& textures);
	float GetAnimFPS() const { return mAnimFPS; }
	void SetAnimFPS(float fps) { mAnimFPS = fps; }
	void SetRunActive(bool b);
private:
	std::vector<SDL_Texture*> mAnimTextures;
	float mcurrFrame;
	float mAnimFPS;
	bool bIsActice;
	
};

