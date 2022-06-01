CC=g++
CFLAGS=-g -I.
CXXFLAGS := -std=c++17 $(CXXFLAGS)
DEPS = Socket.h Chat.h
OBJ = Socket.o Chat.o
LIBS=-lpthread -lSDL2

%.o: %.cc $(DEPS)
	$(CC) -Wall -g -c -o $@ $< $(CFLAGS)

all: cs cc
%.o: %.cpp
	g++ -Wall -c -g $@ $^ $(CXXFLAGS)

cs: $(OBJ) ChatServer.o
	g++ -o $@ $^ $(CFLAGS) $(LIBS)

cc: $(OBJ) ChatClient.o
	g++ -o $@ $^ $(CFLAGS) $(LIBS)


.PHONY: clean

clean:
	rm -f *.o cs cc