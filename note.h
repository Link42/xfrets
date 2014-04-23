#ifndef NOTE_H
#define NOTE_H
#include <allegro5/allegro.h>

class Note
{
public:
	Note(int initialRow, int initialColor);
	
	// Getters
	ALLEGRO_BITMAP *GetBitmap();
	ALLEGRO_COLOR GetColor();
	int GetRow();
	float GetHeight();
	
	// Setters
	void SetColor(int initialColor);

	// Other
	float DropNote();

private:
	int row;
	float height;

	// Private objects
	ALLEGRO_BITMAP *noteBitmap;
	ALLEGRO_COLOR noteColor;
};

#endif
