// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once

#include <array>
#include <cstdint>
#include <fstream>

#include "System.h"
#include "Manager.h"
#include "Entity.h"
#include "messages.h"

#include "Vector2D.h"
#include "Collisions.h"

#include "Transform.h"

#include "SDLUtils.h"
#include "InputHandler.h"

#include "GameCtrlSystem.h"
#include <string>
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

	void receive(const Message&) override {}
	MapSystem();
	virtual ~MapSystem();

	void initSystem(); 

	void update() override;
	
	GameMap::Grid* getGrid() { return grid; };

private:
	GameMap::Grid* grid;

	bool active_;

	Message m; 

	Transform* baseTr; 

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
};

