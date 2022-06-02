#pragma once

#include <vector>

namespace ecs {
class Manager;
}

class MapSystem;
class RenderSystem;
class HeroSystem;


class Game {
public:
	Game();
	virtual ~Game();
	void init();
	void start();
private:
	ecs::Manager *mngr_;

	MapSystem *collisionsSys_;
	RenderSystem *renderSys_;
	HeroSystem* fighterSys_;
};

