#include <string>
#include <ncurses.h>

#include "maze.hh"
#include "player.hh"
#include "util.hh"
#include "log.hh"
#include "menu.hh"
#include "score.hh"

game_player::game_player(std::string n, game_player_type t, game_maze& m)
{
	name = n;
	type = t;

	score = 0;
	level = 1;
	
	m.player = { .x = 1, .y = 1 };
		
	life = 1000;
	weight = 100;

	switch(type) {
	default:
	case WARRIOR:
		strength = 3;
		defense = 2;
		stealth = 1;

		inventory.push_back({"dull sword", WEAPON, NONE,
							{ -1, -1 }, 1, 100, 2, -1});

		break;
		
	case WIZARD:
		strength = 2;
		defense = 2;
		stealth = 1;

		inventory.push_back({"dull wand", WEAPON, NONE,
							{ -1, -1 }, 1, 100, 2, -1});
							
		break;
		
	case ROGUE:
		strength = 2;
		defense = 1;
		stealth = 3;

		inventory.push_back({"dull shank", WEAPON, NONE,
							{ -1, -1 }, 1, 100, 2, -1});
		break;
	}

	armor_set = {
		.head = {"head cloth", ARMOR, NONE, { -1, -1 }, 0, 1000, 0, -1},
		.chest = {"dirty shirt", ARMOR, NONE, { -1, -1 }, 0, 1000, 0, -1},
		.leg = {"dirty pants", ARMOR, NONE, { -1, -1 }, 0, 1000, 0, -1},
		.feet = nothing_item
	};

	current_weapon = nothing_item;
}

void game_player::fight(game_enemy& enemy)
{
	if(current_weapon != nothing_item) {
		current_weapon.durability -= 1;
		if(rrange<int>(0, 100) < 60) {
			enemy.life -= (current_weapon.power - enemy.defense);
			alog.log_m("You attack the %s with your %s, "
						"inflicting %d damage.",
						enemy.name, current_weapon.name,
						current_weapon.power - enemy.defense);
		}
	} else {
		alog.log_m("You could not fight.");
	}
}

void game_player::walk(game_maze& maze, int xd, int yd)
{
	int x = maze.player.x + xd, y = maze.player.y + yd;

	// look for nearby entities
	for(int i : std::vector<int>{ -2, -1, 0, 1, 2 })
	for(int j : std::vector<int>{ -2, -1, 0, 1, 2 })
		switch(maze.describe(x + i, y + j)) {
		case ' ':
		case '#':
			break;

		case 'E':
			alog.log_m("You notice an enemy.");
			break;
				
		case 'I':
			alog.log_m("You notice an item.");
			break;

		case 'C':
			alog.log_m("You notice a chest.");
			break;
		}

	// handle entities that are being interacted with			
	switch(maze.describe(x, y)) {
	case ' ':
	case '#':
		break;

	case 'E':
		for(int i : std::vector<int>{ -1, 0, 1 })
		for(int j : std::vector<int>{ -1, 0, 1 })
			if(maze.describe(x + i, y + j) == 'E') {
				for(game_enemy& e : maze.enemies)
					if(e.coords.x == x + i &&
						e.coords.y == y + j)
						fight(e);
			}
			
		break;
		
	case 'I':
		alog.log_m("You notice an item.");
		for(game_item& i : maze.items)
			if(i.coords.x == x && i.coords.y == y)
				menu.item_menu(*this, i);
				
		break;
		
	case 'C':
		alog.log_m("You notice a chest.");
		for(game_chest& c : maze.chests)
			if(c.coords.x == x && c.coords.y == y)
				menu.chest_menu(*this, c);
				
		break;
								
	case '.':
		maze.player.x += xd;
		maze.player.y += yd;
		life -= 1;
		score += 1;

		if(!(score % 100)) {
			level += 1;
			alog.log_m("Level up!");
		}
		
		break;
	}
}

void game_player::look(game_maze maze)
{
	erase();

	std::string _type;

	switch(type) {
	case WARRIOR: _type = "Warrior"; break;
	case WIZARD: _type = "Wizard"; break;
	case ROGUE: _type = "Rogue"; break;
	}

	std::vector<std::string> data = {
		string_format("%s, %s, Level %d\n", name.c_str(), _type.c_str(), level),
		string_format("LIFE: %d\tSTR: %d\n", life, strength),
		string_format("DEF: %d\tSTH: %d\n", defense, stealth),
		string_format("X: %d\tY: %d\n", maze.player.x, maze.player.y),
		"\n",
		string_format("%s\n", alog.log.back().c_str()),
		"\n",
		"\n"
	};

	auto n = data.begin();
	for(int i = maze.player.y - 4; 
		i < maze.player.y + 4; i++) {
		for(int j = maze.player.x - 5; j < maze.player.x + 5; j++) {
			RPRINTW("%c", maze.describe(j, i));
		}
		
		RPRINTW(" | %s", (*n).c_str());
		n++;
	}
}

void game_player::death_screen()
{
	// display details
	
	erase();

	hscore.update(*this);
	
	RPRINTW("Player: %s\n", name.c_str());
		
	RPRINTW("Life: %d | Strength: %d\n"
			"Defense: %d | Stealth: %d\n"
			"Weight: %d\n\n"
			"Score: %d\n",
			life, strength, defense, stealth,
			weight, score);

	RPRINTW("[ Press any key to continue ]\n");
				
	getch();

	// display high scores
	erase();

	RPRINTW("High Scores\n\n");
	
	// [NAME] [TYPE] [LEVEL] [SCORE] [LIFE] [STRENGTH] [DEFENSE] [STEALTH]
	RPRINTW("Name\tType\tLevel\tScore\tLife\tSTR\tDEF\tSTH\n");

	for(game_player entry : hscore.get())
		RPRINTW("%s\t%s\t%d\t%d\t%d\t%d\t%d\t%d\n",
				entry.name.c_str(),
				N(entry.type).c_str(),
				entry.level,
				entry.score,
				entry.life,
				entry.strength,
				entry.defense,
				entry.stealth
				);

	RPRINTW("\n[ Press any key to continue ]\n");
	
	getch();
}
