#include "gamesettings.h"

// GameSettings constructor
GameSettings::GameSettings() {}

// Getters
float GameSettings::GetFPS() {
	return FPS;
}

int GameSettings::GetSCREEN_W() {
	return SCREEN_W;
}

int GameSettings::GetSCREEN_H() {
	return SCREEN_H;
}

// Setters
void GameSettings::SetFPS(float inputFPS) {
	FPS = inputFPS;
}

void GameSettings::SetScreenDimmensions(int inputWidth, int inputHeight) {
	SCREEN_W = inputWidth;
	SCREEN_H = inputHeight;
}
