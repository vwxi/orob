#ifndef ITEM_HH
#define ITEM_HH

#include <string>
#include <vector>
#include <cstring>

#include "coord.hh"

enum game_item_type { MISC, WEAPON, ARMOR, CONS };
enum game_cons_stat_type { CNONE, LIFE, STR, DEF, STH };
enum game_armor_type { ANONE, HEAD, CHEST, LEG, FEET };

struct game_item
{
	std::string name;
	game_item_type type;
	game_cons_stat_type stat;
	game_armor_type armor_type;
	
	game_coord coords;
	
	int weight;
	int durability;

	int power;

	int rarity;

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
} static nothing_item = {std::string("nothing"), 
								MISC, 
								CNONE, 
								ANONE, 
								{ -1, -1 }, 
								0, 
								0, 
								0, 
								0};

struct game_chest
{
	game_coord coords;

	int difficulty;
	
	std::vector<game_item> items;
};

static std::vector<game_item> global_items = {
	// weapons
	{"twig", WEAPON, CNONE, ANONE, { -1, -1 }, 1, 10, 1, 100},
	{"club", WEAPON, CNONE, ANONE, { -1, -1 }, 2, 15, 2, 70},
	{"wooden spear", WEAPON, CNONE, ANONE, { -1, -1 }, 3, 15, 5, 60},
	{"spiked club", WEAPON, CNONE, ANONE, { -1, -1 }, 5, 15, 5, 50},
	{"wooden sword", WEAPON, CNONE, ANONE, { -1, -1 }, 6, 15, 5, 50},
	
	// food
	{"pile of dust", CONS, CNONE, ANONE, { -1, -1 }, 1, 1, 0, 100},
	{"pit of apple", CONS, LIFE, ANONE, { -1, -1 }, 1, 1, 1, 100},
	{"seed of apple", CONS, LIFE, ANONE, { -1, -1 }, 1, 1, 1, 100},
	{"peel of potato", CONS, LIFE, ANONE, { -1, -1 }, 1, 1, 1, 97},
	{"crumbs of bread loaf", CONS, LIFE, ANONE, { -1, -1 }, 1, 1, 1, 97},
	{"crust of bread loaf", CONS, LIFE, ANONE, { -1, -1 }, 1, 3, 50},

	// potions
	{"strength potion", CONS, STR, ANONE, { -1, -1 }, 1, 1, 5, 30},
	{"defense potion", CONS, DEF, ANONE, { -1, -1 }, 1, 1, 5, 30},
	{"stealth potion", CONS, STH, ANONE, { -1, -1 }, 1, 1, 5, 30},
	
	// armor
	{"wooden chestplate", ARMOR, CNONE, CHEST, { -1, -1 }, 3, 100, 3, 40},
	{"wooden helmet", ARMOR, CNONE, HEAD, { -1, -1 }, 3, 100, 3, 40},
	{"leather leggings", ARMOR, CNONE, LEG, { -1, -1 }, 3, 100, 3, 40},
	{"iron helmet", ARMOR, CNONE, HEAD, { -1, -1 }, 4, 100, 4, 35},
	{"leather boots", ARMOR, CNONE, FEET, { -1, -1 }, 3, 100, 3, 40}
};

#endif
