// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once

#include <vector>

namespace ecs {
class Manager;
}

class GameCtrlSystem;
class CollisionsSystem;
class RenderSystem;
class BulletsSystem;
class FighterSystem;
class AsteroidsSystem;


class Game {
public:
	Game();
	virtual ~Game();
	void init();
	void start();
private:
	ecs::Manager *mngr_;

	GameCtrlSystem *gameCtrlSys_;
	CollisionsSystem *collisionsSys_;
	RenderSystem *renderSys_;
	BulletsSystem* bulletsSys_;
	FighterSystem* fighterSys_;
	AsteroidsSystem* asteroidsSys_;
};

