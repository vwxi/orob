#ifndef ITEM_HH
#define ITEM_HH

#include <string>
#include <vector>
#include <cstring>

#include "coord.hh"

enum game_item_type { MISC, WEAPON, ARMOR, CONS };
enum game_cons_stat_type { NONE, LIFE, STR, DEF, STH };
enum game_chest_cond { GOOD, LOCKED, BROKEN };

struct game_item
{
	std::string name;
	game_item_type type;
	game_cons_stat_type stat;

	game_coord coords;
	
	int weight;
	int durability;

	int power;

	int rarity;

	bool operator!(void)
	{
		return !(*this).name.c_str();
	}

	template <typename T>
	bool operator==(T& thing)
	{
		return std::memcmp((const void*)this, &thing, sizeof(thing));
	}

	template <typename T>
	bool operator!=(T& thing)
	{
		return !(*this == thing);
	}
};

struct game_chest
{
	game_coord coords;
	
	game_chest_cond condition;
	int difficulty;
	
	std::vector<game_item> items;
};

static game_item nothing_item = {"nothing", MISC, NONE, { -1, -1 }, 0, 0, 0, 0};

static std::vector<game_item> global_items = {
	// weapons
	{"twig", WEAPON, NONE, { -1, -1 }, 1, 10, 1, 100},
	{"club", WEAPON, NONE, { -1, -1 }, 2, 15, 2, 99},
	{"wooden spear", WEAPON, NONE, { -1, -1 }, 3, 15, 5, 99},
	{"spiked club", WEAPON, NONE, { -1, -1 }, 5, 15, 5, 99},
	{"wooden sword", WEAPON, NONE, { -1, -1 }, 6, 15, 5, 99},
	
	// food
	{"pile of dust", CONS, NONE, { -1, -1 }, 1, 1, 0, 100}
};

#endif
