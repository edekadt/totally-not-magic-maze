CC=g++
CFLAGS=-g -I.
CXXFLAGS := -std=c++17 $(CXXFLAGS)
DEPS = ./network/Socket.h ./network/Chat.h
OBJ = Socket.o Chat.o
LIBS=-lpthread -lSDL2

%.o: %.cc $(DEPS)
	$(CC) -Wall -g -c -o $@ $< $(CFLAGS)

all: cs cc
Chat.o: Chat.cc
	g++ -Wall -c -g ./network/Chat.cc $(CXXFLAGS)
ChatServer.o: ./network/ChatServer.cc
	g++ -Wall -c -g ./network/ChatServer.cc $(CXXFLAGS)
ChatClient.o: ./network/ChatClient.cc
	g++ -Wall -c -g ./network/ChatClient.cc $(CXXFLAGS)

cs: $(OBJ) ChatServer.o
	g++ -o $@ $^ $(CFLAGS) $(LIBS)

cc: $(OBJ) ChatClient.o
	g++ -o $@ $^ $(CFLAGS) $(LIBS)


.PHONY: clean

clean:
	rm -f *.o cs cc

