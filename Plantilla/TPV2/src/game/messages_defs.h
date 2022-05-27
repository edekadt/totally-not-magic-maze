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
	_m_BASE
};


struct Message {
	msgId_type id;

	union {
	};
};
