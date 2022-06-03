#pragma once

#include <array>
#include <cstdint>
#include <fstream>
#include <string>

#include "../ecs/System.h"
#include "../ecs/Manager.h"
#include "../ecs/Entity.h"

#include "../game/Game.h"

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

	void initializeMap(int mapX, int mapY);

public:

	__SYSID_DECL__(ecs::_sys_COLLISIONSYSTEM);

	MapSystem();
	virtual ~MapSystem();

	void initSystem(); 

	void update() override;
	
	GameMap::Grid* getGrid() { return grid; };

private:
	GameMap::Grid* grid;

	Transform* baseTr; 
	Game* game; 

	int aliveFighters = 0;
	int exits = 0; 

	void addExit(int exitID, int x, int y);
	void addBlock(int x, int y);

	void load(int mapX, int mapY);
	void generateLevel(int numHeroes, int mapX_ = 12, int mapY_ = 12);
	void createPath(int id, std::vector<std::vector<bool>>& occupied);
	Vector2D chooseNextDirection(int x, int y, const Vector2D& lastDirection, const std::vector<std::vector<bool>>& occupied);
	int randomBetween(int low, int high);
	bool validPos(int x, int y);
	int mapX, mapY;

	void clearMap(); 
	
	char[144] mapMsg(); 
};

