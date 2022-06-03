#include "HeroSystem.h"
#include "MapSystem.h"

#include "../game/Game.h"

#include "../sdlutils/InputHandler.h"

void HeroSystem::initSystem() {}

void HeroSystem::update() 
{
		auto & ihldr = ih();

	if (ihldr.keyDownEvent())
	{
		if (ihldr.isKeyDown(SDL_SCANCODE_RIGHT))
		{
			move('R');
		}

		else if (ihldr.isKeyDown(SDL_SCANCODE_LEFT))
		{
			move('L');
		}

		else if (ihldr.isKeyDown(SDL_SCANCODE_UP))
		{
			move('U');
		}

		else if (ihldr.isKeyDown(SDL_SCANCODE_DOWN))
		{
			move('D');
		}
	}
}


void HeroSystem::move(char dir)
{
	switch (dir)
	{
	case 'U':
	case 'L':
    	GameMessage em = GameMessage();
		em.type = GameMessage::MessageType::MOVEMENT;
		em.direction = dir;
		socket.send(em, socket);
		break;

	case 'D':
	case 'R':
		break;
	}
}

