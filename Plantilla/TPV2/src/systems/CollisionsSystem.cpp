// This file is part of the course TPV2@UCM - Samir Genaim

#include "CollisionsSystem.h"

CollisionsSystem::CollisionsSystem() : active_(false) {
}

CollisionsSystem::~CollisionsSystem() {
}

void CollisionsSystem::initSystem() {}

void CollisionsSystem::update() {

	if (active_)
	{
		auto fighter = mngr_->getHandler(ecs::_hdlr_CAZA); 
		auto cazaTr = mngr_->getComponent<Transform>(fighter); 

		for (auto e : mngr_->getEntities(ecs::_grp_ASTEROIDS))
		{
			auto asteroidTransform = mngr_->getComponent<Transform>(e);

			if (Collisions::collides(asteroidTransform->pos_, asteroidTransform->width_,
				asteroidTransform->height_, cazaTr->pos_, cazaTr->width_,
				cazaTr->height_)) 
			{
				m.id = _m_CRASH_CAZA; 
				m.crash_asteroids.e = e;
				mngr_->send(m); 
			}

			else
			{
				for (auto i : mngr_->getEntities(ecs::_grp_BULLETS))
				{
					auto bulletTr = mngr_->getComponent<Transform>(i);

					if (Collisions::collides(asteroidTransform->pos_, asteroidTransform->width_,
						asteroidTransform->height_, bulletTr->pos_, bulletTr->width_,
						bulletTr->height_))
					{
						m.id = _m_CRASH_ASTEROIDS;
						m.crash_asteroids.e = e;
						m.crash_asteroids.bull = i;
						mngr_->send(m);
					}

				}
			}
		}
	}
}

void CollisionsSystem::receive(const Message &m) {
	switch (m.id) {
	case _m_ROUND_START:
		active_ = true;
		break;
	case _m_ROUND_OVER:
		active_ = false;
		break;
	default:
		break;
	}
}
