#include "HeroSystem.h"
#include "MapSystem.h"

#include "../network/Socket.h"

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

void HeroSystem::updatePositions(int positions [8])
{
	for (int i = 0; i < 8; i += 2)
	{
		Transform* tr;
		switch(i)
		{
		case 0:
			tr = mngr_->getComponent<Transform>(mngr_->getHandler(ecs::_hdlr_CAZA0));
			tr->pos_ = Vector2D(positions[i], positions[i+1]);
			break;
		case 1:
			tr = mngr_->getComponent<Transform>(mngr_->getHandler(ecs::_hdlr_CAZA1));
			tr->pos_ = Vector2D(positions[i], positions[i+1]);
			break;
		case 2:
			tr = mngr_->getComponent<Transform>(mngr_->getHandler(ecs::_hdlr_CAZA2));
			tr->pos_ = Vector2D(positions[i], positions[i+1]);
			break;
		case 3:
			tr = mngr_->getComponent<Transform>(mngr_->getHandler(ecs::_hdlr_CAZA3));
			tr->pos_ = Vector2D(positions[i], positions[i+1]);
			break;
		}
	}
}

void HeroSystem::move(char dir)
{
	switch (dir)
	{
	case 'U':
	case 'L':
    {
		GameMessage em = GameMessage();
		em.type = GameMessage::MessageType::MOVEMENT;
		em.direction = dir;
		game->sendMessage(em);
	}
		break;
	case 'D':
	case 'R':
		break;
	}
}
