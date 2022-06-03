#pragma once

#include <array>
#include <cstdint>

#include "../ecs/System.h"
#include "../ecs/Manager.h"
#include "../ecs/Entity.h"

#include "../utils/Vector2D.h"

#include "../components/Image.h"
#include "../components/Transform.h"

#include "../sdlutils/SDLUtils.h"
#include "../sdlutils/InputHandler.h"

#include "MapSystem.h"


class RenderSystem : public ecs::System {
public:

	__SYSID_DECL__(ecs::_sys_RENDERSYSTEM);

	// Inicializar el sistema, etc.
	void initSystem() override;

	void update() override;

private:
	//Metodos para renderizar todas las entidades del juego
	void drawFighter();  
	void drawBase(); 
	void drawMap(); 

	std::map<GameMap::Cells, std::string> mapSprites;

};

