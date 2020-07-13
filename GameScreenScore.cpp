#include "GameScreenScore.h"
#include <iostream>

GameScreenScore::GameScreenScore(SDL_Renderer* renderer) : GameScreen(renderer)
{
	SetUpScreen();
}

GameScreenScore::~GameScreenScore()
{
	// Game Objects
	delete mBackground;
	mBackground = NULL;

	delete characterMario;
	characterMario = NULL;

	delete characterLuigi;
	characterLuigi = NULL;

	delete coinTexture;
	coinTexture = NULL;

	// Sound
	delete finalScoreSound;
	finalScoreSound = NULL;

	// Text
	SDL_DestroyTexture(finalScoresText);

	SDL_DestroyTexture(marioFinalScoreText);
	SDL_DestroyTexture(marioWinText);

	SDL_DestroyTexture(luigiFinalScoreText);
	SDL_DestroyTexture(luigiWinText);

}

void GameScreenScore::Render()
{
	mBackground->Render(Vector2D(), SDL_FLIP_NONE);

	// Title Text
	finalScoresTextRect.x = (SCREEN_WIDTH / 2) - (finalScoresTextRect.w / 2);
	finalScoresTextRect.y = 20;
	SDL_RenderCopy(mRenderer, finalScoresText, NULL, &finalScoresTextRect);

	// Mario Final Score
	characterMario->Render(Vector2D(200, 140), SDL_FLIP_NONE);

	coinTexture->Render(Vector2D(270, 150), SDL_FLIP_NONE);

	marioFinalScoreTextRect.x = 300;
	marioFinalScoreTextRect.y = 148;
	SDL_RenderCopy(mRenderer, marioFinalScoreText, NULL, &marioFinalScoreTextRect);

	// Luigi Final Score
	characterLuigi->Render(Vector2D(200, 240), SDL_FLIP_NONE);

	coinTexture->Render(Vector2D(270, 250), SDL_FLIP_NONE);

	luigiFinalScoreTextRect.x = 300;
	luigiFinalScoreTextRect.y = 248;
	SDL_RenderCopy(mRenderer, luigiFinalScoreText, NULL, &luigiFinalScoreTextRect);

	// Winner Text
	if (marioScore > luigiScore)
	{
		marioWinTextRect.x = (SCREEN_WIDTH / 2) - (marioWinTextRect.w / 2);
		marioWinTextRect.y = SCREEN_HEIGHT - 75;
		SDL_RenderCopy(mRenderer, marioWinText, NULL, &marioWinTextRect);
	}
	else if (luigiScore > marioScore)
	{
		luigiWinTextRect.x = (SCREEN_WIDTH / 2) - (luigiWinTextRect.w / 2);
		luigiWinTextRect.y = SCREEN_HEIGHT - 75;
		SDL_RenderCopy(mRenderer, luigiWinText, NULL, &luigiWinTextRect);
	}
}

void GameScreenScore::Update(float deltaTime, SDL_Event e)
{

}

bool GameScreenScore::SetUpScreen()
{
	LoadAudio();

	OpenInFiles();
	LoadFont();
	LoadPlayerScores();

	Mix_PlayChannel(-1, finalScoreSound, 0);

	mBackground = new Texture2D(mRenderer);
	if (!mBackground->LoadFromFile("Images/GameScreens/Background.png"))
	{
		cout << "Failed to load background texture!";
		return false;
	}

	characterMario = new Texture2D(mRenderer);
	if (!characterMario->LoadFromFile("Images/Characters/Mario.png"))
	{
		cout << "Failed to Load Mario Texture!";
		return false;
	}

	characterLuigi = new Texture2D(mRenderer);
	if (!characterLuigi->LoadFromFile("Images/Characters/Luigi.png"))
	{
		cout << "Failed to Load Luigi Texture!";
		return false;
	}

	coinTexture = new Texture2D(mRenderer);
	if (!coinTexture->LoadFromFile("Images/Objects/CoinTexture.png"))
	{
		cout << "Failed to Load Coin Texture!";
		return false;
	}

	return true;
}