#ifndef _CHARACTER_H
#define _CHARACTER_H

#include <SDL_mixer.h>
#include <iostream>

#include "Texture2D.h"
#include "LevelMap.h"

class Character
{
public:
	Character(SDL_Renderer* renderer, string imagePath, Vector2D startPosition, LevelMap* map);
	~Character();

	virtual void Render();
	virtual void Update(float deltaTime, SDL_Event e);

	// Sound Related
	bool LoadAudio();
	Mix_Chunk* jumpSound;

	// Position Related
	void SetPosition(Vector2D newPosition);
	Vector2D GetPosition();

	// Movement Related
	FACING mFacingDirection;
	bool mMovingLeft;
	bool mMovingRight;
	float mMovementSpeedRight;
	float mMovementSpeedLeft;

	// Collision Related
	float GetCollisionRadius();
	Rect2D GetCollisionBox() { return Rect2D(mPosition.x, mPosition.y, mTexture->GetWidth(), mTexture->GetHeight()); }

	// Jumping Related
	bool IsJumping();
	void CancelJump();

	// Player State Related
	void SetAlive(bool isAlive);
	bool GetAlive() { return alive; }

private:
	// Map Pointer
	LevelMap* mCurrentLevelMap;

protected:
	// Character Set-Up Related
	SDL_Renderer* mRenderer;
	Texture2D* mTexture;
	Vector2D mPosition;

	// Individual Character Movement Related
	virtual void MoveLeft(float deltaTime);
	virtual void MoveRight(float deltaTime);
	virtual void AddGravity(float deltaTime);
	virtual void Jump();

	bool mJumping;
	bool mCanJump;
	float mJumpForce;

	float mCollisionRadius;

	bool alive = true;
};

#endif //_CHARACTER_H