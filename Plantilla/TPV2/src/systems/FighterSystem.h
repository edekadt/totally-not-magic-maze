#pragma once

#include <array>
#include <cstdint>

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

	FighterSystem():cazaTransform_(nullptr) {}
	~FighterSystem() {}

	// Reaccionar a los mensajes recibidos (llamando a métodos correspondientes).
	void receive(const Message& m);

	// Crear la entidad del caza, añadir sus componentes, asociarla con un handler
	// correspondiente, etc.
	void initSystem() override;

	// Si el juego está parado no hacer nada, en otro caso actualizar la velocidad
	// del caza y moverlo como en la práctica 1 (acelerar, desacelerar, etc).
	void update() override;

private:

	// Para reaccionar al mensaje de que ha habido un choque entre el fighter y un
	// un asteroide. Poner el caza en el centro con velocidad (0,0) y rotación 0. No
	// hace falta desactivar la entidad (no dibujarla si el juego está parado).
	void onCollision_FighterAsteroid();

	// Para gestionar el mensaje de que ha acabado una ronda. Desactivar el sistema.
	void onRoundOver();

	// Para gestionar el mensaje de que ha empezado una ronda. Activar el sistema.
	void onRoundStart();

	void fighterMiddle(); 

	// Indica si el sistema está activo o no (modificar el valor en onRoundOver y
	// onRoundStart, y en update no hacer nada si no está activo)
	bool active_;

	Transform* cazaTransform_;
	Texture* t; 
	Message m;

	const float speedLimit = 3.0f;
	const float thrust = 0.2f;
	Vector2D newVel;

	float timeToFire = 250.0f, previousShot;
};

