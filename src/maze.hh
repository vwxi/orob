#ifndef MAZE_HH
#define MAZE_HH

#include <vector>

#include "item.hh"
#include "enemy.hh"
#include "coord.hh"
#include "player.hh"

struct game_player;

// maze is already generated and it is not generated procedurally
struct game_maze
{
	int length;
	int width;

	game_coord player,
				down,
				up;
	
	std::vector<game_coord> walls;
	std::vector<game_item> items;
	std::vector<game_chest> chests;
	std::vector<game_enemy> enemies;

	game_maze();

	char describe(int, int);
	void cycle(game_player&);

private:
	game_coord try_coords();
	game_coord try_coords(game_enemy&);
};

static int current __attribute__((unused));
static std::vector<game_maze> floors;

#endif
