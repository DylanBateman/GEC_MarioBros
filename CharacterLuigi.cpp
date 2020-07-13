#include "CharacterLuigi.h"

CharacterLuigi::CharacterLuigi(SDL_Renderer* renderer, string imagePath, Vector2D startPosition, LevelMap* map) : Character(renderer, imagePath, startPosition, map)
{
	mFacingDirection = FACING_LEFT;

	mSingleSpriteWidth = mTexture->GetWidth() / 3;
	mSingleSpriteHeight = mTexture->GetHeight();

	reverseAnimation = false;
}

CharacterLuigi::~CharacterLuigi()
{

}

void CharacterLuigi::Render()
{
	int left = mCurrentFrame * mSingleSpriteWidth;

	SDL_Rect portionOfSpritesheet = { left, 0, mSingleSpriteWidth, mSingleSpriteHeight };
	SDL_Rect destRect = { (int)(mPosition.x), (int)(mPosition.y), mSingleSpriteWidth, mSingleSpriteHeight };

	if (mFacingDirection == FACING_RIGHT)
	{
		mTexture->Render(portionOfSpritesheet, destRect, SDL_FLIP_NONE);
	}
	else
	{
		mTexture->Render(portionOfSpritesheet, destRect, SDL_FLIP_HORIZONTAL);
	}
}

void CharacterLuigi::Update(float deltaTime, SDL_Event e)
{
	Character::Update(deltaTime, e);

	Keyboard(deltaTime, e);

	// Movement Animation For Either Direction
	if (mMovingRight || mMovingLeft)
	{
		luigiMovingAnimation(deltaTime, e);
	}
}

// Movement Using Keyboard
void CharacterLuigi::Keyboard(float deltaTime, SDL_Event e)
{
	switch (e.type)
	{
	case SDL_KEYUP:
		switch (e.key.keysym.sym)
		{
		case SDLK_LEFT:
			mMovingLeft = false;
			break;

		case SDLK_RIGHT:
			mMovingRight = false;
			break;

		default:
			break;
		}
		break;

	case SDL_KEYDOWN:
		switch (e.key.keysym.sym)
		{
		case SDLK_UP:
			Character::Jump();
			break;

		case SDLK_DOWN:
			break;

		case SDLK_LEFT:
			mMovingRight = false;
			mMovingLeft = true;
			break;

		case SDLK_RIGHT:
			mMovingLeft = false;
			mMovingRight = true;
			break;

		default:
			break;
		}
		break;
	}
}

// Animation For Luigi When Moving
void CharacterLuigi::luigiMovingAnimation(float deltaTime, SDL_Event e)
{
	mFrameDelay -= deltaTime;
	if (mFrameDelay <= 0.0f)
	{
		mFrameDelay = PLAYER_ANIMATION_DELAY;
		if (!reverseAnimation)
		{
			mCurrentFrame++;
			if (mCurrentFrame > 2)
			{
				mCurrentFrame = 2;
				reverseAnimation = true;
			}
		}
		else
		{
			mCurrentFrame--;

			if (mCurrentFrame < 0)
			{
				reverseAnimation = false;
				mCurrentFrame = 0;
			}
		}
	}
}