#pragma once

#include <Windows.h>
#include <vector>

#include "Commons.h"
#include "GameScreen.h"
#include "LevelMap.h"

#include "CharacterCoin.h"
#include "CharacterKoopa.h"
#include "CharacterGoomba.h"

class Texture2D;
class Character;
class POWBlock;

class GameScreenLevel1 : GameScreen
{
public:
	GameScreenLevel1(SDL_Renderer* renderer);
	~GameScreenLevel1();

	void Render();
	void Update(float deltaTime, SDL_Event e);

	Character* characterMario;
	Character* characterLuigi;

	bool playersDead;

private:
	bool SetUpLevel();

	Texture2D* mLevelTexture;
	Texture2D* mTransparency;
	Texture2D* mBackground;

	// Level Map Related
	void SetLevelMap();
	LevelMap* mLevelMap;

	void WallWrapping(Character* character);

	// Game Objects

	void UpdateCoin(float deltaTime, SDL_Event e);
	void CreateCoin(Vector2D position);
	vector<CharacterCoin*> mCoins;

	int coinsLeft;

	//POW Block Related
	POWBlock* mPowBlock;
	bool hitPowBlock;
	float hitTimer;
	void UpdatePOWBlock();

	bool mScreenshake;
	float mScreenshakeTime;
	float mWobble;
	float mBackgroundYPos;
	void DoScreenShake();

	// Enemy Related
	void UpdateKoopas(float deltaTime, SDL_Event e);
	void CreateKoopa(Vector2D position, FACING direction);
	vector<CharacterKoopa*> mEnemyKoopa;

	void UpdateGoombas(float deltaTime, SDL_Event e);
	void CreateGoomba(Vector2D position, FACING direction);
	vector<CharacterGoomba*> mEnemyGoomba;

	float enemyTimer;
};