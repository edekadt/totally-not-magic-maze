#include "Game.h"

#include "../ecs/Manager.h"
#include "../sdlutils/InputHandler.h"
#include "../sdlutils/SDLUtils.h"

#include "../systems/MapSystem.h"
#include "../systems/RenderSystem.h"
#include "../systems/HeroSystem.h"

#include "../utils/Vector2D.h"
#include "../utils/Collisions.h"

#include <thread>
#include <chrono>

using ecs::Manager;


void GameMessage::to_bin()
{
    alloc_data(MESSAGE_SIZE);

    memset(_data, 0, MESSAGE_SIZE);

    //Serializar los campos type, nick y message en el buffer _data
    char* tmp = _data;
    memcpy(tmp, &type, sizeof(uint8_t));
    tmp += sizeof(uint8_t);
	switch(type)
	{
	case 1: // MOVEMENT
	    memcpy(tmp, &direction, sizeof(char));
		break;
	}
}

int GameMessage::from_bin(char * bobj)
{
    alloc_data(MESSAGE_SIZE);

    memcpy(static_cast<void *>(_data), bobj, MESSAGE_SIZE);

    //Reconstruir la clase usando el buffer _data
    char* tmp = _data;

    memcpy(&type, tmp,sizeof(uint8_t));
    tmp += sizeof(uint8_t);
	switch(type)
	{
	case 2:	// UPDATE POS
    	memcpy(&positions[0], tmp, sizeof(char) * 144);
		break;
	case 3:	// NEW MAP
    	memcpy(&map[0], tmp, sizeof(char) * 144);
		break;
	case 4:	// exits
    	memcpy(&positions[0], tmp, sizeof(char) * 144);
		break;
	}
    tmp += sizeof(char) * 8;
	

    return 0;
}

Game::Game(const char * s, const char * p) :
		mngr_(nullptr), //
	    heroSys_(nullptr), //
		mapSys_(nullptr), //
		renderSys_(nullptr),
		socket(s, p) {std::cout<<"Client constructor\n";}

Game::~Game() {
	delete mngr_;
}

void Game::init() {
	std::cout << "init\n";

	// Initialize the SDLUtils singleton
	SDLUtils::init("Magic Maze", 600, 600,
			"resources/config/material.resources.json");

	sdlutils().hideCursor();

	// Create the manager
	mngr_ = new Manager();

	mapSys_ = mngr_->addSystem<MapSystem>();
	heroSys_ = mngr_->addSystem<HeroSystem>();
	renderSys_ = mngr_->addSystem<RenderSystem>();
	
}

void Game::start() {
	std::cout<<"Start\n";
	heroSys_->game = this;

	// a boolean to exit the loop
	bool exit = false;

	auto &ihdlr = ih();
	std::cout<<"Waiting for connection\n";

	std::thread waitForConnetion(&Game::net_thread, this);
	joinGame();
	
	waitForConnetion.join();
	connectionEstablished = true;
	std::cout<<"Connection established\n";

	mngr_->getSystem<MapSystem>()->initSystem();
	std::cout<<"Map inited\n";
	mngr_->getSystem<HeroSystem>()->initSystem();
	std::cout<<"Heroes inited\n";
	mngr_->getSystem<RenderSystem>()->initSystem();
	std::cout<<"Render inited\n";


	std::thread message_thread(&Game::net_thread, this);

	while (!exit) {
		Uint32 startTime = sdlutils().currRealTime();


		std::cout<<"Loop\n";

		// refresh the input handler
		ihdlr.refresh();

		if (ihdlr.isKeyDown(SDL_SCANCODE_ESCAPE)) {
			exit = true;
			continue;
		}

		mngr_->refresh();

		heroSys_->update();
		mapSys_->update();	
		sdlutils().clearRenderer();
		renderSys_->update();
		sdlutils().presentRenderer();

		Uint32 frameTime = sdlutils().currRealTime() - startTime;

		if (frameTime < 10)
			SDL_Delay(10 - frameTime);
	}
	leaveGame();
}

void Game::net_thread()
{
	while (true)
    {	
		server = 0;
        GameMessage msg;
        int isMessage = socket.recv(msg, server);

        if (isMessage != -1)
        {
            switch(msg.type)
            {
                case GameMessage::CLIENTJOINED:
                case GameMessage::CLIENTLEFT:
                case GameMessage::MOVEMENT:
                	break;	// server should never receive a new map or position update message
				case GameMessage::NEWMAP:
					mapSys_->newMap(msg.map);
					if (!connectionEstablished)
					{
						throw new std::exception();
						connectionEstablished = true;
						return;
					}
					break;
				case GameMessage::UPDATEEXITS:
					mapSys_->updateExits(msg.positions);
					break;
				case GameMessage::UPDATEPOS:
					heroSys_->updatePositions(msg.positions);
					break; 	
            }
        }   
    }
}

void Game::joinGame()
{
    std::cout<<"Joining game\n";

    GameMessage em = GameMessage();
    em.type = GameMessage::MessageType::CLIENTJOINED;

    sendMessage(em);
}

void Game::leaveGame()
{
	std::cout<<"Leaving game\n";

    GameMessage em = GameMessage();
    em.type = GameMessage::MessageType::CLIENTLEFT;

    sendMessage(em);
}

void Game::sendMessage(GameMessage& message)
{
	std::cout<<"Sending message\n";

    socket.send(message, socket);
}