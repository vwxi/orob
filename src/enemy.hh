#ifndef ENEMY_HH
#define ENEMY_HH

#include <string>
#include <vector>

#include "coord.hh"

struct game_enemy
{
	std::string name;

	int life;
	int strength;
	int defense;

	game_coord coords;
};

static std::vector<game_enemy> global_enemies = {
	{"rat", 5, 1, 1, {-1, -1}},
	{"snake", 7, 2, 1, {-1, -1}},
	{"goblin", 10, 2, 2, {-1, -1}},
	{"orc", 12, 3, 2, {-1, -1}},
	{"ghost", 15, 5, 3, {-1, -1}},
	{"demon", 20, 5, 5, {-1, -1}}
};

#endif
