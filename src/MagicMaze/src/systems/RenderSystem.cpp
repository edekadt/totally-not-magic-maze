#include "RenderSystem.h"
#include "MapSystem.h"
#include <map>

void RenderSystem::update()
{	 
	drawMap(); 
	drawBase();
	if (active_) {
		drawFighter();
	}
	drawMSG();
}

void RenderSystem::drawFighter()
{

	for (auto e : mngr_->getEntities(ecs::_grp_FIGHTERS))
	{
		if (mngr_->isAlive(e)) {
			auto cazaTr = mngr_->getComponent<Transform>(e);
			assert(cazaTr != nullptr);

			SDL_Rect dest = build_sdlrect(cazaTr->pos_.getX() * 50, cazaTr->pos_.getY() * 50.0, cazaTr->width_, cazaTr->height_);

			if (e == mngr_->getHandler(ecs::_hdlr_CAZA0))
			{
				auto t = &sdlutils().images().at("fighterB");
				t->render(dest, cazaTr->rot_);
			}
			else if (e == mngr_->getHandler(ecs::_hdlr_CAZA1))
			{
				auto t = &sdlutils().images().at("fighterR");
				t->render(dest, cazaTr->rot_);
			}
			else if (e == mngr_->getHandler(ecs::_hdlr_CAZA2))
			{
				auto t = &sdlutils().images().at("fighterG");
				t->render(dest, cazaTr->rot_);
			}
			else if (e == mngr_->getHandler(ecs::_hdlr_CAZA3))
			{
				auto t = &sdlutils().images().at("fighterY");
				t->render(dest, cazaTr->rot_);
			}
		}
	}
}

void RenderSystem::drawBase()
{
	for (auto e : mngr_->getEntities(ecs::_grp_EXITS))
	{
		auto baseTr = mngr_->getComponent<Transform>(e);
		assert(baseTr != nullptr);

		SDL_Rect dest = build_sdlrect(baseTr->pos_.getX() * 50.0, baseTr->pos_.getY() * 50.0, baseTr->width_, baseTr->height_);

		if (e == mngr_->getHandler(ecs::_hdlr_EXIT0))
		{
			auto t = &sdlutils().images().at("exitB");
			t->render(dest, 0);
		}
		else if (e == mngr_->getHandler(ecs::_hdlr_EXIT1))
		{
			auto t = &sdlutils().images().at("exitR");
			t->render(dest, 0);
		}
		else if (e == mngr_->getHandler(ecs::_hdlr_EXIT2))
		{
			auto t = &sdlutils().images().at("exitG");
			t->render(dest, 0);
		}
		else if (e == mngr_->getHandler(ecs::_hdlr_EXIT3))
		{
			auto t = &sdlutils().images().at("exitY");
			t->render(dest, 0);
		}
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
	auto grid = mngr_->getSystem<MapSystem>()->getGrid();
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
	mapSprites.emplace(GameMap::Cells::Empty, "floor");	// Vac�o
	mapSprites.emplace(GameMap::Cells::Wall, "wall");	// Paredes
	mapSprites.emplace(GameMap::Cells::Exit, "");	// Salidas
}
