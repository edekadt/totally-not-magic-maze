#include "FighterSystem.h"
#include "../sdlutils/InputHandler.h"

void FighterSystem::initSystem() 
{
	auto* caza = mngr_->addEntity(ecs::_grp_FIGHTERS);
	auto cazaTransform_ = mngr_->addComponent<Transform>(caza); 
	cazaTransform_->init(Vector2D(sdlutils().width() / 2.0f, sdlutils().height() / 2.0f),
		Vector2D(), 50.0f, 50.0f, 0.0f);
	mngr_->setHandler(ecs::_hdlr_CAZA, caza);

	auto* caza2 = mngr_->addEntity(ecs::_grp_FIGHTERS);
	auto cazaTransform1_ = mngr_->addComponent<Transform>(caza2);
	cazaTransform1_->init(Vector2D(sdlutils().width() / 2.0f + 200.0f, sdlutils().height() / 2.0f),
		Vector2D(), 50.0f, 50.0f, 0.0f);
	mngr_->setHandler(ecs::_hdlr_CAZA1, caza2);

	float contador = 0.0f; 

	for (int i = 0; i < 10; i++)
	{
		auto* bloque = mngr_->addEntity(ecs::_grp_BLOCKS); 
		auto blockTransform = mngr_->addComponent<Transform>(bloque); 
		blockTransform->init(Vector2D(50.0f, contador), Vector2D(), 50.0f, 50.0f, 0.0f); 
		contador += 50.0f; 
	}
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
				move(50.0f / 2, true);
			}
				
			else if (ihldr.isKeyDown(SDL_SCANCODE_LEFT))
			{
				move(-50.0f / 2, true);
			}
				
			else if (ihldr.isKeyDown(SDL_SCANCODE_UP))
			{
				move(-50.0f / 2, false);
			}
				

			else if (ihldr.isKeyDown(SDL_SCANCODE_DOWN))
			{
				move(50.0f / 2, false);
			}			
		}
	}
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

void FighterSystem::onRoundOver()
{
	active_ = false; 
}

void FighterSystem::onRoundStart()
{
	active_ = true; 
}

void FighterSystem::move(float value, bool izqDer)
{
	if (izqDer)
	{
		for (auto e : mngr_->getEntities(ecs::_grp_FIGHTERS))
		{
			auto playerTr = mngr_->getComponent<Transform>(e); 
			playerTr->pos_.setX(playerTr->pos_.getX() + value);
		}
	}

	else
	{
		for (auto e : mngr_->getEntities(ecs::_grp_FIGHTERS))
		{
			auto playerTr = mngr_->getComponent<Transform>(e);
			playerTr->pos_.setY(playerTr->pos_.getY() + value);
		}
	}
}

void FighterSystem::dontMove(float value, bool izqDer)
{
	for (auto e : mngr_->getEntities(ecs::_grp_FIGHTERS))
	{
		auto playerTr = mngr_->getComponent<Transform>(e);

		for (auto b : mngr_->getEntities(ecs::_grp_BLOCKS))
		{
			auto blockTr = mngr_->getComponent<Transform>(b);

			if (Collisions::collides(playerTr->pos_, playerTr->width_, playerTr->height_,
				blockTr->pos_, blockTr->width_, blockTr->height_))
			{
				if (izqDer)
					playerTr->pos_.setX(playerTr->pos_.getX() - value);
				else
					playerTr->pos_.setY(playerTr->pos_.getY() - value); 
			}
		}
	}

	dontMove(value, izqDer); 
	movimientos++; 
}
