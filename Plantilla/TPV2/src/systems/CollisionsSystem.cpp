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
	initializeMap(14, 14);
	selectorLevel(); 
	//generateExits();
	//generateWalls();
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

void CollisionsSystem::generateWalls()
{
	// bordes
	addVrtWall(0, 0, 11);
	addVrtWall(15, 0, 11);
	addHrzWall(1, 14, 0);
	addHrzWall(1, 14, 11);
	
	// muros interiores
	addVrtWall(1, 2, 3);
	addVrtWall(1, 7, 10);
	addVrtWall(3, 1, 3);
	addVrtWall(3, 6, 9);
	addVrtWall(6, 8, 10);
	addVrtWall(8, 7, 8);
	addVrtWall(12, 4, 7);
	addVrtWall(13, 3, 7);

	addHrzWall(4, 10, 3);
	addHrzWall(2, 6, 5);
	addHrzWall(10, 11, 5);
	addHrzWall(7, 11, 6);
	addHrzWall(9, 10, 8);
	addHrzWall(10, 14, 9);
	addHrzWall(7, 8, 10);

	addBlock(5, 1);
	addBlock(9, 1);
	addBlock(14, 1);
	addBlock(7, 2);
	addBlock(11, 2);
	addBlock(8, 4);
	addBlock(5, 7);
	addBlock(4, 9);
	addBlock(14, 10);
}

void CollisionsSystem::generateExits()
{
	addExit(0, 4, 1);
	addExit(1, 13, 10);
}

void CollisionsSystem::addHrzWall(int startX, int endX, int y)
{
	for (int x = startX; x <= endX; ++x)
		addBlock(x, y);
}

void CollisionsSystem::addVrtWall(int x, int startY, int endY)
{
	for (int y = startY; y <= endY; ++y)
		addBlock(x, y);
}

void CollisionsSystem::addBlock(int x, int y)
{
	//auto* block = mngr_->addEntity(ecs::_grp_BLOCKS);
	//auto transform = mngr_->addComponent<Transform>(block);
	//transform->init(Vector2D(x, y), Vector2D(), 50.0f, 50.0f, 0.0f);

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
		std::fstream src(filename);
		std::string text;


		for (int j = 0; j < mapY; j++)
		{
			std::getline(src, text);
			int aux = 0;

			for (int i = 0; i < mapX; i++)
			{
				if (text[aux] == '-')
					(*grid)[i][j] = GameMap::Cells::Wall;
				else if (text[aux] >= '0' && text[aux] <= '3')
					mngr_->getSystem<FighterSystem>()->addFighter(text[aux] - '0', i, j);
				else if (text[aux] >= 'A' && text[aux] <= 'D')
				{
					addExit(text[aux] - 'A', i, j);
				}
				else
					(*grid)[i][j] = GameMap::Cells::Empty;
					

				aux++;
			}
		}

		std::cout << "load";
	}

	void CollisionsSystem::selectorLevel()
	{
		srand(static_cast<long unsigned int>(time(0)));
		int level = rand() % 2;
		std::string filename;
		for (int i = 0; i < 100; i++)
		{
			std::cout << (level = rand() % 2);
			filename = "resources/config/level" + std::to_string(level) + ".txt";
			std::cout << filename << std::endl;
		}
		load(filename, 14, 14);
	}

