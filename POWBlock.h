#ifndef _POWBLOCK_H
#define _POWBLOCK_H

#include <SDL.h>
#include "Commons.h"
#include "Texture2D.h"
#include "LevelMap.h"

class POWBlock
{
public:
	POWBlock(SDL_Renderer* renderer, LevelMap* map);
	~POWBlock();

	void Render();
	Rect2D GetCollisionBox() { return Rect2D(mPosition.x, mPosition.y, mSingleSpriteWidth, mTexture->GetHeight()); }; //mTexture->GetWidth()

	void TakeHit();
	bool IsAvailable() { return mNumberOfHitsLeft > 0; };

private:
	SDL_Renderer* mRenderer;
	Vector2D mPosition;
	Texture2D* mTexture;
	LevelMap* mLevelMap;

	float mSingleSpriteWidth;
	float mSingleSpriteHeight;

	int mNumberOfHitsLeft;
};

#endif // !_POWBLOCK_H