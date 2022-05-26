// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once

#include <cstdint>
#include "../ecs/ecs.h"

#include "../utils/Vector2D.h"

using msgId_type = uint8_t;
enum msgId : msgId_type {
	_m_GAME_START, //
	_m_GAME_OVER, //
	_m_NEW_GAME, //
	_m_ROUND_OVER, //
	_m_ROUND_START, //
	_m_SHOOT, //
	_m_CRASH_CAZA, //
	_m_NO_ASTEROIDS, // Este es el mensaje de que el jugador ha destruido todos los asteroides que lo recibira GameCtrlSystem y ese sistema es el que acabara la partida
	_m_CRASH_ASTEROIDS // Este es el mensaje de que una bala ha destruido un asteroide
};


struct Message {
	msgId_type id;

	union {
		struct {
			Vector2D* pos;
			Vector2D* speed;
			double width;
			double height;
			float rot;
		} bullet;

		// _m_CRASH_ASTEROIDS
		struct {
			ecs::Entity *e; //Esta es la entidad del asteroide que ha chocado con la bala
			ecs::Entity* bull; //Esta es la entidad de la bala que se envia para que se destruya
		} crash_asteroids;
	};
};
