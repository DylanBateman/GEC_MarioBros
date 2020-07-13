#pragma once

#include <Windows.h>
#include <vector>


#include "GameScreen.h"
#include "LevelMap.h"

#include "CharacterCoin.h"
#include "CharacterKoopa.h"
#include "CharacterGoomba.h"

using namespace std;

class Texture2D;
class Character;
class POWBlock;

class GameScreenLevel2 : GameScreen
{
public:
	GameScreenLevel2(SDL_Renderer* renderer);
	~GameScreenLevel2();

	void Render();
	void Update(float deltaTime, SDL_Event e);

	Character* characterMario;
	Character* characterLuigi;

	bool playersDead;

private:
	bool SetUpLevel();

	Texture2D* mBackground;
	Texture2D* mLevelTexture;
	Texture2D* mTransparency;

	// Map Related
	void SetLevelMap();
	LevelMap* mLevelMap;

	float mBackgroundYPos;

	// Collision Related
	void WallCollisions(Character* character);

	// Game Objects
	void UpdateCoin(float deltaTime, SDL_Event e);
	void CreateCoin(Vector2D position);
	vector<CharacterCoin*> mCoins;

	int coinsLeft;

	// Enemy Related
	void UpdateKoopas(float deltaTime, SDL_Event e);
	void CreateKoopa(Vector2D position, FACING direction);
	vector<CharacterKoopa*> mEnemyKoopa;

	void UpdateGoombas(float deltaTime, SDL_Event e);
	void CreateGoomba(Vector2D position, FACING direction);
	vector<CharacterGoomba*> mEnemyGoomba;

	float enemyTimer;

};