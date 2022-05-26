 #include "RenderSystem.h"

void RenderSystem::update()
{	 
	if (active_) {
		drawFighter();
	}
	drawMSG();
	drawLife();
}

void RenderSystem::drawFighter()
{
	auto caza = mngr_->getHandler(ecs::_hdlr_CAZA);

	auto cazaTr_ = mngr_->getComponent<Transform>(caza);
	assert(cazaTr_ != nullptr);
	auto t = &sdlutils().images().at("fighter");

	SDL_Rect dest = build_sdlrect(cazaTr_->pos_, cazaTr_->width_, cazaTr_->height_);
	t->render(dest, cazaTr_->rot_);
}

void RenderSystem::drawLife()
{
	auto life_ = mngr_->getHandler(ecs::_hdlr_CAZA);
	assert(life_ != nullptr);

	auto vidas = 3; 

	auto t = &sdlutils().images().at("heart");

	int cont = 0;

	for (int i = 0; i < vidas; i++)
	{
		SDL_Rect dest = build_sdlrect(cont + 10.0f, 15.0f, 30.0f, 30.0f);
		t->render(dest, 0);

		cont += 35.0f; 
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
