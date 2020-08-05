#ifndef MENU_HH
#define MENU_HH

#include "player.hh"
#include "maze.hh"

#include <ncurses.h>

enum game_armor_action { WEAR, REMOVE };

struct game_menu 
{
	void spawn_menu(game_player&);
	void inventory_menu(game_player&);
	void player_menu(game_player);

	void chest_menu(game_player&, game_chest&);
	void item_menu(game_player&, game_item&);
	void armor_menu(game_player&, game_item&, game_armor_action);
} static menu __attribute__((unused));

#endif
