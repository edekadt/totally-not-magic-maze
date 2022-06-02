#include "Game.h"

#include "../ecs/Manager.h"
#include "../sdlutils/InputHandler.h"
#include "../sdlutils/SDLUtils.h"

#include "../systems/MapSystem.h"
#include "../systems/RenderSystem.h"
#include "../systems/HeroSystem.h"

#include "../utils/Vector2D.h"
#include "../utils/Collisions.h"

using ecs::Manager;

Game::Game() :
		mngr_(nullptr), //
	    fighterSys_(nullptr), //
		collisionsSys_(nullptr), //
		renderSys_(nullptr) {}

Game::~Game() {
	delete mngr_;
}

void Game::init() {

	// Initialize the SDLUtils singleton
	SDLUtils::init("Magic Maze", 600, 600,
			"resources/config/material.resources.json");

	sdlutils().hideCursor();

	// Create the manager
	mngr_ = new Manager();

	collisionsSys_ = mngr_->addSystem<MapSystem>();
	fighterSys_ = mngr_->addSystem<HeroSystem>();
	renderSys_ = mngr_->addSystem<RenderSystem>();
}

void Game::start() {
	mngr_->getSystem<MapSystem>()->initSystem();
	mngr_->getSystem<HeroSystem>()->initSystem();
	mngr_->getSystem<RenderSystem>()->initSystem();

	// a boolean to exit the loop
	bool exit = false;

	auto &ihdlr = ih();

	while (!exit) {
		Uint32 startTime = sdlutils().currRealTime();

		// refresh the input handler
		ihdlr.refresh();

		if (ihdlr.isKeyDown(SDL_SCANCODE_ESCAPE)) {
			exit = true;
			continue;
		}

		mngr_->refresh();

		fighterSys_->update();
		collisionsSys_->update();	

		sdlutils().clearRenderer();
		renderSys_->update();
		sdlutils().presentRenderer();

		Uint32 frameTime = sdlutils().currRealTime() - startTime;

		if (frameTime < 10)
			SDL_Delay(10 - frameTime);
	}
}
