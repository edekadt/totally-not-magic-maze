// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once

#include <array>
#include <cstdint>
#include <fstream>

#include "../ecs/System.h"
#include "../ecs/Manager.h"
#include "../ecs/Entity.h"
#include "../ecs/messages.h"

#include "../utils/Vector2D.h"
#include "../utils/Collisions.h"

#include "../components/Transform.h"

#include "../sdlutils/SDLUtils.h"
#include "../sdlutils/InputHandler.h"

#include "GameCtrlSystem.h"
#include <string>
namespace GameMap
{
	enum class Cells { Empty, Wall, Exit};
	using Grid = std::vector<std::vector<Cells>>;
}

struct Transform;

class CollisionsSystem: public ecs::System {

	void initializeMap(int mapX, int mapY);

public:

	__SYSID_DECL__(ecs::_sys_COLLISIONSYSTEM);

	void receive(const Message&) override;
	CollisionsSystem();
	virtual ~CollisionsSystem();

	void initSystem(); 

	void update() override;
	
	GameMap::Grid* getGrid() { return grid; };

private:
	GameMap::Grid* grid;

	bool active_;

	Message m; 

	Transform* baseTr; 

	int players = 0;
	int exits = 0; 

	void addExit(int exitID, int x, int y);
	void addBlock(int x, int y);

	void load(std::string filename, int mapX, int mapY);
	void selectorLevel();

	void cleanMap(); 
};

