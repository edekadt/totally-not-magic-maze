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
			move(1, 0);
		}

		else if (ihldr.isKeyDown(SDL_SCANCODE_LEFT))
		{
			move(-1, 0);
		}

		else if (ihldr.isKeyDown(SDL_SCANCODE_UP))
		{
			move(0, -1);
		}

		else if (ihldr.isKeyDown(SDL_SCANCODE_DOWN))
		{
			move(0, 1);
		}
	}
}

ecs::Entity* HeroSystem::getFighterExit(ecs::Entity* fighter)
{
	return exits[fighter];
}

void HeroSystem::addFighter(int fighterID, int x, int y)
{
	++numFighters;
	auto* fighter = mngr_->addEntity(ecs::_grp_FIGHTERS);
	auto fighterTransform = mngr_->addComponent<Transform>(fighter);
	fighterTransform->init(Vector2D(x, y), Vector2D(), 50.0f, 50.0f, 0.0f);
	switch (fighterID)
	{
	case 0:
		mngr_->setHandler(ecs::_hdlr_CAZA0, fighter);
		break;
	case 1:
		mngr_->setHandler(ecs::_hdlr_CAZA1, fighter);
		break;
	case 2:
		mngr_->setHandler(ecs::_hdlr_CAZA2, fighter);
		break;
	case 3:
		mngr_->setHandler(ecs::_hdlr_CAZA3, fighter);
		break;
	}
}

void HeroSystem::addFighterExits()
{
	switch (numFighters)
	{
	case 4:
		exits.emplace(mngr_->getHandler(ecs::_hdlr_CAZA3), mngr_->getHandler(ecs::_hdlr_EXIT3));
	case 3:
		exits.emplace(mngr_->getHandler(ecs::_hdlr_CAZA2), mngr_->getHandler(ecs::_hdlr_EXIT2));
	case 2:
		exits.emplace(mngr_->getHandler(ecs::_hdlr_CAZA1), mngr_->getHandler(ecs::_hdlr_EXIT1));
	case 1:
		exits.emplace(mngr_->getHandler(ecs::_hdlr_CAZA0), mngr_->getHandler(ecs::_hdlr_EXIT0));
		break;
	}
}

void HeroSystem::resetLevel()
{
	numFighters = 0; 
	exits.clear(); 
}

void HeroSystem::move(int x, int y)
{
	for (auto e : mngr_->getEntities(ecs::_grp_FIGHTERS))
	{
		auto playerTr = mngr_->getComponent<Transform>(e); 
		if (checkMove(playerTr, x, y))
		{
			playerTr->pos_.setX(playerTr->pos_.getX() + x);
			playerTr->pos_.setY(playerTr->pos_.getY() + y);
		}
	}

	game->updPos(); 
	movimientos++;
}

bool HeroSystem::checkMove(Transform* tr, int x, int y)
{
	// Si hay una pared delante, no movemos
	auto grid = mngr_->getSystem<MapSystem>()->getGrid();
	GameMap::Cells cell = (*grid)[(int)(tr->pos_.getX() + x)][(int)(tr->pos_.getY() + y)];
	if (cell == GameMap::Cells::Wall)
		return false;

	// Comprobamos que ninguno de los otros personajes está delante
	for (auto f : mngr_->getEntities(ecs::_grp_FIGHTERS))
	{
		auto otherTr = mngr_->getComponent<Transform>(f);

		if (otherTr != tr && otherTr->pos_.getX() == tr->pos_.getX() + x && otherTr->pos_.getY() == tr->pos_.getY() + y)
			return false;
	}
	return true;
}
