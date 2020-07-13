#include "GameScreen.h"
#include "GameScreenMenu.h"
#include "GameScreenLevel1.h"
#include "GameScreenLevel2.h"
#include "GameScreenScore.h"
#include "GameScreenManager.h"

#include <iostream>

using namespace std;

GameScreenManager::GameScreenManager(SDL_Renderer* renderer, SCREENS startScreen)
{
	mRenderer = renderer;
	mCurrentScreen = NULL;

	// Sets Up First Screen
	ChangeScreen(startScreen);
}

GameScreenManager::~GameScreenManager()
{
	mRenderer = NULL;

	delete mCurrentScreen;
	mCurrentScreen = NULL;
}

void GameScreenManager::Render()
{
	mCurrentScreen->Render();
}

void GameScreenManager::Update(float deltaTime, SDL_Event e)
{
	mCurrentScreen->Update(deltaTime, e);
}

void GameScreenManager::ChangeScreen(SCREENS newScreen)
{
	// Clears Old Screen
	if (mCurrentScreen != NULL)
	{
		delete mCurrentScreen;
	}

	GameScreenMenu* tempScreenMenu;
	GameScreenLevel1* tempScreenLevel1;
	GameScreenLevel2* tempScreenLevel2;
	GameScreenScore* tempScreenScore;
	switch (newScreen)
	{
	case SCREEN_MENU:
		tempScreenMenu = new GameScreenMenu(mRenderer);
		mCurrentScreen = (GameScreen*)tempScreenMenu;
		tempScreenMenu = NULL;
		break;

	case SCREEN_LEVEL1:
		tempScreenLevel1 = new GameScreenLevel1(mRenderer);
		mCurrentScreen = (GameScreen*)tempScreenLevel1;
		tempScreenLevel1 = NULL;
		break;

	case SCREEN_LEVEL2:
		tempScreenLevel2 = new GameScreenLevel2(mRenderer);
		mCurrentScreen = (GameScreen*)tempScreenLevel2;
		tempScreenLevel2 = NULL;
		break;

	case SCREEN_SCORE:
		tempScreenScore = new GameScreenScore(mRenderer);
		mCurrentScreen = (GameScreen*)tempScreenScore;
		tempScreenScore = NULL;
		break;

	default:
		cout << "Unable To Load Game Screens!" << endl;
		break;
	}
}