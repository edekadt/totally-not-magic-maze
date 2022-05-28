#include "RenderSystem.h"
#include "CollisionsSystem.h"
#include <map>

void RenderSystem::update()
{	 
	drawBase();
	drawMap(); 
	drawLife();
	if (active_) {
		drawFighter();
	}
	drawMSG();
}

void RenderSystem::drawFighter()
{
	int cont = 0; 

	for (auto e : mngr_->getEntities(ecs::_grp_FIGHTERS))
	{
		if (mngr_->isAlive(e)) {
			auto cazaTr = mngr_->getComponent<Transform>(e);
			assert(cazaTr != nullptr);

			SDL_Rect dest = build_sdlrect(cazaTr->pos_.getX() * 50, cazaTr->pos_.getY() * 50.0, cazaTr->width_, cazaTr->height_);

			if (cont == 0)
			{
				auto t = &sdlutils().images().at("fighter");
				t->render(dest, cazaTr->rot_);
			}

			else if (cont == 1)
			{
				auto t = &sdlutils().images().at("fighter2");
				t->render(dest, cazaTr->rot_);
			}

			cont++; 
		}
	}
}

void RenderSystem::drawBase()
{
	int cont = 0; 

	for (auto e : mngr_->getEntities(ecs::_grp_EXITS))
	{
		auto baseTr = mngr_->getComponent<Transform>(e);
		assert(baseTr != nullptr);

		SDL_Rect dest = build_sdlrect(baseTr->pos_.getX() * 50.0, baseTr->pos_.getY() * 50.0, baseTr->width_, baseTr->height_);

		if (cont == 0)
		{
			auto t = &sdlutils().images().at("salidaAzul");
			t->render(dest, 0);
		}

		else if (cont == 1)
		{
			auto t = &sdlutils().images().at("salidaVerde");
			t->render(dest, 0);
		}

		cont++; 
	}
}

void RenderSystem::drawMap()
{
	//for (auto e : mngr_->getEntities(ecs::_grp_BLOCKS))
	//{
	//	auto blockTr = mngr_->getComponent<Transform>(e); 
	//	auto t = &sdlutils().images().at("fire");
	//	SDL_Rect dest = build_sdlrect(blockTr->pos_.getX() * 50.0, blockTr->pos_.getY() * 50.0, blockTr->width_, blockTr->height_);
	//	t->render(dest, 0);
	//}
	auto grid = mngr_->getSystem<CollisionsSystem>()->getGrid();
	for (int i = 0; i < (*grid).size(); ++i)
	{
		for (int j = 0; j < (*grid)[i].size(); ++j)
		{
			std::string sprite = mapSprites[((*grid)[i][j])];
			if (sprite != "")
			{
				auto t = &sdlutils().images().at(sprite);
				SDL_Rect dest = build_sdlrect(i * 50.0, j * 50.0, 50.0, 50.0);
				t->render(dest, 0);
			}
		}
	}
}


void RenderSystem::drawLife()
{
	auto life_ = mngr_->getHandler(ecs::_hdlr_CAZA);
	assert(life_ != nullptr);

	auto vidas = 2; 

	auto t = &sdlutils().images().at("heart");

	int cont = 0;

	for (int i = 0; i < vidas; i++)
	{
		SDL_Rect dest = build_sdlrect(cont + 10.0f, 15.0f, 30.0f, 30.0f);
		t->render(dest, 0);

		cont += 50.0f; 
	}
}

void RenderSystem::drawMSG()
{
	auto state = mngr_->getSystem<GameCtrlSystem>()->getState();

	// message when game is not running
	if (state != GameCtrlSystem::RUNNING) {

		// game over message
		if (state == GameCtrlSystem::GAMEOVER) {
			auto win = mngr_->getSystem<GameCtrlSystem>()->getWinner();
			if (win) {
				auto& t = sdlutils().msgs().at("gameoverW");
				t.render((sdlutils().width() - t.width()) / 2,
					(sdlutils().height() - t.height()) / 2);
			}
			else {
				auto& t = sdlutils().msgs().at("gameoverL");
				t.render((sdlutils().width() - t.width()) / 2,
					(sdlutils().height() - t.height()) / 2);
			}
		}

		// new game message
		if (state == GameCtrlSystem::NEWGAME) {
			auto& t = sdlutils().msgs().at("start");
			t.render((sdlutils().width() - t.width()) / 2,
				sdlutils().height() / 2 + t.height() * 2);
		}
		else {
			auto& t = sdlutils().msgs().at("continue");
			t.render((sdlutils().width() - t.width()) / 2,
				sdlutils().height() / 2 + t.height() * 2);
		}
	}
}

void RenderSystem::receive(const Message& msg)
{
	switch (msg.id)
	{
	case _m_ROUND_START:
		active_ = true;
		break;
	case _m_ROUND_OVER:
		active_ = false;
		break;
	default:
		break;
	}
}

void RenderSystem::initSystem()
{
	mapSprites.emplace(GameMap::Cells::Empty, "");		// Vac�o
	mapSprites.emplace(GameMap::Cells::Wall, "fire");	// Paredes
	mapSprites.emplace(GameMap::Cells::Exit, "ball");	// Salidas
}
