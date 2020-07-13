#include "GameScreenLevel1.h"
#include "Texture2D.h"
#include "CharacterMario.h"
#include "CharacterLuigi.h"
#include "POWBlock.h"

GameScreenLevel1::GameScreenLevel1(SDL_Renderer* renderer) : GameScreen(renderer)
{
	playersDead = false;

	startTimer = false;
	startGameTimer = START_TIMER;

	enemyTimer = ENEMY_TIMER;

	paused = false;
	pauseMusic = MUSIC_TIMER;

	hitPowBlock = false;
	hitTimer = 0.0f;

	coinsLeft = 20;

	SetUpLevel();
}

GameScreenLevel1::~GameScreenLevel1()
{
	// Game Objects
	delete mLevelTexture;
	mLevelTexture = NULL;

	delete mTransparency;
	mTransparency = NULL;

	delete mBackground;
	mBackground = NULL;

	delete characterMario;
	characterMario = NULL;

	delete characterLuigi;
	characterLuigi = NULL;

	delete mPowBlock;
	mPowBlock = NULL;

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

void GameScreenLevel1::Render()
{
	mBackground->Render(Vector2D(), SDL_FLIP_NONE);
	mLevelTexture->Render(Vector2D(0, mBackgroundYPos), SDL_FLIP_NONE);

	// Renders Players
	characterMario->Render();
	characterLuigi->Render();

	// Render Game Objects
	mPowBlock->Render();

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

			nextLevelTextRect.x = (SCREEN_WIDTH / 2) - (nextLevelTextRect.w / 2);
			nextLevelTextRect.y = ((SCREEN_HEIGHT / 2) - (nextLevelTextRect.h / 2)) + 40;
			SDL_RenderCopy(mRenderer, nextLevelText, NULL, &nextLevelTextRect);
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

void GameScreenLevel1::Update(float deltaTime, SDL_Event e)
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

		if (mScreenshake)
		{
			mScreenshakeTime -= deltaTime;
			mWobble++;
			mBackgroundYPos = sin(mWobble);
			mBackgroundYPos *= 3.0f;

			// End Shake After Time
			if (mScreenshakeTime <= 0.0f)
			{
				mScreenshake = false;
				mBackgroundYPos = 0.0f;
			}
		}

		characterMario->Update(deltaTime, e);
		WallWrapping(characterMario);

		characterLuigi->Update(deltaTime, e);
		WallWrapping(characterLuigi);

		CharacterCollisions(characterMario, characterLuigi);

		UpdatePOWBlock();

		// Spawn Enemies
		enemyTimer -= 0.25f;

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

bool GameScreenLevel1::SetUpLevel()
{
	SetLevelMap();

	LoadAudio();
	LoadFont();

	OpenOutFiles();

	mBackground = new Texture2D(mRenderer);
	if (!mBackground->LoadFromFile("Images/GameScreens/Background.png"))
	{
		cout << "Failed To Load Cackground Texture!";
		return false;
	}

	mLevelTexture = new Texture2D(mRenderer);
	if (!mLevelTexture->LoadFromFile("Images/GameScreens/Level_1.png"))
	{
		cout << "Failed To Load Level Texture!";
		return false;
	}

	mTransparency = new Texture2D(mRenderer);
	if (!mTransparency->LoadFromFile("Images/Transparency.png"))
	{
		cout << "Failed To Load Transparency!";
		return false;
	}

	characterMario = new CharacterMario(mRenderer, "Images/Characters/MarioWalk.png", Vector2D(64, 342), mLevelMap);
	characterLuigi = new CharacterLuigi(mRenderer, "Images//Characters/LuigiWalk.png", Vector2D(414, 342), mLevelMap);

	mPowBlock = new POWBlock(mRenderer, mLevelMap);
	mScreenshake = false;
	mBackgroundYPos = 0.0f;

	CreateKoopa(Vector2D(64, 34), FACING_RIGHT);
	CreateGoomba(Vector2D(414, 34), FACING_LEFT);

	// Create and Set Coin Positions.
	{

		// Floor
		CreateCoin(Vector2D(127, 356));
		CreateCoin(Vector2D(181, 356));
		CreateCoin(Vector2D(235, 356));
		CreateCoin(Vector2D(289, 356));
		CreateCoin(Vector2D(342, 356));
		CreateCoin(Vector2D(385, 356));

		// Bottom-left platform
		CreateCoin(Vector2D(25, 266));
		CreateCoin(Vector2D(89, 266));
		CreateCoin(Vector2D(153, 266));

		// Bottom-right platform
		CreateCoin(Vector2D(345, 266));
		CreateCoin(Vector2D(409, 266));
		CreateCoin(Vector2D(471, 266));

		// Middle-left platform
		CreateCoin(Vector2D(25, 170));

		// Middle platform
		CreateCoin(Vector2D(153, 138));
		CreateCoin(Vector2D(217, 138));
		CreateCoin(Vector2D(281, 138));
		CreateCoin(Vector2D(345, 138));

		// Middle-right platform
		CreateCoin(Vector2D(471, 170));

		//Upper Left Platform
		CreateCoin(Vector2D(167, 41));

		//Upper Right Platform
		CreateCoin(Vector2D(328, 41));
	}

	return true;
}

void GameScreenLevel1::SetLevelMap()
{
	int map[MAP_HEIGHT][MAP_WIDTH] = { {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
										{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
										{1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1},
										{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
										{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
										{0,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0},
										{1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1},
										{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
										{0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0},
										{1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1},
										{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
										{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
										{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1} };

	// Clear Old Map
	if (mLevelMap != NULL)
	{
		delete mLevelMap;
	}

	// Set New Level Map
	mLevelMap = new LevelMap(map);
}

void GameScreenLevel1::UpdatePOWBlock()
{
	if (hitPowBlock)
	{
		hitTimer -= 1.0f;
	}
	if (hitTimer == 0.0f)
	{
		hitPowBlock = false;
	}

	if (Collisions::Instance()->Box(characterMario->GetCollisionBox(), mPowBlock->GetCollisionBox()))
	{
		if (mPowBlock->IsAvailable())
		{
			if (characterMario->IsJumping() && !hitPowBlock)
			{
				Mix_PlayChannel(-1, thwompSound, 0);
				DoScreenShake();

				mPowBlock->TakeHit();
				hitTimer = POWBLOCK_HIT_TIMER;
				hitPowBlock = true;

				characterMario->CancelJump();
			}
		}
	}

	else if (Collisions::Instance()->Box(characterLuigi->GetCollisionBox(), mPowBlock->GetCollisionBox()))
	{
		if (mPowBlock->IsAvailable() && !hitPowBlock)
		{
			if (characterLuigi->IsJumping())
			{
				Mix_PlayChannel(-1, thwompSound, 0);
				DoScreenShake();

				mPowBlock->TakeHit();
				hitTimer = POWBLOCK_HIT_TIMER;
				hitPowBlock = true;

				characterLuigi->CancelJump();
			}
		}
	}
}

void GameScreenLevel1::DoScreenShake()
{
	mScreenshake = true;
	mScreenshakeTime = SCREENSHAKE_DURATION;
	mWobble = 0.0f;

	for (unsigned int i = 0; i < mEnemyKoopa.size(); i++)
	{
		mEnemyKoopa[i]->TakeDamage();
	}

	for (unsigned int i = 0; i < mEnemyGoomba.size(); i++)
	{
		mEnemyGoomba[i]->TakeDamage();
	}
}

void GameScreenLevel1::WallWrapping(Character* character)
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
}

void GameScreenLevel1::UpdateKoopas(float deltaTime, SDL_Event e)
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
		pauseMusic -= 1.0f;
	}

	if (pauseMusic == 0.0f)
	{
		pauseMusic = MUSIC_TIMER;
		startTimer = false;
		Mix_ResumeMusic();
	}
}

void GameScreenLevel1::UpdateGoombas(float deltaTime, SDL_Event e)
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

			// Checks For Collision With Player
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
		pauseMusic -= 1.0f;
	}

	if (pauseMusic == 0.0f)
	{
		pauseMusic = MUSIC_TIMER;
		startTimer = false;
		Mix_ResumeMusic();
	}
}

void GameScreenLevel1::UpdateCoin(float deltaTime, SDL_Event e)
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
				Mix_PlayChannel(-1, coinSound, 1);
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

void GameScreenLevel1::CreateKoopa(Vector2D position, FACING direction)
{
	CharacterKoopa* characterKoopa = new CharacterKoopa(mRenderer, "Images/Enemies/Koopa.png", mLevelMap, position, direction);
	mEnemyKoopa.push_back(characterKoopa);
}

void GameScreenLevel1::CreateGoomba(Vector2D position, FACING direction)
{
	CharacterGoomba* characterGoomba = new CharacterGoomba(mRenderer, "Images/Enemies/Goomba.png", mLevelMap, position, direction);
	mEnemyGoomba.push_back(characterGoomba);
}

void GameScreenLevel1::CreateCoin(Vector2D position)
{
	CharacterCoin* characterCoin = new CharacterCoin(mRenderer, "Images/Objects/Coin.png", mLevelMap, position);
	mCoins.push_back(characterCoin);
}