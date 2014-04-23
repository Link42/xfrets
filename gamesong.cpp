#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <vector>
#include <algorithm>
#include "note.h"
#include "gamesong.h"
#include "gamesettings.h"
#include "song.h"

// Constructor
GameSong::GameSong(ALLEGRO_DISPLAY *inputDisplay, GameSettings *inputGameSettings) {
	display = inputDisplay;
	gameSettings = inputGameSettings;
}


// Start function
int GameSong::StartSong() {
	ALLEGRO_EVENT_QUEUE *event_queue = NULL;
	ALLEGRO_TIMER *timer = NULL, *createTimer = NULL;
	ALLEGRO_BITMAP *collum[5];
	bool redraw = true;
	std::vector<Note*> NoteList;
	srand(time(NULL)); // TEMP, seed for random functions

	// Load the Midi file
	const char *filePath = "midi/notes.mid"; // TODO move this to the constructor
	Song *song = new Song();
	song->loadSong(filePath);

	// Create our timers
	timer = al_create_timer(1.0 / gameSettings->GetFPS());
	createTimer = al_create_timer(0.5);

	// Creates empty event queue and register event with it
	event_queue = al_create_event_queue();
	al_register_event_source(event_queue, al_get_display_event_source(display));
	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	al_register_event_source(event_queue, al_get_timer_event_source(createTimer));

	// Create bitmaps for collums
	for ( int i = 0; i < 6; i++) {
		collum[i] = al_create_bitmap(gameSettings->GetSCREEN_W() / 250, gameSettings->GetSCREEN_H() * 0.8);
		al_set_target_bitmap(collum[i]);
		al_clear_to_color(al_map_rgb(255,255,255));
	}
	
	// Draw the backbuffer
	al_set_target_backbuffer(display);
	al_clear_to_color(al_map_rgb(0,0,0));
	
	// Updates both front and back buffers
	al_flip_display();
	
	// Derp Derp
	al_start_timer(timer);
	al_start_timer(createTimer);

	while(1) {
		ALLEGRO_EVENT ev;

		//Wait for something to happen, not to waste cycles I guess?
		al_wait_for_event(event_queue, &ev);

		if (ev.type == ALLEGRO_EVENT_TIMER) {
			// If the regualr game loop timer set off redraw
			if (ev.timer.source == timer) {
				// Drop all notes
				
				std::vector<Note*>::size_type i = 0;
				while ( i < NoteList.size() ) {
					if ( NoteList[i]->DropNote() >= 0.8 ) {	
						delete NoteList[i];
						NoteList.erase( NoteList.begin() + i );
					} else {
						++i;
					}
				}

				//for ( auto &i : NoteList) {
				//	// Destroy note if its at the bottom
				//	if (i->DropNote() <= 0) {
				//		delete i;
				//		i = NULL;
				//	}
				//}

				// And tell it to redraw stuff
				redraw = true;
			}

			// TESTING, spawn block timer
			if (ev.timer.source == createTimer) {
				Note *note = new Note(rand() % 5, rand() % 5);
				NoteList.push_back(note);
			}
		}
		else if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
			// Of course if we closed the window kill shit
			break;
		}

		if (redraw && al_is_event_queue_empty(event_queue)) {
			redraw = false;

			// Derp Derp
			al_clear_to_color(al_map_rgb(0,0,0));

			// Draw the grid lines
			for ( int i = 0; i < 6; i++ ) {
				al_draw_bitmap(collum[i], gameSettings->GetSCREEN_W() * (0.25 + (i * 0.1)),
						gameSettings->GetSCREEN_H() * 0.1, 0);
			}


			// Draw the notes
			for ( auto &i : NoteList ) {
				al_draw_filled_circle(
					gameSettings->GetSCREEN_W() * (0.3 + (i->GetRow() * 0.1)), 
					gameSettings->GetSCREEN_H() * (0.1 + (i->GetHeight())),
					gameSettings->GetSCREEN_W() * 0.035,
					i->GetColor());
			}
			
			al_flip_display();
		}

	}

	al_destroy_timer(timer);
	al_destroy_timer(createTimer);
	al_destroy_event_queue(event_queue);

	return 1;
}
