// This file is part of the course TPV2@UCM - Samir Genaim

#include "GameCtrlSystem.h"
#include "HeroSystem.h"

#include "Manager.h"

#include "InputHandler.h"
#include "SDLUtils.h"

GameCtrlSystem::GameCtrlSystem() :
		state_(NEWGAME), 
		winner(false) {
}

GameCtrlSystem::~GameCtrlSystem() {
}


void GameCtrlSystem::update() {
	if (state_ != RUNNING) {

		auto &inhdlr = ih();

		if (inhdlr.isKeyDown(SDL_SCANCODE_SPACE)) {
			switch (state_) {
			case NEWGAME:
			case PAUSED:
				startRound();
				break;
			case GAMEOVER:
				startGame();
				break;
			default:
				break;
			}
		}
	}
}

void GameCtrlSystem::receive(const Message &m) {
	switch (m.id) {
	default:
		break;
	}
}

void GameCtrlSystem::startRound() {
	Message m;
	state_ = RUNNING;
	m.id = _m_ROUND_START;
	mngr_->send(m);
}

void GameCtrlSystem::startGame() {
	Message m;
	state_ = NEWGAME;
	m.id = _m_NEW_GAME;
	mngr_->send(m);
}

void GameCtrlSystem::roundOver() {
	Message m;
	state_ = PAUSED;
	m.id = _m_ROUND_OVER;
	mngr_->send(m);
}

void GameCtrlSystem::gameOver() {
	Message m;
	state_ = GAMEOVER;
	m.id = _m_ROUND_OVER;
	mngr_->send(m);
	m.id = _m_GAME_OVER;
	mngr_->send(m);
}

bool GameCtrlSystem::canStillPlay()
{
	return mngr_->getSystem<HeroSystem>()->getMov() < maxMov;
}

