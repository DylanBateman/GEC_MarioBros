#pragma once

#include "Character.h"

class CharacterGoomba : public Character
{
public:
	CharacterGoomba(SDL_Renderer* renderer, string imagePath, LevelMap* map, Vector2D startPosition, FACING startFacing);
	~CharacterGoomba();

	void Render();
	void Update(float deltaTime, SDL_Event e);

	void TakeDamage();

	void Jump();

	// Check if goomba in vulnerable state
	bool GetVulnerableState() { return mVulnerable; }

private:
	float mSingleSpriteWidth;
	float mSingleSpriteHeight;

	// POW Block Related
	bool mVulnerable;
	float mVulnerableTime;

	// Movement Animation related
	float mFrameDelay;
	int mCurrentFrame;
	void GoombaMovingAnimation(float deltaTime, SDL_Event e);

	void FlipCorrectOrientation();
};