#ifndef _GAMESCREEN_H
#define _GAMESCREEN_H

#include <SDL.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <string>
#include <fstream>

#include "Collisions.h"
#include "Constants.h"

using namespace std;

class Texture2D;

class GameScreen
{
public:
	GameScreen(SDL_Renderer* renderer);
	~GameScreen();

	virtual void Render();
	virtual void Update(float deltaTime, SDL_Event e);

	void CharacterCollisions(Character* characterMario, Character* characterLuigi);

	// Paused Related
	void CheckPaused(SDL_Event e);
	bool paused;

	// Score Related
	int marioScore;
	int luigiScore;

	// File Handling
	bool OpenOutFiles();
	void WriteToFiles();
	ofstream marioFileOut;
	ofstream luigiFileOut;

	bool OpenInFiles();
	ifstream marioFileIn;
	ifstream luigiFileIn;

	// Font Related
	void LoadFont();
	void LoadPlayerScores();

	// Text Sizes
	TTF_Font* fontLarge;
	TTF_Font* fontMedium;
	TTF_Font* fontSmall;

	// Text Colour
	SDL_Color colorWhite;

	// Level Start Text
	SDL_Surface* startTextSurface;
	SDL_Texture* startText;
	SDL_Rect startTextRect;

	// Pause Menu Text
	SDL_Surface* pauseTextSurface;
	SDL_Texture* pauseText;
	SDL_Rect pauseTextRect;

	// Mario Text
	SDL_Surface* marioTextSurface;
	SDL_Texture* marioText;
	SDL_Rect marioTextRect;

	//Mario In-Game Displayed Score
	SDL_Surface* marioScoreTextSurface;
	SDL_Texture* marioScoreText;
	SDL_Rect marioScoreTextRect;
	string marioScoreString;

	// Mario Final Score
	SDL_Surface* marioFinalScoreTextSurface;
	SDL_Texture* marioFinalScoreText;
	SDL_Rect marioFinalScoreTextRect;
	string marioFinalScoreString;

	// Mario Wins Text
	SDL_Surface* marioWinTextSurface;
	SDL_Texture* marioWinText;
	SDL_Rect marioWinTextRect;

	// Luigi Text
	SDL_Surface* luigiTextSurface;
	SDL_Texture* luigiText;
	SDL_Rect luigiTextRect;

	// Luigi In-Game Displayed Score
	SDL_Surface* luigiScoreTextSurface;
	SDL_Texture* luigiScoreText;
	SDL_Rect luigiScoreTextRect;
	string luigiScoreString;

	// Luigi Final Score
	SDL_Surface* luigiFinalScoreTextSurface;
	SDL_Texture* luigiFinalScoreText;
	SDL_Rect luigiFinalScoreTextRect;
	string luigiFinalScoreString;

	// Luigi Wins Text
	SDL_Surface* luigiWinTextSurface;
	SDL_Texture* luigiWinText;
	SDL_Rect luigiWinTextRect;

	// Final Scores Title Text
	SDL_Surface* finalScoresTextSurface;
	SDL_Texture* finalScoresText;
	SDL_Rect finalScoresTextRect;

	// Game Won Text
	SDL_Surface* gameWonTextSurface;
	SDL_Texture* gameWonText;
	SDL_Rect gameWonTextRect;

	// Level End Text
	SDL_Surface* gameOverTextSurface;
	SDL_Texture* gameOverText;
	SDL_Rect gameOverTextRect;

	// Next Level Text
	SDL_Surface* nextLevelTextSurface;
	SDL_Texture* nextLevelText;
	SDL_Rect nextLevelTextRect;

	// View Scores Text
	SDL_Surface* viewScoresTextSurface;
	SDL_Texture* viewScoresText;
	SDL_Rect viewScoresTextRect;


	// Audio Related
	bool LoadAudio();

	Mix_Music* levelMusic;
	bool startTimer;
	float pauseMusic;
	float startGameTimer;

	Mix_Chunk* coinSound;
	Mix_Chunk* deathSound;
	Mix_Chunk* levelWinSound;
	Mix_Chunk* gameOverSound;
	Mix_Chunk* kickSound;
	Mix_Chunk* pauseSound;
	Mix_Chunk* thwompSound;
	Mix_Chunk* finalScoreSound;

private:

protected:
	SDL_Renderer* mRenderer;
};

#endif // !_GAMESCREEN_H