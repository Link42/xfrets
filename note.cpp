#include <allegro5/allegro.h>
#include "note.h"

// Note constructor
Note::Note(int initialRow, int initialColor) {

	height = 0.0;
	noteBitmap = al_create_bitmap(20, 20);

	row = initialRow;
	SetColor(initialColor);
}


// Getters
//
ALLEGRO_BITMAP* Note::GetBitmap() {
	return noteBitmap;
}

ALLEGRO_COLOR Note::GetColor() {
	return noteColor;
}

float Note::GetHeight() {
	return height;
}

int Note::GetRow() {
	return row;
}


// Setters

void Note::SetColor(int colorIndex) {
	
	switch (colorIndex) {
		// Red
		case 1 : noteColor = al_map_rgb(255,0,0);
			  break;
		// Green
		case 2 : noteColor = al_map_rgb(0,255,0);
			  break;
		// Blue
		case 3 : noteColor = al_map_rgb(0,0,255);
			  break;
		// Yellow
		case 4 : noteColor = al_map_rgb(255,255,0);
			  break;
		// Teal
		case 5 : noteColor = al_map_rgb(0,255,255);
			  break;
	}
}


// Others

float Note::DropNote() {
	height += 0.005;
	return height;
}
