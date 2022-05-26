#include "FighterSystem.h"
#include "../sdlutils/InputHandler.h"

void FighterSystem::initSystem() 
{
	auto* caza = mngr_->addEntity();

	cazaTransform_ = mngr_->addComponent<Transform>(caza); 
	cazaTransform_->init(Vector2D(sdlutils().width() / 2.0f, sdlutils().height() / 2.0f),
		Vector2D(), 50.0f, 50.0f, 0.0f);

	mngr_->setHandler(ecs::_hdlr_CAZA, caza);
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
			//Si pulsamos a la derecha o izquierda, rotamos 
			//la nave pero no la desplazamos
			if (ihldr.isKeyDown(SDL_SCANCODE_RIGHT))
			{

			}

			else if (ihldr.isKeyDown(SDL_SCANCODE_LEFT))
			{

			}

			//Si pulsamos la flecha hacia arriba, avanzamos y 
			//aumentamos la velocidad de movimiento
			else if (ihldr.isKeyDown(SDL_SCANCODE_UP))
			{
				newVel = cazaTransform_->vel_ + Vector2D(0, -1).rotate(cazaTransform_->rot_) * thrust;

				if (newVel.magnitude() > speedLimit)
					cazaTransform_->vel_ = newVel.normalize() * speedLimit;
				else
					cazaTransform_->vel_ = cazaTransform_->vel_ + 
					Vector2D(0, -1).rotate(cazaTransform_->rot_) * thrust;
			}

		}

		cazaTransform_->move();
	}
}

void FighterSystem::onCollision_FighterAsteroid()
{
	fighterMiddle(); 
}

void FighterSystem::fighterMiddle()
{
	cazaTransform_->vel_ = Vector2D(0, 0);
	cazaTransform_->rot_ = 0.0f;
	cazaTransform_->pos_ = Vector2D(sdlutils().width() / 2, sdlutils().height() / 2);
}

void FighterSystem::receive(const Message& msg) 
{
	switch (msg.id)
	{
	case _m_CRASH_CAZA:
		onCollision_FighterAsteroid();
		break;
	case _m_ROUND_OVER: 
		onRoundOver();
		break;
	case _m_ROUND_START: 
		onRoundStart();
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
