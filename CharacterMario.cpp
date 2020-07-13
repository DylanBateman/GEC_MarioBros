#include "CharacterMario.h"

CharacterMario::CharacterMario(SDL_Renderer* renderer, string imagePath, Vector2D startPosition, LevelMap* map) : Character(renderer, imagePath, startPosition, map)
{
	mFacingDirection = FACING_RIGHT;

	mSingleSpriteWidth = mTexture->GetWidth() / 3;
	mSingleSpriteHeight = mTexture->GetHeight();

	reverseAnimation = false;
}

CharacterMario::~CharacterMario()
{

}

void CharacterMario::Render()
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

void CharacterMario::Update(float deltaTime, SDL_Event e)
{
	Character::Update(deltaTime, e);

	Keyboard(deltaTime, e);

	// Movement Animation For Either Direction
	if (mMovingRight || mMovingLeft)
	{
		marioMovingAnimation(deltaTime, e);
	}
}

// Movement Using Keyboard
void CharacterMario::Keyboard(float deltaTime, SDL_Event e)
{
	switch (e.type)
	{
	case SDL_KEYUP:
		switch (e.key.keysym.sym)
		{
		case SDLK_a:
			mMovingLeft = false;
			break;

		case SDLK_d:
			mMovingRight = false;
			break;

		default:
			break;
		}
		break;

	case SDL_KEYDOWN:
		switch (e.key.keysym.sym)
		{
		case SDLK_w:
			Character::Jump();
			break;

		case SDLK_s:
			break;

		case SDLK_a:
			mMovingRight = false;
			mMovingLeft = true;
			break;

		case SDLK_d:
			mMovingLeft = false;
			mMovingRight = true;
			break;

		default:
			break;
		}
		break;
	}
}

// Animation For Mario When Moving
void CharacterMario::marioMovingAnimation(float deltaTime, SDL_Event e)
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