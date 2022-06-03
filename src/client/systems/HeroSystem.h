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

class HeroSystem : public ecs::System {
public:

	__SYSID_DECL__(ecs::_sys_FIGHTERSYSTEM);

	HeroSystem() {}
	~HeroSystem() {}

	void initSystem() override;

	void update() override;

	void move(char dir);
	
private:

	int numFighters = 0;
};

