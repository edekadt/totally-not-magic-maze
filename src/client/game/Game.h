#include <vector>
#include "../network/Serializable.h"
#include "../network/Socket.h"

namespace ecs {
class Manager;
}

class MapSystem;
class RenderSystem;
class HeroSystem;


class GameMessage: public Serializable
{
public:
    static const size_t MESSAGE_SIZE = sizeof(char) * 144 + sizeof(uint8_t);

    enum MessageType
    {
        CLIENTJOINED  	= 0,
        MOVEMENT 		= 1,
		UPDATEPOS		= 2,
        NEWMAP	 		= 3,
        UPDATEEXITS     = 4,
        CLIENTLEFT 		= 5
    };

    GameMessage(){};
    ~GameMessage(){};

    GameMessage(const char& d):direction(d),type(MessageType::MOVEMENT){};

    void to_bin();

    int from_bin(char * bobj);

    uint8_t type;
	union {
		char direction; // Up, Down, Left, Right, None
        int positions [8]; // 2 coords for each of the 4 heroes or exits
    	char map [144];
	};
};

class Game {
public:
	Game(const char * s, const char * p);
	virtual ~Game();
	void init();
	void start();
    void sendMessage(GameMessage& message); 
private:
	ecs::Manager *mngr_;

	MapSystem *mapSys_;
	RenderSystem *renderSys_;
	HeroSystem* heroSys_;
	
    Socket* server;

    Socket socket;
	void net_thread();
    void joinGame();
    void leaveGame();

    bool connectionEstablished = false;
};

