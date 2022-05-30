// This file is part of the course TPV2@UCM - Samir Genaim

#include "CollisionsSystem.h"
#include "RenderSystem.h"
#include "FighterSystem.h"
#include <random>

CollisionsSystem::CollisionsSystem() : active_(false) {
}

CollisionsSystem::~CollisionsSystem() {
}

void CollisionsSystem::initSystem() {
	selectorLevel(); 
}

void CollisionsSystem::update() {
	auto& fighters = mngr_->getEntities(ecs::_grp_FIGHTERS);

	for (ecs::Entity* e : fighters) {
		auto tr = mngr_->getComponent<Transform>(e);
		auto exitPos = mngr_->getComponent<Transform>(mngr_->getSystem<FighterSystem>()->getFighterExit(e))->pos_;
		if ((int)tr->pos_.getX() == (int)exitPos.getX() && (int)tr->pos_.getY() == (int)exitPos.getY())
		{
			std::cout << "Exit";
			mngr_->setAlive(e, false); 
			players--; 

			if (players <= 0)
			{
				Message m; 
				m.id = _m_GAME_OVER; 
				mngr_->send(m); 
			}
		}
	}
}

void CollisionsSystem::addExit(int exitID, int x, int y)
{
	ecs::hdlrId exitHdlr;
	switch (exitID)
	{
	case 0:
		exitHdlr = ecs::_hdlr_EXIT0;
		break;
	case 1:
		exitHdlr = ecs::_hdlr_EXIT1;
		break;
	case 2:
		exitHdlr = ecs::_hdlr_EXIT2;
		break;
	case 3:
		exitHdlr = ecs::_hdlr_EXIT3;
		break;
	default:
		throw new std::exception("Handler no reconocido");
	}

	(*grid)[x][y] = GameMap::Cells::Exit;

	auto* base = mngr_->addEntity(ecs::_grp_EXITS);
	baseTr = mngr_->addComponent<Transform>(base);
	baseTr->init(Vector2D(x, y), Vector2D(), 50.0f, 50.0f, 0.0f);
	mngr_->setHandler(exitHdlr, base);
}

void CollisionsSystem::addBlock(int x, int y)
{
	(*grid)[x][y] = GameMap::Cells::Wall;
}

void CollisionsSystem::initializeMap(int mapX, int mapY)
{
	grid = new GameMap::Grid(mapY);
	for (int i = 0; i < mapX; ++i)
	{
		grid->push_back(std::vector<GameMap::Cells>());
		for (int j = 0; j < mapY; ++j)
			(*grid)[i].push_back(GameMap::Cells::Empty);
	}
}

void CollisionsSystem::receive(const Message& m) {
	switch (m.id) {
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

void CollisionsSystem::load(std::string filename, int mapX, int mapY)
{
	initializeMap(mapX, mapY);
	// 0 = vacío
	// 1 = pared
	// 2-5 = salidas
	// 6-9 = personajes

	std::fstream src(filename);
	std::string text;

	for (int j = 0; j < mapY; j++)
	{
		std::getline(src, text);
		for (int i = 0; i < mapX; i++)
		{
			switch (text[i])
			{
			case '0':
				(*grid)[i][j] = GameMap::Cells::Empty;
				break;
			case'1':
				addBlock(i, j);
				break;
			case'2':
			case'3':
			case'4':
			case'5':
				addExit(text[i] - '2', i, j);
				break;
			case'6':
			case'7':
			case'8':
			case'9':
				mngr_->getSystem<FighterSystem>()->addFighter(text[i] - '6', i, j);
				break;
			}
		}
	}
}

void CollisionsSystem::selectorLevel()
{
	srand(static_cast<long unsigned int>(time(0)));
	int level = rand() % 2;
	std::string filename = "resources/config/level" + std::to_string(1) + ".txt";
	load(filename, 12, 12);
}

