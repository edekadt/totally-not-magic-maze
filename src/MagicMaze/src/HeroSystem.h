#pragma once

#include <array>
#include <cstdint>
#include <map>

#include "System.h"
#include "Entity.h"
#include "Manager.h"

#include "Collisions.h"
#include "Vector2D.h"

#include "SDLUtils.h"
#include "Texture.h"

#include "Transform.h"

#include "GameCtrlSystem.h"
//#include "messages.h"

class HeroSystem : public ecs::System {
public:

	__SYSID_DECL__(ecs::_sys_FIGHTERSYSTEM);

	HeroSystem() {}
	~HeroSystem() {}

	// Reaccionar a los mensajes recibidos (llamando a m�todos correspondientes).
	void receive(const Message& m);

	// Crear la entidad del caza, a�adir sus componentes, asociarla con un handler
	// correspondiente, etc.
	void initSystem() override;

	// Si el juego est� parado no hacer nada, en otro caso actualizar la velocidad
	// del caza y moverlo como en la pr�ctica 1 (acelerar, desacelerar, etc).
	void update() override;

	int getMov() { return movimientos; }

	ecs::Entity* getFighterExit(ecs::Entity* fighter);

	void addFighter(int fighterID, int x, int y);
	void addFighterExits();

	void resetLevel();
	
private:
	// Asocia cada fighter con su salida
	std::map<ecs::Entity*, ecs::Entity*> exits;

	// Para gestionar el mensaje de que ha acabado una ronda. Desactivar el sistema.
	void onRoundOver();

	// Para gestionar el mensaje de que ha empezado una ronda. Activar el sistema.
	void onRoundStart();

	void move(int x, int y);

	// Comprueba si hay una pared o un caza delante en la direcci�n que se quiere mover.
	// Devuelve false si no se puede realizar el movimiento
	bool checkMove(Transform* tr, int x, int y);

	// Indica si el sistema est� activo o no (modificar el valor en onRoundOver y
	// onRoundStart, y en update no hacer nada si no est� activo)
	bool active_;

	Texture* t; 
	int numFighters = 0;

	int movimientos = 0; 
};

