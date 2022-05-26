// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once
#include <array>
#include <cstdint>

#include "../ecs/System.h"

class GameCtrlSystem: public ecs::System {
public:

	__SYSID_DECL__(ecs::_sys_GAMECTRLSYSTEM);


	enum State {
		NEWGAME = 0, // just before starting a new game
		PAUSED, // between rounds
		RUNNING, // playing
		GAMEOVER // game over
	};

	GameCtrlSystem();
	virtual ~GameCtrlSystem();

	inline State getState() {
		return state_;
	}

	bool getWinner() {
		return winner;
	}

	void update() override;
	void receive(const Message&) override;

	void initSystem() override {}


private:

	void startRound();
	void startGame();
	void roundOver();
	void gameOver();

	// Para gestionar el mensaje que ha habido un choque entre el fighter y un asteroide.
	// Tiene que avisar que ha acabado la ronda, quitar una vida al fighter, y si no hay m�s vidas
	// avisar que ha acabado el juego (y quien es el ganador).
	void onCollision_FighterAsteroid();


	bool winner; //es true si el ganador es el jugador, false si el ganador son los asteroides
	State state_;
};

