#include "GameScreenMenu.h"

#include <iostream>

GameScreenMenu::GameScreenMenu(SDL_Renderer* renderer) : GameScreen(renderer)
{
	SetUpMenu();
}

GameScreenMenu::~GameScreenMenu()
{
	// Game Objects
	delete mBackground;
	mBackground = NULL;

	delete mMarioLogo;
	mMarioLogo = NULL;

	// Text Textures
	SDL_DestroyTexture(startText);
}

// Renders Each Object and Sets Position On Screen
void GameScreenMenu::Render()
{
	mBackground->Render(Vector2D(), SDL_FLIP_NONE);
	mMarioLogo->Render(Vector2D(((SCREEN_WIDTH / 2) - (mMarioLogo->GetWidth() / 2)), 50), SDL_FLIP_NONE);

	startTextRect.x = (SCREEN_WIDTH / 2) - (startTextRect.w / 2);
	startTextRect.y = ((SCREEN_HEIGHT / 2) - (startTextRect.h / 2)) + 60;
	SDL_RenderCopy(mRenderer, startText, NULL, &startTextRect);
}

void GameScreenMenu::Update(float deltaTime, SDL_Event e)
{
}

bool GameScreenMenu::SetUpMenu()
{
	// Loads Text For This Screen
	LoadFont();

	// Loads Images For This Screen
	mBackground = new Texture2D(mRenderer);
	if (!mBackground->LoadFromFile("Images/GameScreens/Background.png"))
	{
		cout << "Failed to load background texture!";
		return false;
	}

	mMarioLogo = new Texture2D(mRenderer);
	if (!mMarioLogo->LoadFromFile("Images/GameScreens/MarioLogo.png"))
	{
		cout << "Failed to load mario logo!";
		return false;
	}

	return true;
}