// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once

#include <vector>

namespace ecs {
class Manager;
}

class GameCtrlSystem;
class MapSystem;
class RenderSystem;
class BulletsSystem;
class HeroSystem;
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
	MapSystem *collisionsSys_;
	RenderSystem *renderSys_;
	BulletsSystem* bulletsSys_;
	HeroSystem* fighterSys_;
	AsteroidsSystem* asteroidsSys_;
};

