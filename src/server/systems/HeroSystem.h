#pragma once

#include <array>
#include <cstdint>
#include <map>

#include "../ecs/System.h"
#include "../ecs/Entity.h"
#include "../ecs/Manager.h"


#include "../utils/Collisions.h"
#include "../utils/Vector2D.h"

#include "../sdlutils/SDLUtils.h"
#include "../sdlutils/Texture.h"

#include "../components/Transform.h"

class Game;

class HeroSystem : public ecs::System {
public:

	__SYSID_DECL__(ecs::_sys_FIGHTERSYSTEM);

	HeroSystem() {}
	~HeroSystem() {}

	void initSystem() override;

	void update() override;

	int getMov() { return movimientos; }

	ecs::Entity* getFighterExit(ecs::Entity* fighter);

	void addFighter(int fighterID, int x, int y);
	void addFighterExits();

	void resetLevel();

	void move(int x, int y);
	
    Game* game; 
private:

	// Asocia cada fighter con su salida
	std::map<ecs::Entity*, ecs::Entity*> exits;

	bool checkMove(Transform* tr, int x, int y);

	void sendPositionUpdate();

	Texture* t; 
	int numFighters = 0;
	int movimientos = 0; 
};

