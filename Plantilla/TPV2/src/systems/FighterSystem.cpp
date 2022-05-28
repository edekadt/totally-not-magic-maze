#include "FighterSystem.h"
#include "../sdlutils/InputHandler.h"
#include "CollisionsSystem.h"

void FighterSystem::initSystem() 
{
	auto* caza = mngr_->addEntity(ecs::_grp_FIGHTERS);
	auto cazaTransform_ = mngr_->addComponent<Transform>(caza); 
	cazaTransform_->init(Vector2D(6, 4),
		Vector2D(), 50.0f, 50.0f, 0.0f);
	mngr_->setHandler(ecs::_hdlr_CAZA, caza);
	addFighterExit(caza, mngr_->getHandler(ecs::_hdlr_EXIT1));

	auto* caza2 = mngr_->addEntity(ecs::_grp_FIGHTERS);
	auto cazaTransform1_ = mngr_->addComponent<Transform>(caza2);
	cazaTransform1_->init(Vector2D(9, 7),
		Vector2D(), 50.0f, 50.0f, 0.0f);
	mngr_->setHandler(ecs::_hdlr_CAZA1, caza2);
	addFighterExit(caza2, mngr_->getHandler(ecs::_hdlr_EXIT2));
}

void FighterSystem::update() 
{
	if (active_)
	{
		//Variable para detectar pulsacion de teclado
		auto& ihldr = ih();

		//Si hay una tecla pulsada...
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
}

ecs::Entity* FighterSystem::getFighterExit(ecs::Entity* fighter)
{
	return exits[fighter];
}

void FighterSystem::receive(const Message& msg) 
{
	switch (msg.id)
	{
	case _m_ROUND_OVER: 
		onRoundOver();
		break;
	case _m_ROUND_START: 
		onRoundStart();
		break;
	case _m_BASE:
		break; 
	default:
		break;
	}
}

void FighterSystem::addFighterExit(ecs::Entity* fighter, ecs::Entity* exit)
{
	exits.emplace(fighter, exit);
}

void FighterSystem::onRoundOver()
{
	active_ = false; 
}

void FighterSystem::onRoundStart()
{
	active_ = true; 
}

void FighterSystem::move(int x, int y)
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

	movimientos++;
}

bool FighterSystem::checkMove(Transform* tr, int x, int y)
{
	// Si hay una pared delante, no movemos
	auto grid = mngr_->getSystem<CollisionsSystem>()->getGrid();
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
