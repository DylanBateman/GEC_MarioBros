#ifndef _CHARACTERLUIGI_H
#define _CHARACTERLUIGI_H

#include "Character.h"

class CharacterLuigi : public Character
{
public:
	CharacterLuigi(SDL_Renderer* renderer, string imagePath, Vector2D startPosition, LevelMap* map);
	~CharacterLuigi();

protected:
	void Render();
	void Update(float deltaTime, SDL_Event e);
	void Keyboard(float deltaTime, SDL_Event e);

private:
	float mSingleSpriteWidth;
	float mSingleSpriteHeight;

	// Movement Animation Related
	float mFrameDelay;
	int mCurrentFrame;
	bool reverseAnimation;

	void luigiMovingAnimation(float deltaTime, SDL_Event e);
};

#endif // !_CHARACTERLUIGI_H