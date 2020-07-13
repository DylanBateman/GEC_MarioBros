#include "GameScreenLevel2.h"
#include "Texture2D.h"
#include "CharacterMario.h"
#include "CharacterLuigi.h"

GameScreenLevel2::GameScreenLevel2(SDL_Renderer* renderer) : GameScreen(renderer)
{
	mBackgroundYPos = 0.0f;

	playersDead = false;

	startTimer = false;
	startGameTimer = START_TIMER;

	pauseMusic = MUSIC_TIMER;
	enemyTimer = ENEMY_TIMER;

	coinsLeft = 10;

	SetUpLevel();
}

GameScreenLevel2::~GameScreenLevel2()
{
	// Game Objects
	delete mBackground;
	mBackground = NULL;

	delete mLevelTexture;
	mLevelTexture = NULL;

	delete mTransparency;
	mTransparency = NULL;

	delete characterMario;
	characterMario = NULL;

	delete characterLuigi;
	characterLuigi = NULL;

	mLevelMap = NULL;

	// File Handling
	if (marioFileOut.is_open())
	{
		marioFileOut.close();
	}
	if (luigiFileOut.is_open())
	{
		luigiFileOut.close();
	}

	if (marioFileIn.is_open())
	{
		marioFileIn.close();
	}
	if (luigiFileIn.is_open())
	{
		luigiFileIn.close();
	}

	// Enemy Related
	mEnemyKoopa.clear();
	mEnemyGoomba.clear();
	mCoins.clear();

	// Sound Related
	delete levelMusic;
	levelMusic = NULL;

	delete coinSound;
	coinSound = NULL;

	delete deathSound;
	deathSound = NULL;

	delete levelWinSound;
	levelWinSound = NULL;

	delete gameOverSound;
	gameOverSound = NULL;

	delete kickSound;
	kickSound = NULL;

	delete pauseSound;
	pauseSound = NULL;

	delete thwompSound;
	thwompSound = NULL;

	delete finalScoreSound;
	finalScoreSound = NULL;

	// Text Related
	SDL_DestroyTexture(pauseText);

	SDL_DestroyTexture(marioText);
	SDL_DestroyTexture(marioScoreText);

	SDL_DestroyTexture(luigiText);
	SDL_DestroyTexture(luigiScoreText);

	SDL_DestroyTexture(gameOverText);
	SDL_DestroyTexture(nextLevelText);
	SDL_DestroyTexture(viewScoresText);
}

void GameScreenLevel2::Render()
{
	mBackground->Render(Vector2D(), SDL_FLIP_NONE);
	mLevelTexture->Render(Vector2D(0, mBackgroundYPos), SDL_FLIP_NONE);

	// Render Players
	characterMario->Render();
	characterLuigi->Render();

	// Render Coins
	for (unsigned int i = 0; i < mCoins.size(); i++)
	{
		mCoins[i]->Render();
	}

	// Render Enemies
	for (unsigned int i = 0; i < mEnemyKoopa.size(); i++)
	{
		mEnemyKoopa[i]->Render();
	}
	for (unsigned int i = 0; i < mEnemyGoomba.size(); i++)
	{
		mEnemyGoomba[i]->Render();
	}

	// Render Mario Score
	marioTextRect.x = 72;
	marioTextRect.y = 5;
	SDL_RenderCopy(mRenderer, marioText, NULL, &marioTextRect);
	marioScoreTextRect.x = 195;
	marioScoreTextRect.y = 6;
	SDL_RenderCopy(mRenderer, marioScoreText, NULL, &marioScoreTextRect);

	// Render Luigi Score
	luigiTextRect.x = 302;
	luigiTextRect.y = 5;
	SDL_RenderCopy(mRenderer, luigiText, NULL, &luigiTextRect);
	luigiScoreTextRect.x = 425;
	luigiScoreTextRect.y = 6;
	SDL_RenderCopy(mRenderer, luigiScoreText, NULL, &luigiScoreTextRect);

	if (paused)
	{
		mTransparency->Render(Vector2D(), SDL_FLIP_NONE);

		pauseTextRect.x = (SCREEN_WIDTH / 2) - (pauseTextRect.w / 2);
		pauseTextRect.y = (SCREEN_HEIGHT / 2) - (pauseTextRect.h / 2);
		SDL_RenderCopy(mRenderer, pauseText, NULL, &pauseTextRect);
	}
	else
	{
		if (coinsLeft <= 0)
		{
			mBackground->Render(Vector2D(), SDL_FLIP_NONE);

			Mix_PlayChannel(-1, levelWinSound, 0);

			gameWonTextRect.x = (SCREEN_WIDTH / 2) - (gameWonTextRect.w / 2);
			gameWonTextRect.y = (SCREEN_HEIGHT / 2) - (gameWonTextRect.h / 2);
			SDL_RenderCopy(mRenderer, gameWonText, NULL, &gameWonTextRect);

			viewScoresTextRect.x = (SCREEN_WIDTH / 2) - (viewScoresTextRect.w / 2);
			viewScoresTextRect.y = ((SCREEN_HEIGHT / 2) - (viewScoresTextRect.h / 2)) + 40;
			SDL_RenderCopy(mRenderer, viewScoresText, NULL, &viewScoresTextRect);
		}

		if (characterMario->GetAlive() == false && characterLuigi->GetAlive() == false && coinsLeft > 0)
		{
			mBackground->Render(Vector2D(), SDL_FLIP_NONE);

			gameOverTextRect.x = (SCREEN_WIDTH / 2) - (gameOverTextRect.w / 2);
			gameOverTextRect.y = (SCREEN_HEIGHT / 2) - (gameOverTextRect.h / 2);
			SDL_RenderCopy(mRenderer, gameOverText, NULL, &gameOverTextRect);

			viewScoresTextRect.x = (SCREEN_WIDTH / 2) - (viewScoresTextRect.w / 2);
			viewScoresTextRect.y = ((SCREEN_HEIGHT / 2) - (viewScoresTextRect.h / 2)) + 40;
			SDL_RenderCopy(mRenderer, viewScoresText, NULL, &viewScoresTextRect);
		}
	}
}

void GameScreenLevel2::Update(float deltaTime, SDL_Event e)
{
	startGameTimer -= 1.0f;

	if (startGameTimer <= 0.0f)
	{
		CheckPaused(e);
	}

	if (startGameTimer == 1000.0f)
	{
		Mix_PlayMusic(levelMusic, -1);
	}

	if (!paused && startGameTimer <= 0.0f && coinsLeft > 0)
	{
		if (pauseMusic == MUSIC_TIMER)
		{
			Mix_ResumeMusic();
		}

		characterMario->Update(deltaTime, e);
		WallCollisions(characterMario);

		characterLuigi->Update(deltaTime, e);
		WallCollisions(characterLuigi);

		// Spawn Enemies
		enemyTimer -= 0.5f;
		switch ((int)enemyTimer)
		{
		case 15000:
			CreateKoopa(Vector2D(64, 32), FACING_RIGHT);
			break;

		case 10000:
			CreateGoomba(Vector2D(414, 32), FACING_LEFT);
			break;

		case 5000:
			CreateGoomba(Vector2D(64, 32), FACING_RIGHT);
			break;

		case 0:
			enemyTimer = ENEMY_TIMER;
			CreateKoopa(Vector2D(414, 32), FACING_LEFT);
			break;

		default:
			break;
		}

		UpdateKoopas(deltaTime, e);
		UpdateGoombas(deltaTime, e);

		UpdateCoin(deltaTime, e);

		if (characterMario->GetAlive() == false && characterLuigi->GetAlive() == false && playersDead == false)
		{
			Mix_HaltMusic();
			Mix_HaltChannel(-1);
			Mix_PlayChannel(-1, gameOverSound, 0);
			playersDead = true;
		}
	}
	else
	{
		Mix_PauseMusic();
	}
}

bool GameScreenLevel2::SetUpLevel()
{
	SetLevelMap();

	LoadAudio();
	OpenInFiles();
	LoadFont();

	mBackground = new Texture2D(mRenderer);
	if (!mBackground->LoadFromFile("Images/GameScreens/Background.png"))
	{
		cout << "Failed to load black background texture!";
		return false;
	}

	mLevelTexture = new Texture2D(mRenderer);
	if (!mLevelTexture->LoadFromFile("Images/GameScreens/Level_2.png"))
	{
		cout << "Failed to load level map texture!";
		return false;
	}

	mTransparency = new Texture2D(mRenderer);
	if (!mTransparency->LoadFromFile("Images/Transparency.png"))
	{
		cout << "Failed to load transparency image!";
		return false;
	}

	characterMario = new CharacterMario(mRenderer, "Images/Characters/MarioWalk.png", Vector2D(64, 342), mLevelMap);
	characterLuigi = new CharacterLuigi(mRenderer, "Images/Characters/LuigiWalk.png", Vector2D(414, 342), mLevelMap);

	CreateKoopa(Vector2D(64, 34), FACING_RIGHT);
	CreateGoomba(Vector2D(414, 34), FACING_LEFT);

	// Create and Set Coin Positions.
	{
		// Bottom Left 
		CreateCoin(Vector2D(25, 267));

		// Left platform
		CreateCoin(Vector2D(24, 172));

		//Bottom Right
		CreateCoin(Vector2D(472, 267));

		// Right platform
		CreateCoin(Vector2D(470, 172));

		// Bottom-Centre platform
		CreateCoin(Vector2D(250, 265));

		// Centre-left platform
		CreateCoin(Vector2D(151, 109));
		CreateCoin(Vector2D(186, 109));

		// Centre-right platform
		CreateCoin(Vector2D(312, 109));
		CreateCoin(Vector2D(346, 109));

		// Centre-Top platform
		CreateCoin(Vector2D(254, 40));
	}

	return true;
}

void GameScreenLevel2::SetLevelMap()
{
	int map[MAP_HEIGHT][MAP_WIDTH] = {  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
										{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
										{1,1,1,1,1,0,0,0,1,1,1,0,0,1,1,1,1},
										{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
										{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
										{0,0,0,0,0,1,1,1,0,0,1,1,1,0,0,0,0},
										{1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1},
										{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
										{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
										{1,1,1,0,0,0,0,1,1,1,0,0,0,0,0,1,1},
										{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
										{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
										{1,1,1,1,1,0,0,0,0,0,0,0,0,1,1,1,1} };

	// Clear Old Map
	if (mLevelMap != NULL)
	{
		delete mLevelMap;
	}

	// Set New Level Map
	mLevelMap = new LevelMap(map);
}

void GameScreenLevel2::WallCollisions(Character* character)
{
	// Right Wall Collision - Wrap Left Side
	if (character->GetPosition().x >= SCREEN_WIDTH - character->GetCollisionBox().width / 3)
	{
		character->SetPosition(Vector2D(0, character->GetPosition().y));
	}

	// Left Wall Collision - Wrap Right Side
	if (character->GetPosition().x < 0)
	{
		character->SetPosition(Vector2D(SCREEN_WIDTH - character->GetCollisionBox().width / 3, character->GetPosition().y));
	}

	// Bottom Floor Collision - Players / Enemies Die
	if (character->GetPosition().y > SCREEN_HEIGHT)
	{
		if (character == characterMario && character->GetAlive() == true)
		{
			Mix_PauseMusic();
			startTimer = true;
			Mix_PlayChannel(-1, deathSound, 0);

			character->SetAlive(false);
			characterMario->SetPosition(Vector2D(500, 1000));

		}

		else if (character == characterLuigi && character->GetAlive() == true)
		{
			Mix_PauseMusic();
			startTimer = true;
			Mix_PlayChannel(-1, deathSound, 0);

			character->SetAlive(false);
			characterLuigi->SetPosition(Vector2D(500, 1000));
		}
	}

	if (startTimer == true)
	{
		pauseMusic -= 0.5f;
	}

	if (pauseMusic <= 0.0f)
	{
		pauseMusic = MUSIC_TIMER;
		startTimer = false;
		Mix_ResumeMusic();
	}
}

void GameScreenLevel2::UpdateKoopas(float deltaTime, SDL_Event e)
{
	if (!mEnemyKoopa.empty())
	{
		int enemyIndexToDelete = -1;
		for (unsigned int i = 0; i < mEnemyKoopa.size(); i++)
		{
			// Checks If Enemy On Bottom Tiles
			if (mEnemyKoopa[i]->GetPosition().y > 300.0f)
			{
				// Checks Enemy Off Screen
				if (mEnemyKoopa[i]->GetPosition().x < (float)(-mEnemyKoopa[i]->GetCollisionBox().width * 0.5f) || mEnemyKoopa[i]->GetPosition().x > SCREEN_WIDTH - (float)(mEnemyKoopa[i]->GetCollisionBox().width * 0.5f))
				{
					mEnemyKoopa[i]->SetAlive(false);

				}
			}

			mEnemyKoopa[i]->Update(deltaTime, e);

			// Checks For Collision With Player.
			if (mEnemyKoopa[i]->GetPosition().y > 300.0f || mEnemyKoopa[i]->GetPosition().y <= 64.0f && (mEnemyKoopa[i]->GetPosition().x < 64.0f || mEnemyKoopa[i]->GetPosition().x > SCREEN_WIDTH - 96.0f))
			{
			}
			else
			{
				if (Collisions::Instance()->Circle(mEnemyKoopa[i], characterMario))
				{
					if (mEnemyKoopa[i]->GetVulnerableState())
					{
						Mix_PlayChannel(-1, kickSound, 0);

						mEnemyKoopa[i]->SetAlive(false);

						marioScore += 20;
						WriteToFiles();
						LoadPlayerScores();
					}

					else
					{
						Mix_PauseMusic();

						startTimer = true;

						Mix_PlayChannel(-1, deathSound, 0);

						characterMario->SetAlive(false);
						characterMario->SetPosition(Vector2D(500, 1000));

					}
				}
				else if (Collisions::Instance()->Circle(mEnemyKoopa[i], characterLuigi))
				{
					if (mEnemyKoopa[i]->GetVulnerableState())
					{
						Mix_PlayChannel(-1, kickSound, 0);

						mEnemyKoopa[i]->SetAlive(false);

						luigiScore += 20;
						WriteToFiles();
						LoadPlayerScores();

					}
					else
					{
						Mix_PauseMusic();

						startTimer = true;

						Mix_PlayChannel(-1, deathSound, 0);

						characterLuigi->SetAlive(false);
						characterLuigi->SetPosition(Vector2D(500, 1000));
					}
				}
			}

			if (!mEnemyKoopa[i]->GetAlive())
			{
				enemyIndexToDelete = i;
			}
		}

		if (enemyIndexToDelete != -1)
		{
			mEnemyKoopa.erase(mEnemyKoopa.begin() + enemyIndexToDelete);
		}
	}

	if (startTimer == true)
	{
		pauseMusic -= 0.5f;
	}

	if (pauseMusic <= 0.0f)
	{
		pauseMusic = MUSIC_TIMER;
		startTimer = false;
		Mix_ResumeMusic();
	}
}

void GameScreenLevel2::UpdateGoombas(float deltaTime, SDL_Event e)
{
	if (!mEnemyGoomba.empty())
	{
		int enemyIndexToDelete = -1;
		for (unsigned int i = 0; i < mEnemyGoomba.size(); i++)
		{
			// Checks If Enemy On Bottom Tiles
			if (mEnemyGoomba[i]->GetPosition().y > 300.0f)
			{
				// Checks If Enemy Off Screen
				if (mEnemyGoomba[i]->GetPosition().x < (float)(-mEnemyGoomba[i]->GetCollisionBox().width * 0.5f) || mEnemyGoomba[i]->GetPosition().x > SCREEN_WIDTH - (float)(mEnemyGoomba[i]->GetCollisionBox().width * 0.5f))
				{
					mEnemyGoomba[i]->SetAlive(false);;
				}
			}

			mEnemyGoomba[i]->Update(deltaTime, e);

			// Checks For Collision With Player.
			if (mEnemyGoomba[i]->GetPosition().y > 300.0f || mEnemyGoomba[i]->GetPosition().y <= 64.0f && (mEnemyGoomba[i]->GetPosition().x < 64.0f || mEnemyGoomba[i]->GetPosition().x > SCREEN_WIDTH - 96.0f))
			{
			}
			else
			{
				if (Collisions::Instance()->Circle(mEnemyGoomba[i], characterMario))
				{
					if (mEnemyGoomba[i]->GetVulnerableState())
					{
						Mix_PlayChannel(-1, kickSound, 0);

						mEnemyGoomba[i]->SetAlive(false);

						marioScore += 20;
						WriteToFiles();
						LoadPlayerScores();
					}

					else
					{
						Mix_PauseMusic();

						startTimer = true;

						Mix_PlayChannel(-1, deathSound, 0);

						characterMario->SetAlive(false);
						characterMario->SetPosition(Vector2D(500, 1000));
					}
				}
				else if (Collisions::Instance()->Circle(mEnemyGoomba[i], characterLuigi))
				{
					if (mEnemyGoomba[i]->GetVulnerableState())
					{
						Mix_PlayChannel(-1, kickSound, 0);

						mEnemyGoomba[i]->SetAlive(false);

						luigiScore += 20;
						WriteToFiles();
						LoadPlayerScores();
					}

					else
					{
						Mix_PauseMusic();

						startTimer = true;

						Mix_PlayChannel(-1, deathSound, 0);

						characterLuigi->SetAlive(false);
						characterLuigi->SetPosition(Vector2D(500, 1000));

					}
				}
			}

			if (!mEnemyGoomba[i]->GetAlive())
			{
				enemyIndexToDelete = i;
			}
		}

		if (enemyIndexToDelete != -1)
		{
			mEnemyGoomba.erase(mEnemyGoomba.begin() + enemyIndexToDelete);
		}
	}

	if (startTimer == true)
	{
		pauseMusic -= 0.5f;
	}

	if (pauseMusic <= 0.0f)
	{
		pauseMusic = MUSIC_TIMER;
		startTimer = false;
		Mix_ResumeMusic();
	}
}

void GameScreenLevel2::UpdateCoin(float deltaTime, SDL_Event e)
{
	if (!mCoins.empty())
	{
		int coinIndexToDelete = -1;
		for (unsigned int i = 0; i < mCoins.size(); i++)
		{
			mCoins[i]->Update(deltaTime, e);

			if (Collisions::Instance()->Circle(mCoins[i], characterMario))
			{
				Mix_PlayChannel(-1, coinSound, 0);
				mCoins[i]->SetAlive(false);

				coinsLeft--;
				marioScore += 10;

				WriteToFiles();
				LoadPlayerScores();
			}

			if (Collisions::Instance()->Circle(mCoins[i], characterLuigi))
			{
				Mix_PlayChannel(-1, coinSound, 0);
				mCoins[i]->SetAlive(false);

				coinsLeft--;
				luigiScore += 10;

				WriteToFiles();
				LoadPlayerScores();
			}

			if (!mCoins[i]->GetAlive())
			{
				coinIndexToDelete = i;
			}
		}

		if (coinIndexToDelete != -1)
		{
			mCoins.erase(mCoins.begin() + coinIndexToDelete);
		}
	}
}


void GameScreenLevel2::CreateKoopa(Vector2D position, FACING direction)
{
	CharacterKoopa* characterKoopa = new CharacterKoopa(mRenderer, "Images/Enemies/Koopa.png", mLevelMap, position, direction);
	mEnemyKoopa.push_back(characterKoopa);
}

void GameScreenLevel2::CreateGoomba(Vector2D position, FACING direction)
{
	CharacterGoomba* characterGoomba = new CharacterGoomba(mRenderer, "Images/Enemies/Goomba.png", mLevelMap, position, direction);
	mEnemyGoomba.push_back(characterGoomba);
}

void GameScreenLevel2::CreateCoin(Vector2D position)
{
	CharacterCoin* characterCoin = new CharacterCoin(mRenderer, "Images/Objects/Coin.png", mLevelMap, position);
	mCoins.push_back(characterCoin);
}