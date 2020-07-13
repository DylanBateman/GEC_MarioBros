#include "GameScreen.h"

#include <iostream>

GameScreen::GameScreen(SDL_Renderer* renderer)
{
	mRenderer = renderer;
}

GameScreen::~GameScreen()
{
	mRenderer = NULL;
}

void GameScreen::Render()
{

}

void GameScreen::Update(float deltaTime, SDL_Event e)
{

}

void GameScreen::CheckPaused(SDL_Event e)
{
	switch (e.type)
	{
	case SDL_KEYUP:
		switch (e.key.keysym.sym)
		{
		case SDLK_p:
			Mix_PlayChannel(-1, pauseSound, 0);
			paused = !paused;
			break;
		}
		break;
	}
}

bool GameScreen::LoadAudio()
{
	levelMusic = Mix_LoadMUS("Music/Mario_Overworld.ogg");
	if (levelMusic == NULL)
	{
		cout << "Failed to Load Level Music Error: " << Mix_GetError() << endl;
		return false;
	}

	coinSound = Mix_LoadWAV("Music/SFX/Coin.wav");
	if (coinSound == NULL)
	{
		cout << "Failed to Load Coin Sound! Error: " << Mix_GetError() << endl;
		return false;
	}

	thwompSound = Mix_LoadWAV("Music/SFX/Thwomp.wav");
	if (thwompSound == NULL)
	{
		cout << "Failed to Load Thwomp Sound! Error: " << Mix_GetError() << endl;
		return false;
	}

	kickSound = Mix_LoadWAV("Music/SFX/Kick.wav");
	if (kickSound == NULL)
	{
		cout << "Failed to Load Kick Sound! Error: " << Mix_GetError() << endl;
		return false;
	}

	deathSound = Mix_LoadWAV("Music/SFX/Death.wav");
	if (deathSound == NULL)
	{
		cout << "Failed to Load Death Sound! Error: " << Mix_GetError() << endl;
		return false;
	}

	levelWinSound = Mix_LoadWAV("Music/SFX/LevelWin.wav");
	if (levelWinSound == NULL)
	{
		cout << "Failed to Load Level Win Sound! Error: " << Mix_GetError() << endl;
		return false;
	}

	gameOverSound = Mix_LoadWAV("Music/SFX/GameOver.wav");
	if (gameOverSound == NULL)
	{
		cout << "Failed to Load Game Over Sound! Error: " << Mix_GetError() << endl;
		return false;
	}

	pauseSound = Mix_LoadWAV("Music/SFX/Pause.wav");
	if (pauseSound == NULL)
	{
		cout << "Failed to Load Pause Sound! Error: " << Mix_GetError() << endl;
		return false;
	}

	finalScoreSound = Mix_LoadWAV("Music/SFX/FinalScores.wav");
	if (finalScoreSound == NULL)
	{
		cout << "Failed to Load Final Score Sound! Error: " << Mix_GetError() << endl;
		return false;
	}

	return true;

}

// Stores All Text Used
void GameScreen::LoadFont()
{
	fontLarge = TTF_OpenFont("Font/MarioBros_Font.ttf", 32);
	fontMedium = TTF_OpenFont("Font/MarioBros_Font.ttf", 16);
	fontSmall = TTF_OpenFont("Font/MarioBros_Font.ttf", 10);

	colorWhite = { 255, 255, 255, 255 };

	// Start Text
	startTextSurface = TTF_RenderText_Solid(fontMedium, "'ENTER' TO START", colorWhite);
	startText = SDL_CreateTextureFromSurface(mRenderer, startTextSurface);
	SDL_QueryTexture(startText, NULL, NULL, &startTextRect.w, &startTextRect.h);

	SDL_FreeSurface(startTextSurface);
	startTextSurface = nullptr;

	// Final Score Title Text
	finalScoresTextSurface = TTF_RenderText_Solid(fontLarge, "FINAL SCORES", colorWhite);
	finalScoresText = SDL_CreateTextureFromSurface(mRenderer, finalScoresTextSurface);
	SDL_QueryTexture(finalScoresText, NULL, NULL, &finalScoresTextRect.w, &finalScoresTextRect.h);

	SDL_FreeSurface(finalScoresTextSurface);
	finalScoresTextSurface = nullptr;


	// Paused Text
	pauseTextSurface = TTF_RenderText_Solid(fontLarge, "Paused!", colorWhite);
	pauseText = SDL_CreateTextureFromSurface(mRenderer, pauseTextSurface);
	SDL_QueryTexture(pauseText, NULL, NULL, &pauseTextRect.w, &pauseTextRect.h);

	SDL_FreeSurface(pauseTextSurface);
	pauseTextSurface = nullptr;

	// Mario On-Screen Score
	marioTextSurface = TTF_RenderText_Solid(fontSmall, "mario score ", colorWhite);
	marioText = SDL_CreateTextureFromSurface(mRenderer, marioTextSurface);
	SDL_QueryTexture(marioText, NULL, NULL, &marioTextRect.w, &marioTextRect.h);

	SDL_FreeSurface(marioTextSurface);
	marioTextSurface = nullptr;

	// Mario Score Text
	marioScoreString = to_string(marioScore);
	marioScoreTextSurface = TTF_RenderText_Solid(fontSmall, marioScoreString.c_str(), colorWhite);
	marioScoreText = SDL_CreateTextureFromSurface(mRenderer, marioScoreTextSurface);
	SDL_QueryTexture(marioScoreText, NULL, NULL, &marioScoreTextRect.w, &marioScoreTextRect.h);

	SDL_FreeSurface(marioScoreTextSurface);
	marioScoreTextSurface = nullptr;

	// MARIO Final Score Text
	marioFinalScoreString = to_string(marioScore);
	marioFinalScoreTextSurface = TTF_RenderText_Solid(fontMedium, marioFinalScoreString.c_str(), colorWhite);
	marioFinalScoreText = SDL_CreateTextureFromSurface(mRenderer, marioFinalScoreTextSurface);
	SDL_QueryTexture(marioFinalScoreText, NULL, NULL, &marioFinalScoreTextRect.w, &marioFinalScoreTextRect.h);

	SDL_FreeSurface(marioFinalScoreTextSurface);
	marioFinalScoreTextSurface = nullptr;

	// Mario Win Text
	marioWinTextSurface = TTF_RenderText_Solid(fontMedium, "MARIO WINS!", colorWhite);
	marioWinText = SDL_CreateTextureFromSurface(mRenderer, marioWinTextSurface);
	SDL_QueryTexture(marioWinText, NULL, NULL, &marioWinTextRect.w, &marioWinTextRect.h);

	SDL_FreeSurface(marioWinTextSurface);
	marioWinTextSurface = nullptr;

	// Luigi On-Screen Score Text
	luigiTextSurface = TTF_RenderText_Solid(fontSmall, "luigi score ", colorWhite);
	luigiText = SDL_CreateTextureFromSurface(mRenderer, luigiTextSurface);
	SDL_QueryTexture(luigiText, NULL, NULL, &luigiTextRect.w, &luigiTextRect.h);

	SDL_FreeSurface(luigiTextSurface);
	luigiTextSurface = nullptr;

	// Luigi Score Text
	luigiScoreString = to_string(luigiScore);
	luigiScoreTextSurface = TTF_RenderText_Solid(fontSmall, luigiScoreString.c_str(), colorWhite);
	luigiScoreText = SDL_CreateTextureFromSurface(mRenderer, luigiScoreTextSurface);
	SDL_QueryTexture(luigiScoreText, NULL, NULL, &luigiScoreTextRect.w, &luigiScoreTextRect.h);

	SDL_FreeSurface(luigiScoreTextSurface);
	luigiScoreTextSurface = nullptr;

	// Luigi Final Score Text
	luigiFinalScoreString = to_string(luigiScore);
	luigiFinalScoreTextSurface = TTF_RenderText_Solid(fontMedium, luigiFinalScoreString.c_str(), colorWhite);
	luigiFinalScoreText = SDL_CreateTextureFromSurface(mRenderer, luigiFinalScoreTextSurface);
	SDL_QueryTexture(luigiFinalScoreText, NULL, NULL, &luigiFinalScoreTextRect.w, &luigiFinalScoreTextRect.h);

	SDL_FreeSurface(luigiFinalScoreTextSurface);
	luigiFinalScoreTextSurface = nullptr;

	// Luigi Win Text
	luigiWinTextSurface = TTF_RenderText_Solid(fontMedium, "LUIGI WINS!", colorWhite);
	luigiWinText = SDL_CreateTextureFromSurface(mRenderer, luigiWinTextSurface);
	SDL_QueryTexture(luigiWinText, NULL, NULL, &luigiWinTextRect.w, &luigiWinTextRect.h);

	SDL_FreeSurface(luigiWinTextSurface);
	luigiWinTextSurface = nullptr;

	// Game Won Text
	gameWonTextSurface = TTF_RenderText_Solid(fontMedium, "You Collected All The Coins!", colorWhite);
	gameWonText = SDL_CreateTextureFromSurface(mRenderer, gameWonTextSurface);
	SDL_QueryTexture(gameWonText, NULL, NULL, &gameWonTextRect.w, &gameWonTextRect.h);

	SDL_FreeSurface(gameWonTextSurface);
	gameWonTextSurface = nullptr;

	// Game Over Text
	gameOverTextSurface = TTF_RenderText_Solid(fontLarge, "Game Over!", colorWhite);
	gameOverText = SDL_CreateTextureFromSurface(mRenderer, gameOverTextSurface);
	SDL_QueryTexture(gameOverText, NULL, NULL, &gameOverTextRect.w, &gameOverTextRect.h);

	SDL_FreeSurface(gameOverTextSurface);
	gameOverTextSurface = nullptr;

	// Next Level Text
	nextLevelTextSurface = TTF_RenderText_Solid(fontSmall, "'ENTER' - LEVEL 2", colorWhite);
	nextLevelText = SDL_CreateTextureFromSurface(mRenderer, nextLevelTextSurface);
	SDL_QueryTexture(nextLevelText, NULL, NULL, &nextLevelTextRect.w, &nextLevelTextRect.h);

	SDL_FreeSurface(nextLevelTextSurface);
	nextLevelTextSurface = nullptr;

	// View Scores Text
	viewScoresTextSurface = TTF_RenderText_Solid(fontSmall, "'ENTER' - VIEW SCORES", colorWhite);
	viewScoresText = SDL_CreateTextureFromSurface(mRenderer, viewScoresTextSurface);
	SDL_QueryTexture(viewScoresText, NULL, NULL, &viewScoresTextRect.w, &viewScoresTextRect.h);

	SDL_FreeSurface(viewScoresTextSurface);
	viewScoresTextSurface = nullptr;

}

void GameScreen::LoadPlayerScores()
{
	// Mario Score
	marioScoreString = to_string(marioScore);
	marioScoreTextSurface = TTF_RenderText_Solid(fontSmall, marioScoreString.c_str(), colorWhite);
	marioScoreText = SDL_CreateTextureFromSurface(mRenderer, marioScoreTextSurface);
	SDL_QueryTexture(marioScoreText, NULL, NULL, &marioScoreTextRect.w, &marioScoreTextRect.h);

	SDL_FreeSurface(marioScoreTextSurface);
	marioScoreTextSurface = nullptr;

	// Luigi Score
	luigiScoreString = to_string(luigiScore);
	luigiScoreTextSurface = TTF_RenderText_Solid(fontSmall, luigiScoreString.c_str(), colorWhite);
	luigiScoreText = SDL_CreateTextureFromSurface(mRenderer, luigiScoreTextSurface);
	SDL_QueryTexture(luigiScoreText, NULL, NULL, &luigiScoreTextRect.w, &luigiScoreTextRect.h);

	SDL_FreeSurface(luigiScoreTextSurface);
	luigiScoreTextSurface = nullptr;
}

bool GameScreen::OpenOutFiles()
{
	// Mario Files
	marioFileOut.open("Score/MarioScore.txt", ostream::trunc);
	if (!marioFileOut)
	{
		cout << endl << "Failed to open Mario Score File (Writing)" << endl;
		return false;
	}
	else
	{
		cout << endl << "Opened Mario Score File (Writing)" << endl;
		marioFileOut << "0";
	}

	marioFileOut.close();

	// Luigi Files
	luigiFileOut.open("Score/LuigiScore.txt", ostream::trunc);
	if (!luigiFileOut)
	{
		cout << endl << "Failed to Open Luigi Score File (Writing)" << endl;
		return false;
	}
	else
	{
		cout << endl << "Opened Luigi Score File (Writing)" << endl;
		luigiFileOut << "0";
	}

	luigiFileOut.close();

	return true;
}

void GameScreen::WriteToFiles()
{
	marioFileOut.open("Score/MarioScore.txt", ostream::trunc);
	marioFileOut << marioScore;
	marioFileOut.close();

	luigiFileOut.open("Score/LuigiScore.txt", ostream::trunc);
	luigiFileOut << luigiScore;
	luigiFileOut.close();

}

bool GameScreen::OpenInFiles()
{
	// Mario Files
	marioFileIn.open("Score/MarioScore.txt");
	if (!marioFileIn)
	{
		cout << endl << "Failed to Open Mario Score File (Reading)" << endl;
		return false;
	}
	else
	{
		cout << endl << "Opened Mario Score File (Reading)" << endl;
		marioFileIn >> marioScore;
	}

	marioFileIn.close();

	// Luigi Files
	luigiFileIn.open("Score/LuigiScore.txt");
	if (!luigiFileIn)
	{
		cout << endl << "Failed to open Luigi Score File (Reading)" << endl;
		return false;
	}
	else
	{
		cout << endl << "Opened Luigi Score File (Reading)" << endl;
		luigiFileIn >> luigiScore;
	}

	luigiFileIn.close();

	return true;
}

void GameScreen::CharacterCollisions(Character* character1, Character* character2)
{
	if (Collisions::Instance()->Circle(character1, character2))
	{
		int tempPosition;
		int newPosition;

		// Both Players Move
		if (character1->mMovingRight && character2->mMovingLeft)
		{
			tempPosition = character1->GetPosition().x;
			newPosition = tempPosition -= COLLISION_FORCE;
			character1->SetPosition(Vector2D(newPosition, character1->GetPosition().y));

			tempPosition = character2->GetPosition().x;
			newPosition = tempPosition += COLLISION_FORCE;
			character2->SetPosition(Vector2D(newPosition, character2->GetPosition().y));
		}
		else if (character1->mMovingLeft && character2->mMovingRight)
		{
			tempPosition = character1->GetPosition().x;
			newPosition = tempPosition += COLLISION_FORCE;
			character1->SetPosition(Vector2D(newPosition, character1->GetPosition().y));

			tempPosition = character2->GetPosition().x;
			newPosition = tempPosition -= COLLISION_FORCE;
			character2->SetPosition(Vector2D(newPosition, character2->GetPosition().y));
		}

		// Only Mario Moving
		else if (character1->mMovingRight)
		{
			tempPosition = character1->GetPosition().x;
			newPosition = tempPosition -= COLLISION_FORCE;
			character1->SetPosition(Vector2D(newPosition, character1->GetPosition().y));
		}
		else if (character1->mMovingLeft)
		{
			tempPosition = character1->GetPosition().x;
			newPosition = tempPosition += COLLISION_FORCE;
			character1->SetPosition(Vector2D(newPosition, character1->GetPosition().y));
		}
		// Only Luigi Moving
		else if (character2->mMovingRight)
		{
			tempPosition = character2->GetPosition().x;
			newPosition = tempPosition -= COLLISION_FORCE;
			character2->SetPosition(Vector2D(newPosition, character2->GetPosition().y));
		}
		else if (character2->mMovingLeft)
		{
			tempPosition = character2->GetPosition().x;
			newPosition = tempPosition += COLLISION_FORCE;
			character2->SetPosition(Vector2D(newPosition, character2->GetPosition().y));
		}
		// Both Players Moving Same Direction
		else if ((character1->mMovingRight && character2->mMovingRight) || (character1->mMovingLeft && character2->mMovingLeft))
		{
		}
	}
}