#ifndef _CHARACTERMARIO_H
#define _CHARACTERMARIO_H

#include "Character.h"

class CharacterMario : public Character
{
public:
	CharacterMario(SDL_Renderer* renderer, string imagePath, Vector2D startPosition, LevelMap* map);
	~CharacterMario();

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

	void marioMovingAnimation(float deltaTime, SDL_Event e);
};

#endif // !_CHARACTERMARIO_H