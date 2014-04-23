#include "song.h"
#include <iostream>
#include <iomanip>
#include <math.h>

// Song constructor
Song::Song() {}

int Song::loadSong(const char *inputFile) {

	// Testing
	verbose = true;

	FILE *file = NULL;

	// MIDI file paramters:
	int step = 0; // Progress marker for file itterations
	
	// Header Chunk Variables
	uint header_length;
	uint track_format;
	uint number_of_tracks;
	uint file_division;



	// Open file in binary mode
	file = fopen(inputFile, "rb");

	// Get the size of the file in bytes
	long fileSize = getFileSize(file);

	// Allocate space in the buffer for the while file
	fileBuf = new BYTE[fileSize];
	
	// Read the file in to the buffer
	fread(fileBuf, fileSize, 1, file);

	// Set base to 16(Hex)
	std::setbase(16);

	//==============================
	//         HEADER CHUNK
	//==============================        
	
	// Check for MIDI Header, bytes 0-3
	if (concatBytes(0,3) != 0x4d546864) {
		printf("Error: MIDI file header invalid");
		return -1;
	}
	verbose ? printf("MIDI Header: %X\n", concatBytes(0, 3)) : 0; // Verbose Statment

	// Header length, bytes 4-7
	header_length = concatBytes(4, 7);
	verbose ? printf("Header Length: %X\n", header_length) : 0; // Verbose Statment

	// Format of track, bytes 8-9
	track_format = concatBytes(8, 9);
	verbose ? printf("Track Format: %X\n", track_format) : 0; // Verbose Statment

	// Number of tracks, bytes 10-11
	number_of_tracks = concatBytes(10, 11);;
	verbose ? printf("Number of Tracks: %X\n", number_of_tracks) : 0; // Verbose Statment

	// File divison i.e. Units Per Beat, bytes 12-13
	file_division = concatBytes(12, 13);
	verbose ? printf("File Division: %X\n", file_division) : 0; // Verbose Statment

	// Set the step
	step = 14;


	//==============================        
	//      TRACK CHUNK LOOP
	//==============================
	int vlength_length = 0;
	int current_track = 0;
	for ( int i = step; i < fileSize; i++ ) {
	
		//======================
		//     TRACK CHUNK
		//======================
		if (concatBytes(step, step+3) == 0x4d54726b) {
			verbose ? printf("Track (%d) Header: %X\n",current_track, concatBytes(step, step+3)) : 0; // Verbose Statment
		
			step += 4;
			uint track_length = concatBytes(step, step+3);
			verbose ? printf("Track (%d) Length: %X\n", current_track, track_length) : 0; // Verbose Statment

			step += 4; // DO A BARREL ROLL

			int track_event_index = -1;

			//=============
			// TRACK EVENT
			// ============
			bool endOfTrack = false;
			while ( !endOfTrack ) {
				track_event_index++; // Keep track of which track event we are at

				vlength_length = vlength(step); // Find the length of the vlength value
				uint track_event_length = concatBytes(step, step+vlength_length);
				verbose ? printf("\nTrck Event (%d) Length: %X\n", track_event_index, track_event_length) : 0; // Verbose Statment
				step += (vlength_length + 1); // Add the one as the length could be 0


				// Check for Headers
				bool isMeta = false;
				bool isMidi = false;

				// Initalize Running Status (Headers omitted if same note is infferd)
				uint runningStatus;
				switch (fileBuf[step]) {
					case 0xff:
						// META
						isMeta = true;
						verbose ? printf("Meta Event (%d) Header \"FF\" Found\n", track_event_index) : 0; // Verbose Statment
						break;
					case 0x90:
						// MIDI
						isMidi = true;
						verbose ? printf("Midi Event (%d) Header \"90\" Found\n", track_event_index) : 0; // Verbose Statment
						break;
				}
				step++;
				
				//============
				// META EVENT
				//============
				if (isMeta) {
					unsigned char meta_type = fileBuf[step];
	
					// Advance step so we dont have to do it every time in the case
					step++;
	
					// Variables that may be used in the case
					uint sequence_length;
					uint meta_tempo;
					uint meta_time_sig_numerator;
					uint meta_time_sig_demoninator;
					uint meta_time_sig_metronome;
					uint meta_time_sig_32notes_per_beat;
	
					switch (meta_type) {
						// Sequence Number
						case 0x00 : 
							printf("0x00 unimplemented\n");
							break;
						// Text Event
						case 0x01 : 
							printf("0x01 unimplemented\n");
							break;
						// Copyright Notice
						case 0x02 : 
							printf("0x02 unimplemented\n");
							break;
						// Sequence or Track Name
						case 0x03 :
							verbose ? printf("Meta Event (%d) Type: 0x03 \"Sequence or Track Name\"\n", track_event_index) :0; // Verbose Statement
	
							vlength_length = vlength(step);
							sequence_length = concatBytes(step, step + vlength_length);
							verbose ? printf("Meta Event (%d) Sequence Length: %d\n", track_event_index, sequence_length) : 0; // Verbose Statement
							step += (vlength_length + 1); // Advance past the parameter
	
							verbose ? printf("Meta Event (%d) Sequence Name: ", track_event_index) : 0; // Verbose Statement
							for  ( int j = 0; j < sequence_length; j++ ) {
								printf("%c", fileBuf[step + j]);
							}
							step += sequence_length; // Advance past the meta event data
							verbose ? printf("\n") : 0;
							break;
						// Instrument Name
						case 0x04 : 
							printf("0x04 unimplemented\n");
							break;
						// Lyric Text
						case 0x05 : 
							printf("0x05 unimplemented\n");
							break;
						// Marker Text
						case 0x06 :
							printf("0x06 unimplemented\n");
						       	break;
						// Cue Point
						case 0x07 : 
							printf("0x07 unimplemented\n");
							break;
						// MIDI Channel prefix assignment
						case 0x20 : 
							printf("0x20 unimplemented\n");
							break;
						// End of Track
						case 0x2f : 
							verbose ? printf("Meta Event (%d) Type: 0x2f \"End of Track\"\n\n", track_event_index) : 0;
							step++; // The End of Track has an arbitary 0x00 in it, we should skip past it
							current_track++;
							endOfTrack = true;
							break;
						// Tempo Setting
						case 0x51 :
							verbose ? printf("Meta Event (%d) Type: 0x51 \"Tempo Setting\"\n", track_event_index) : 0; // Verbose Statement
							
							(fileBuf[step] != 0x03 && verbose) ? printf("Meta Event (%d) Type: 0x51 has invalid length!\n", track_event_index) : 0; // Verbose Statment
							step++;
							
							meta_tempo = concatBytes(step, step+2);
							verbose ? printf("Meta Event (%d) Tempo: %X (Calculated Tempo): %d\n", track_event_index, meta_tempo, (60000000/meta_tempo)) : 0;
							step += 3;
							break;
						// SMPTE Offset
						case 0x54 : 
							printf("0x54 unimplemented\n");
							break;
						// Time Signature
						case 0x58 : 
							(fileBuf[step] != 0x04 && verbose) ? printf("Meta Event (%d) Type: 0x58 has invalid length!\n", track_event_index) : 0; // Verbose Statment
							step++;
	
							meta_time_sig_numerator = fileBuf[step];
							step++;
							meta_time_sig_demoninator = fileBuf[step];
							step++;
							verbose ? printf("Meta Event (%d) Signature: %d/%d\n", track_event_index, meta_time_sig_numerator, pow(2, meta_time_sig_demoninator)) : 0;
	
							meta_time_sig_metronome = fileBuf[step];
							verbose ? printf("Meta Event (%d) Metronome: %d\n", track_event_index, meta_time_sig_metronome) : 0;
							step++;
	
							meta_time_sig_32notes_per_beat = fileBuf[step];
							verbose ? printf("Meta Event (%d) 32 Notes Per Beat: %d\n", track_event_index, meta_time_sig_32notes_per_beat) : 0;
							step++;
							break;
						// Key Signature
						case 0x59 :
							printf("0x59 unimplemented\n");
						       	break;
						// Sequencer Sepcific Event
						case 0x7f : 
							printf("0x7f unimplemented\n");
							break;
					}
				}
			
				//============
				// MIDI EVENT
				//============

				if (isMidi) {
					unsigned char midi_type = fileBuf[step];
	
					// Advance step so we dont have to do it every time in the case
					step++;

					switch (midi_type) {
						// Note On
						case 0x90 :
						
							break;
					}

				}
			}
		}	
		step++;
	}





	//printf("%X\n", file_division);
}

int Song::vlength(int startIndex) {
	int amountOfBytes = 0;
	while (fileBuf[startIndex + amountOfBytes] & 0x80) {
		amountOfBytes++;
	}

	return amountOfBytes;
}

uint Song::concatBytes(int startIndex, int endIndex) {
	int j = 0; // Counter for Bytetets
	uint result = 0; // Return value

	for ( int i = endIndex; i >= startIndex; i--) {
		result += (fileBuf[i] << j);
		j += 8;
	}
       	return result;
}

long Song::getFileSize(FILE *file) {
	long lCurPos, lEndPos;
	lCurPos = ftell(file);
	fseek(file, 0, 2);
	lEndPos = ftell(file);
	fseek(file, lCurPos, 0);
	return lEndPos;
}
