// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once

#include <array>
#include <cstdint>

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

struct Transform;

class CollisionsSystem: public ecs::System {

public:

	__SYSID_DECL__(ecs::_sys_COLLISIONSYSTEM);

	void receive(const Message&) override;
	CollisionsSystem();
	virtual ~CollisionsSystem();
	void initSystem() override;
	void update() override;

private:

	bool active_;

	Message m; 
};

