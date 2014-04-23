#include <stdio.h>
#include <allegro5/allegro.h>
#include "gamesong.h"
#include "gamesettings.h"

int main(int argc, char **argv) {
	ALLEGRO_DISPLAY *display = NULL;
	GameSettings *gameSettings = new GameSettings();

	// Initalize some game settings
	gameSettings->SetFPS(60);
	gameSettings->SetScreenDimmensions(1280, 720);

	/* The al_init function is a automatic function to replace al_install_system
	 * the function initializes allegro and must be placed before any other calls
	 * to allegro.
	*/

	// Error handler
	if (!al_init()) {
		fprintf(stderr, "failed to initalize allegro!\n");
		return -1;
	}

	// Very simply create a display of x y dimmensions
	display = al_create_display(gameSettings->GetSCREEN_W(), gameSettings->GetSCREEN_H());
	// Error handler
	if (!display) {
		fprintf(stderr, "failed to create display!\n");
		return -1;
	}

	GameSong *gameSong = new GameSong(display, gameSettings);
	gameSong->StartSong();

	al_destroy_display(display);

	return 0;
}
