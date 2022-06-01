// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once
#include <array>
#include <cstdint>

#include "System.h"

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

	bool canStillPlay(); 


	bool winner; //es true si el ganador es el jugador, false si el ganador son los asteroides
	State state_;

	const int maxMov = 40; 
};

