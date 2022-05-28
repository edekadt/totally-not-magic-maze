// This file is part of the course TPV2@UCM - Samir Genaim

#include "CollisionsSystem.h"
#include "RenderSystem.h"

CollisionsSystem::CollisionsSystem() : active_(false) {
}

CollisionsSystem::~CollisionsSystem() {
}

void CollisionsSystem::initSystem() {
	generateExits();
	generateWalls();
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

void CollisionsSystem::addExit(int exitID, int x, int y)
{
	ecs::hdlrId exitHdlr;
	switch (exitID)
	{
	case 1:
		exitHdlr = ecs::_hdlr_BASE1;
		break;
	case 2:
		exitHdlr = ecs::_hdlr_BASE2;
		break;
	default:
		throw new std::exception("Handler no reconocido");
	}

	auto* base = mngr_->addEntity(ecs::_grp_EXITS);
	baseTr = mngr_->addComponent<Transform>(base);
	baseTr->init(Vector2D(x * 50.0f, y * 50.0f), Vector2D(), 50.0f, 50.0f, 0.0f);
	mngr_->setHandler(exitHdlr, base);
}

void CollisionsSystem::generateWalls()
{
	// bordes
	addVrtWall(0, 0, 11);
	addVrtWall(15, 0, 11);
	addHrzWall(1, 14, 0);
	addHrzWall(1, 14, 11);
	
	// muros interiores
	addVrtWall(1, 2, 3);
	addVrtWall(1, 7, 10);
	addVrtWall(3, 1, 3);
	addVrtWall(3, 6, 9);
	addVrtWall(6, 8, 10);
	addVrtWall(8, 7, 8);
	addVrtWall(12, 4, 7);
	addVrtWall(13, 3, 7);

	addHrzWall(4, 10, 3);
	addHrzWall(2, 6, 5);
	addHrzWall(10, 11, 5);
	addHrzWall(7, 11, 6);
	addHrzWall(9, 10, 8);
	addHrzWall(10, 14, 9);
	addHrzWall(7, 8, 10);

	addBlock(5, 1);
	addBlock(9, 1);
	addBlock(14, 1);
	addBlock(7, 2);
	addBlock(11, 2);
	addBlock(8, 4);
	addBlock(5, 7);
	addBlock(4, 9);
	addBlock(14, 10);
}

void CollisionsSystem::generateExits()
{
	addExit(1, 4, 1);
	addExit(2, 13, 10);
}

void CollisionsSystem::addHrzWall(int startX, int endX, int y)
{
	for (int x = startX; x <= endX; ++x)
		addBlock(x, y);
}

void CollisionsSystem::addVrtWall(int x, int startY, int endY)
{
	for (int y = startY; y <= endY; ++y)
		addBlock(x, y);
}

void CollisionsSystem::addBlock(int x, int y)
{
	auto* block = mngr_->addEntity(ecs::_grp_BLOCKS);
	auto transform = mngr_->addComponent<Transform>(block);
	transform->init(Vector2D(x * 50.0f, y * 50.0), Vector2D(), 50.0f, 50.0f, 0.0f);

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
