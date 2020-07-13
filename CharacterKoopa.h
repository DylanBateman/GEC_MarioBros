#pragma once

#include "Character.h"

class CharacterKoopa : public Character
{
public:
	CharacterKoopa(SDL_Renderer* renderer, string imagePath, LevelMap* map, Vector2D startPosition, FACING startFacing);
	~CharacterKoopa();

	void Render();
	void Update(float deltaTime, SDL_Event e);

	void TakeDamage();

	void Jump();

	// Check If Koopa In Vulnerable State
	bool GetVulnerableState() { return mVulnerable; }

private:
	float mSingleSpriteWidth;
	float mSingleSpriteHeight;

	// POW Block Related
	bool mVulnerable;
	float mVulnerableTime;

	// Movement Animation Related
	float mFrameDelay;
	int mCurrentFrame;
	void KoopaMovingAnimation(float deltaTime, SDL_Event e);

	void FlipCorrectOrientation();
};