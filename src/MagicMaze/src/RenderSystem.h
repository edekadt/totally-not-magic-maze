#pragma once

#include <array>
#include <cstdint>

#include "System.h"
#include "Manager.h"
#include "Entity.h"

#include "Vector2D.h"

#include "Image.h"
#include "Transform.h"

#include "SDLUtils.h"
#include "InputHandler.h"

#include "GameCtrlSystem.h"
#include "MapSystem.h"


class RenderSystem : public ecs::System {
public:

	__SYSID_DECL__(ecs::_sys_RENDERSYSTEM);

	// Reaccionar a los mensajes recibidos (llamando a m�todos correspondientes).
	void receive(const Message& m) override;

	// Inicializar el sistema, etc.
	void initSystem() override;

	// - Dibujar asteroides, balas y caza (s�lo si el juego no est� parado).
	// - Dibujar las vidas (siempre).
	// - Dibujar los mensajes correspondiente: si el juego est� parado, etc (como en
	// la pr�ctica 1)
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

