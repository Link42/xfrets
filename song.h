#ifndef SONG_H
#define SONG_H
#include <string>

class Song
{
public:
	Song();
	int loadSong(const char *inputFile);

private:
	typedef unsigned char BYTE;
	BYTE *fileBuf;
	bool verbose;
	
	long getFileSize(FILE *file); // Find size of input file
	uint concatBytes(int startIndex, int endIndex); // Concatenate X amount of bytes
	int vlength(int startIndex); // Find a vlength
};

#endif
