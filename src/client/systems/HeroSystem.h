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

#include "../network/Socket.h"

class Socket;

class HeroSystem : public ecs::System {
public:

	__SYSID_DECL__(ecs::_sys_FIGHTERSYSTEM);

	HeroSystem() {}
	~HeroSystem() {}

	void initSystem() override;

	void update() override;

	void updatePositions(int positions [8]);

	void move(char dir);

	void setSocket(Socket* soc);
	
private:

	Socket* socket;
	int numFighters = 0;
};

