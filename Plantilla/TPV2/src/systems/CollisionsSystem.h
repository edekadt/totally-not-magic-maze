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

	const int maxPlayers = 2; 
	int players = maxPlayers;

	void addExit(int exitID, int x, int y);
	void addBlock(int x, int y);

	void load(std::string filename, int mapX, int mapY);
	void selectorLevel();
	void generateLevel(int numHeroes, int mapX = 12, int mapY = 12);
	void createPath(int id, int mapX, int mapY, std::vector<std::vector<bool>>& occupied);
	Vector2D chooseNextDirection(int x, int y, const Vector2D& lastDirection, const std::vector<std::vector<bool>>& occupied);
	int randomBetween(int low, int high);
	void checkValidPosition(int x, int y);
};

