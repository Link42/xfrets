#ifndef GAMESONG_H
#define GAMESONG_H
#include <allegro5/allegro.h>
#include "gamesettings.h"
#include "note.h"

class GameSong
{
public:
	GameSong(ALLEGRO_DISPLAY *inputDisplay, GameSettings *inputGameSettings);
	
	// Other
	int StartSong();

private:
	ALLEGRO_DISPLAY *display;
	GameSettings *gameSettings;
};

#endif
