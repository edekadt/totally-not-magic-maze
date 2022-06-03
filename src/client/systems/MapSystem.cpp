#include "MapSystem.h"
#include "RenderSystem.h"
#include "HeroSystem.h"

#include <random>

MapSystem::MapSystem() {
}

MapSystem::~MapSystem() {
}

void MapSystem::initSystem() {
	initializeMap();
	for (int i = 0; i < 4; ++i)
		addExit(i);
}

void MapSystem::update() { }

void MapSystem::addExit(int exitID)
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
		//throw new std::exception("Handler no reconocido");
		break; 
	}
	auto* base = mngr_->addEntity(ecs::_grp_EXITS);
	auto baseTr = mngr_->addComponent<Transform>(base);
	baseTr->init(Vector2D(-1, -1), Vector2D(), 50.0f, 50.0f, 0.0f);
	mngr_->setHandler(exitHdlr, base);
}

void MapSystem::addBlock(int x, int y)
{
	(*grid)[x][y] = GameMap::Cells::Wall;
}

void MapSystem::initializeMap()
{
	int mapX = 12, mapY = 12;
	if (grid != NULL)
		delete grid;
	grid = new GameMap::Grid(mapY);
	for (int i = 0; i < mapX; ++i)
	{
		grid->push_back(std::vector<GameMap::Cells>());
		for (int j = 0; j < mapY; ++j)
			(*grid)[i].push_back(GameMap::Cells::Empty);
	}
}

void MapSystem::newMap(char maptext [144])
{
	initializeMap();
	for (int j = 0; j < 12; j++)
	{
		for (int i = 0; i < 12; i++)
		{
			switch (maptext[12*j + i])
			{
				case 'W':
					addBlock(i, j);
					break;
				case 'X':
					(*grid)[i][j] = GameMap::Cells::Exit;
					break;
				case '-':
					break;
			}
		}
	}
}

void MapSystem::updateExits(int positions [8])
{
	for (int i = 0; i < 8; i += 2)
	{
		Transform* tr;
		switch(i)
		{
		case 0:
			tr = mngr_->getComponent<Transform>(mngr_->getHandler(ecs::_hdlr_EXIT0));
			tr->pos_ = Vector2D(positions[i], positions[i+1]);
			break;
		case 1:
			tr = mngr_->getComponent<Transform>(mngr_->getHandler(ecs::_hdlr_EXIT1));
			tr->pos_ = Vector2D(positions[i], positions[i+1]);
			break;
		case 2:
			tr = mngr_->getComponent<Transform>(mngr_->getHandler(ecs::_hdlr_EXIT2));
			tr->pos_ = Vector2D(positions[i], positions[i+1]);
			break;
		case 3:
			tr = mngr_->getComponent<Transform>(mngr_->getHandler(ecs::_hdlr_EXIT3));
			tr->pos_ = Vector2D(positions[i], positions[i+1]);
			break;
		}
	}
}