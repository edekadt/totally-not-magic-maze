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
        CLIENTLEFT 		= 4
    };

    GameMessage(){};
    ~GameMessage(){};

    GameMessage(const char& d):direction(d),type(MessageType::MOVEMENT){};

    void to_bin();

    int from_bin(char * bobj);

    uint8_t type;
	union {
		char direction; // Up, Down, Left, Right, None
    	std::vector<std::vector<char>> map;
	};
};

class Game {
public:
	Game(const char * s, const char * p);
	virtual ~Game();
	void init();
	void start();
private:
	ecs::Manager *mngr_;

	MapSystem *mapSys_;
	RenderSystem *renderSys_;
	HeroSystem* heroSys_;
	
    Socket* client;

    Socket socket;
	void do_messages();

};

