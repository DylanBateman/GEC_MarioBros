#ifndef _GAMESCREENSCORE_H
#define _GAMESCREENSCORE_H

#include "GameScreen.h"
#include "Texture2D.h"

class Texture2D;

class GameScreenScore : GameScreen
{
public:
	GameScreenScore(SDL_Renderer* renderer);
	~GameScreenScore();

	void Render();
	void Update(float deltaTime, SDL_Event e);

private:
	bool SetUpScreen();

	Texture2D* mBackground;

	Texture2D* characterMario;
	Texture2D* characterLuigi;
	Texture2D* coinTexture;
};
#endif // !_GAMESCREENSCORE_H