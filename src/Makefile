CC=g++
CFLAGS=-g -I.
CXXFLAGS := -std=c++11 $(CXXFLAGS)
DEPS = Socket.h Chat.h
OBJ = Socket.o Chat.o
LIBS=-lpthread

%.o: %.cc $(DEPS)
	$(CC) -g -c -o $@ $< $(CFLAGS)

all: cs cc
Chat.o: Chat.cc
	g++ -c Chat.cc $(CXXFLAGS)
ChatServer.o: ChatServer.cc
	g++ -c ChatServer.cc $(CXXFLAGS)
ChatClient.o: ChatClient.cc
	g++ -c ChatClient.cc $(CXXFLAGS)

cs: $(OBJ) ChatServer.o
	g++ -o $@ $^ $(CFLAGS) $(LIBS)

cc: $(OBJ) ChatClient.o
	g++ -o $@ $^ $(CFLAGS) $(LIBS)


.PHONY: clean

clean:
	rm -f *.o cs cc

