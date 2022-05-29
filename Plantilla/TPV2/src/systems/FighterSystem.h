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

#include "GameCtrlSystem.h"
//#include "messages.h"

class FighterSystem : public ecs::System {
public:

	__SYSID_DECL__(ecs::_sys_FIGHTERSYSTEM);

	FighterSystem() {}
	~FighterSystem() {}

	// Reaccionar a los mensajes recibidos (llamando a métodos correspondientes).
	void receive(const Message& m);

	// Crear la entidad del caza, añadir sus componentes, asociarla con un handler
	// correspondiente, etc.
	void initSystem() override;

	// Si el juego está parado no hacer nada, en otro caso actualizar la velocidad
	// del caza y moverlo como en la práctica 1 (acelerar, desacelerar, etc).
	void update() override;

	int getMov() { return movimientos; }

	ecs::Entity* getFighterExit(ecs::Entity* fighter);

	void addFighter(int fighterID, int x, int y);
	
private:
	// Asocia cada fighter con su salida
	std::map<ecs::Entity*, ecs::Entity*> exits;
	void addFighterExits();

	// Para gestionar el mensaje de que ha acabado una ronda. Desactivar el sistema.
	void onRoundOver();

	// Para gestionar el mensaje de que ha empezado una ronda. Activar el sistema.
	void onRoundStart();

	void move(int x, int y);

	// Comprueba si hay una pared o un caza delante en la dirección que se quiere mover.
	// Devuelve false si no se puede realizar el movimiento
	bool checkMove(Transform* tr, int x, int y);

	// Indica si el sistema está activo o no (modificar el valor en onRoundOver y
	// onRoundStart, y en update no hacer nada si no está activo)
	bool active_;

	Texture* t; 
	Message m;
	int numFighters = 0;

	int movimientos; 
};

