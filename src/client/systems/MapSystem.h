#pragma once

#include <array>
#include <cstdint>
#include <fstream>
#include <string>

#include "../ecs/System.h"
#include "../ecs/Manager.h"
#include "../ecs/Entity.h"

#include "../utils/Vector2D.h"
#include "../utils/Collisions.h"

#include "../components/Transform.h"

#include "../sdlutils/SDLUtils.h"
#include "../sdlutils/InputHandler.h"

namespace GameMap
{
	enum class Cells { Empty, Wall, Exit};
	using Grid = std::vector<std::vector<Cells>>;
}

struct Transform;

class MapSystem: public ecs::System {


public:

	__SYSID_DECL__(ecs::_sys_COLLISIONSYSTEM);

	MapSystem();
	virtual ~MapSystem();

	void initSystem(); 

	void newMap(char maptext [144]);

	void update() override;
	
	GameMap::Grid* getGrid() { return grid; };

	void updateExits(int positions [8]);

private:
	GameMap::Grid* grid;

	void addExit(int exitID);
	void addBlock(int x, int y);

	void clearMap(); 
	void initializeMap();
};

