#ifndef PLAYER_HH
#define PLAYER_HH

#include <string>
#include <vector>

#include "maze.hh"
#include "item.hh"

enum game_player_type
{
	WARRIOR,
	WIZARD,
	ROGUE
};

struct game_armor_set
{
	game_item head,
			chest,
			leg,
			feet;
};

struct game_maze;
struct game_player
{
	std::string name;
	game_player_type type;

	int life;
	int strength;
	int defense;
	int stealth;

	int weight;

	int score;

	int level;

	game_item current_weapon;

	game_armor_set armor_set;
	
	std::vector<game_item> inventory;
		
	game_player(std::string, game_player_type, game_maze&);

	void walk(game_maze&, int, int);

	void look(game_maze);
	void fight(game_enemy&);
	
	void inventory_menu();
	void chest_menu();
	
	void death_screen();
};

#endif
