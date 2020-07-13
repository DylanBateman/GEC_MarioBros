#ifndef _CHARACTERCOIN_H
#define _CHARACTERCOIN_H

#include "Character.h"

class CharacterCoin : public Character
{
public:
	CharacterCoin(SDL_Renderer* renderer, string imagePath, LevelMap* map, Vector2D startPosition);
	~CharacterCoin();

	void Render();
	void Update(float deltaTime, SDL_Event e);

	// CHecks if coin has been collected
	bool GetCollectedState() { return mCollected; }

private:
	float mSingleSpriteWidth;
	float mSingleSpriteHeight;

	float mFrameDelay;
	int mCurrentFrame;

	bool mCollected;
};

#endif //_CHARACTERCOIN_H