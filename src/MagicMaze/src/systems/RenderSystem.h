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

#include "GameCtrlSystem.h"
#include "MapSystem.h"


class RenderSystem : public ecs::System {
public:

	__SYSID_DECL__(ecs::_sys_RENDERSYSTEM);

	// Reaccionar a los mensajes recibidos (llamando a métodos correspondientes).
	void receive(const Message& m) override;

	// Inicializar el sistema, etc.
	void initSystem() override;

	// - Dibujar asteroides, balas y caza (sólo si el juego no está parado).
	// - Dibujar las vidas (siempre).
	// - Dibujar los mensajes correspondiente: si el juego está parado, etc (como en
	// la práctica 1)
	void update() override;

private:
	//Metodos para renderizar todas las entidades del juego
	void drawFighter(); 
	void drawMSG();  
	void drawBase(); 
	void drawMap(); 

	//Este booleano es para indicar si se dibuja el caza, los asteroides y las balas cuando esta activo
	bool active_; 

	std::map<GameMap::Cells, std::string> mapSprites;

};

