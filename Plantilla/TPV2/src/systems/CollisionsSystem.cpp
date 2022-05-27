// This file is part of the course TPV2@UCM - Samir Genaim

#include "CollisionsSystem.h"
#include "RenderSystem.h"

CollisionsSystem::CollisionsSystem() : active_(false) {
}

CollisionsSystem::~CollisionsSystem() {
}

void CollisionsSystem::initSystem() {
	auto* base = mngr_->addEntity();
	baseTr = mngr_->addComponent<Transform>(base);
	baseTr->init(Vector2D(300.0f, 50.0f), Vector2D(), 50.0f, 50.0f, 0.0f);
	mngr_->setHandler(ecs::_hdlr_BASE, base);
}

void CollisionsSystem::update() {
	auto& fighters = mngr_->getEntities(ecs::_grp_FIGHTERS);

	for (ecs::Entity* e : fighters) {
		auto eTR = mngr_->getComponent<Transform>(e);

		if (Collisions::collides(eTR->pos_, eTR->width_, eTR->height_, 
			baseTr->pos_, baseTr->width_, baseTr->height_)) {

			mngr_->setAlive(e, false); 
			players--; 

			if (players <= 0)
			{
				Message m; 
				m.id = _m_GAME_OVER; 
				mngr_->send(m); 
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
