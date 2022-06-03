#include "MapSystem.h"
#include "RenderSystem.h"
#include "HeroSystem.h"

#include "../game/Game.h"

#include <random>

MapSystem::MapSystem() {
}

MapSystem::~MapSystem() {
}

void MapSystem::initSystem() {
	load(12, 12); 
	//generateLevel(4); 
}

void MapSystem::update() {
	auto& fighters = mngr_->getEntities(ecs::_grp_FIGHTERS);

	for (ecs::Entity* e : fighters) {
		auto tr = mngr_->getComponent<Transform>(e);
		auto exitPos = mngr_->getComponent<Transform>(mngr_->getSystem<HeroSystem>()->getFighterExit(e))->pos_;
		if ((int)tr->pos_.getX() == (int)exitPos.getX() && (int)tr->pos_.getY() == (int)exitPos.getY())
		{
			mngr_->setAlive(e, false); 
			aliveFighters--; 

			if (aliveFighters <= 0)
			{
				clearMap(); 
				mngr_->getSystem<HeroSystem>()->resetLevel(); 
				auto fighters = randomBetween(2, 5);
				generateLevel(fighters); 
			}
		}
	}
}

void MapSystem::addExit(int exitID, int x, int y)
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

	(*grid)[x][y] = GameMap::Cells::Exit;

	auto* base = mngr_->addEntity(ecs::_grp_EXITS);
	baseTr = mngr_->addComponent<Transform>(base);
	baseTr->init(Vector2D(x, y), Vector2D(), 50.0f, 50.0f, 0.0f);
	mngr_->setHandler(exitHdlr, base);
}

void MapSystem::addBlock(int x, int y)
{
	(*grid)[x][y] = GameMap::Cells::Wall;
}

void MapSystem::initializeMap(int mapX, int mapY)
{
	grid = new GameMap::Grid(mapY);
	for (int i = 0; i < mapX; ++i)
	{
		grid->push_back(std::vector<GameMap::Cells>());
		for (int j = 0; j < mapY; ++j)
			(*grid)[i].push_back(GameMap::Cells::Empty);
	}
}

void MapSystem::load(int mapX, int mapY)
{
	initializeMap(mapX, mapY);
	// 0 = vac�o
	// 1 = pared
	// 2-5 = salidas
	// 6-9 = personajes

	std::fstream src("resources/config/level1.txt");
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
				mngr_->getSystem<HeroSystem>()->addFighter(text[i] - '6', i, j);
				aliveFighters++; 
				break;
			}
		}
	}

	mngr_->getSystem<HeroSystem>()->addFighterExits();
}

void MapSystem::generateLevel(int numHeroes, int mapX_, int mapY_)
{
	mapX = mapX_;
	mapY = mapY_;
	initializeMap(mapX, mapY);
	srand(static_cast<long unsigned int>(time(0)));
	// 0 = vac�o
	// 1 = pared
	// 2-5 = salidas
	// 6-9 = personajes

	// Marks which cells are part of a path (cells that must not be walls)
	std::vector<std::vector<bool>> occupied = std::vector<std::vector<bool>>(mapY, std::vector<bool>(mapX, false));

	// Add outer walls
	for (int i = 0; i < mapX; ++i)
	{
		addBlock(i, 0);
		addBlock(i, mapY-1);
		occupied[i][0] = true;
		occupied[i][mapY-1] = true;
	}
	for (int j = 1; j < mapY-1; ++j)
	{
		addBlock(0, j);
		addBlock(mapX-1, j);
		occupied[0][j] = true;
		occupied[mapX-1][j] = true;
	}

	// Choose an exit cell, then wander around a bit and set the spawn
	for (int i = 0; i < numHeroes; i++)
		createPath(i, occupied);

	// Randomly fill in remaining cells with walls
	for (int j = 0; j < mapY; j++)
	{
		for (int i = 0; i < mapX; i++)
		{
			if (!occupied[i][j])
				addBlock(i, j);
		}
	}

	mngr_->getSystem<HeroSystem>()->addFighterExits();

	mapMsg();
}

void MapSystem::createPath(int id, std::vector<std::vector<bool>>& occupied)
{
	Vector2D exit;

	switch (id)
	{
	case 0:
		exit = Vector2D(randomBetween(1, mapX / 2), randomBetween(1, mapY / 2));
		break;
	case 1:
		exit = Vector2D(randomBetween(1, mapX / 2), randomBetween(mapY / 2, mapY - 1));
		break;
	case 2:
		exit = Vector2D(randomBetween(mapX / 2, mapX - 1), randomBetween(1, mapY / 2));
		break;
	case 3:
		exit = Vector2D(randomBetween(mapX / 2, mapX - 1), randomBetween(mapY / 2, mapY - 1));
		break;
	}
	addExit(id, exit.getX(), exit.getY());

	int pathLength = randomBetween(20, 40);

	Vector2D pathDirection;
	switch (rand() % 4)
	{
	case 0:
		pathDirection = Vector2D(0, 1);
		break;
	case 1:
		pathDirection = Vector2D(0, -1);
		break;
	case 2:
		pathDirection = Vector2D(1, 0);
		break;
	case 3:
		pathDirection = Vector2D(-1, 0);
		break;
	}
	int x = exit.getX(), y = exit.getY();
	for (int i = 0; i < pathLength; ++i)
	{
		occupied[x][y] = true;
		pathDirection = chooseNextDirection(x, y, pathDirection, occupied);
		x += pathDirection.getX();
		y += pathDirection.getY();
	}

	occupied[x][y] = true; 

	mngr_->getSystem<HeroSystem>()->addFighter(id, x, y);
	aliveFighters++; 
}

Vector2D MapSystem::chooseNextDirection(int x, int y, const Vector2D& lastDirection, const std::vector<std::vector<bool>>& occupied)
{
	assert(abs(lastDirection.getX() <= 1) && abs(lastDirection.getY() <= 1));
	
	int direction = rand() % 4;
	if (direction <= 1 && validPos(x + lastDirection.getX(), y + lastDirection.getY())) 
		// keep same direction
		return lastDirection;
	else if (validPos(x - lastDirection.getY(), y + lastDirection.getX()) && validPos(x + lastDirection.getY(), y - lastDirection.getX()))
	{
		// turn either direction
		return direction == 2 ? Vector2D(-lastDirection.getY(), lastDirection.getX()) :
			Vector2D(lastDirection.getY(), -lastDirection.getX());
	}
	else if (validPos(x - lastDirection.getY(), y + lastDirection.getX())) 
	{
		// turn clockwise
		return Vector2D(-lastDirection.getY(), lastDirection.getX());
	}
	else
	{
		// turn anti-clockwise
		return Vector2D(lastDirection.getY(), -lastDirection.getX());
	}
}

int MapSystem::randomBetween(int low, int high)
{
	return rand() % (high-low) + low;
}

bool MapSystem::validPos(int x, int y)
{
	return  x > 0 && x < mapX - 1 && y > 0 && y < mapY - 1;
}

void MapSystem::clearMap()
{
	for (auto fighter : mngr_->getEntities(ecs::_grp_FIGHTERS))
	{
		mngr_->setAlive(fighter, false); 
	}

	for (auto exit : mngr_->getEntities(ecs::_grp_EXITS))
	{
		mngr_->setAlive(exit, false); 
	}
}

void MapSystem::SendMessages()
{
	exitsMsg();
	mapMsg();
}

void MapSystem::mapMsg()
{
	GameMessage msg = GameMessage();
    int aux = 0;  
    for (int j = 0; j < 12; j++)
    {
        for (int i = 0; i < 12; i++)
        {
            if ((*grid)[i][j] == GameMap::Cells::Exit)
            {
                msg.map[aux] = 'X'; 
            }
            else if ((*grid)[i][j] == GameMap::Cells::Wall)
            {
                msg.map[aux] = 'W';
            }
            else
            {
                msg.map[aux] = '-';
            }
            aux++; 
        }
    }
	msg.type = GameMessage::NEWMAP;
	game->sendMessage(msg);
}


void MapSystem::exitsMsg()
{
	
	GameMessage msg = GameMessage();
	for (int i = 0; i< 8; i+=2)
	{
		ecs::Entity* hdlr = NULL;
		switch(i)
		{
		case 0:
			hdlr = mngr_->getHandler(ecs::_hdlr_EXIT0);
			break;
		case 1:
			hdlr = mngr_->getHandler(ecs::_hdlr_EXIT1);
			break;
		case 2:
			hdlr = mngr_->getHandler(ecs::_hdlr_EXIT2);
			break;
		case 3:
			hdlr = mngr_->getHandler(ecs::_hdlr_EXIT3);
			break;
		}
		if (hdlr != NULL)
		{
			Transform* tr = mngr_->getComponent<Transform>(hdlr);
			if (tr->pos_.getX() != -1 && tr->pos_.getY() != -1)
			{
				msg.positions[i] = tr->pos_.getX();
				msg.positions[i+1] = tr->pos_.getY();
			}
			else
			{
				msg.positions[i] = -1;
				msg.positions[i+1] = -1;
			}
		}
		else
		{
			msg.positions[i] = -1;
			msg.positions[i+1] = -1;
		}
	}
	msg.type = GameMessage::UPDATEEXITS;
	game->sendMessage(msg);
}