CC=g++
LIBS=`pkg-config --cflags --libs allegro-5.0 allegro_primitives-5.0`
SOURCES=xfrets.cpp note.cpp gamesong.cpp gamesettings.cpp song.cpp

all: $(SOURCES)
	$(CC) $(SOURCES) -pthread -g -std=c++11 -lrt -lm $(LIBS) -o xfrets
