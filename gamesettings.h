#ifndef GAMESETTINGS_H
#define GAMESETTINGS_H

class GameSettings
{
public:
	GameSettings();

	// Getters
	float GetFPS();
	int GetSCREEN_W();
	int GetSCREEN_H();

	// Setters
	void SetFPS(float inputFPS);
	void SetScreenDimmensions(int inputWidth, int inputHeight);

private:
	// Values
	float FPS;
	int SCREEN_W;
	int SCREEN_H;

};

#endif
