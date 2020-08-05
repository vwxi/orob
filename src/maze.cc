#include <vector>
#include <iostream>

#include "util.hh"
#include "item.hh"
#include "maze.hh"
#include "coord.hh"
#include "log.hh"

// initialize the maze
game_maze::game_maze()
{
	// dimensions
	length = 200;
	width = 200;

	player = { .x = 0, .y = 0 };
	
	up = { .x = rrange<int>(0, width), .y = rrange<int>(0, length) };
	down = { .x = rrange<int>(0, width), .y = rrange<int>(0, length) };
	
	// prevent exceptions
	chests.resize(1);
	enemies.resize(1);
	items.resize(1);
	walls.resize(1);
	
	// insert walls
	int _r = rrange<int>(100, (length*width)/4);
	
	for(int i = 0; i < _r; i++) {
		game_coord c = { .x = rrange<int>(0, width), .y = rrange<int>(0, length) };
		walls.push_back(c);
	}

	// insert chests (based on percentages or w/e)
	_r = rrange<int>(100, length);
	
	for(int i = 0; i < _r; i++) { // 30 chests could appear in the maze
		if(rrange<int>(0, 100) < 90) { // but not all chests may appear (40% chance)
			game_chest c;
			
			c.coords = { .x = rrange<int>(0, width), .y = rrange<int>(0, length) };

			c.difficulty = rrange<int>(5, 7);
			
			std::vector<game_item> _items;

			for(int i = 0; i < rrange<int>(0, rrange(1, 10)); i++) {
				game_item _item = relemv<game_item>(global_items);

				if(_item.rarity > rrange<int>(0, 100))
					_items.push_back(_item);
			}
			
			c.items = _items;
			chests.push_back(c);
		}
	}

	// insert item on ground
	_r = rrange<int>(100, length);
	
	for(int i = 0; i < _r; i++) { // 40 items could appear in the maze
		if(rrange<int>(0, 100) > 40) { // but not all items may appear (40% chance)
			game_item it = relemv<game_item>(global_items);

			it.coords = { .x = rrange<int>(0, width), .y = rrange<int>(0, length) };

			items.push_back(it);
		}
	}

	// insert mystery potions
	_r = rrange<int>(50, length);
					
	for(int i = 0; i < _r; i++) {
		if(rrange<int>(0, 100) > 30) {
			game_item it = {
				.name = "mystery potion",
				.type = CONS,
				.stat = relemv<game_cons_stat_type>(
					std::vector<game_cons_stat_type>
					{CNONE, LIFE, STR, DEF, STH}
				),
				.armor_type = ANONE,
				.coords = { .x = rrange<int>(0, width),
							.y = rrange<int>(0, length) },
				.weight = 1,
				.durability = 1,
				.power = rrange<int>(-10, 10),
				.rarity = 30
			};

			items.push_back(it);
		}
	}

	// insert enemies in maze
	_r = rrange<int>(0, 100);
	
	for(int i = 0; i < _r; i++) { // 20 enemies could appear
		if(rrange<int>(0, 100) < 60) { // but not all enemies may appear (60% chance)
			game_enemy e = relemv<game_enemy>(global_enemies);

			e.coords = try_coords();

			enemies.push_back(e);
		}
	}
}

// describe a tile on the maze with a single character
char game_maze::describe(int x, int y)
{
	// out of bounds?
	if(x > width || y > length ||
		x < 0 || y < 0) return ' ';

	// search walls
	for(game_coord wall : walls)
		if(wall.x == x && wall.y == y) return '#';

	// search items
	for(game_item item : items)
		if(item.coords.x == x && item.coords.y == y) return 'I';

	// search chests
	for(game_chest chest : chests)
		if(chest.coords.x == x && chest.coords.y == y) return 'C';

	// search enemies
	for(game_enemy enemy : enemies)
		if(enemy.coords.x == x && enemy.coords.y == y) return 'E';

	// search stairs
	if(up.x == x && up.y == y) return '<';
	if(down.x == x && down.y == y) return '>';
	
	// is player there?
	if(x == player.x && y == player.y) return '@';
	 
	return '.';
}

// for each move in the game, enemies have to move.
// enemies will move one tile towards the player on
// each move.

game_coord game_maze::try_coords()
{
	game_coord rc = { .x = rrange<int>(0, width), 
					  .y = rrange<int>(0, length) };

	switch(describe(rc.x, rc.y)) {
	case '#': case 'E': case 'I': case 'C': 
	case '>': case '<': 
		try_coords();
		break;
	}

	return rc;
}

game_coord game_maze::try_coords(game_enemy& e)
{
	game_coord rc = { .x = e.coords.x + rrange<int>(-2, 2),
					  .y = e.coords.y + rrange<int>(-2, 2) };

	switch(describe(rc.x, rc.y)) {
	case '#': case 'E': case 'I': case 'C':
		try_coords(e);
		break;
	}

	return rc;
}

void game_maze::cycle(game_player& player)
{
	// iterate through each enemy in the maze and move
	// them according to the environment (not hitting walls)
	for(game_enemy& enemy : enemies) {
		enemy.coords = try_coords(enemy);

		for(int i : std::vector<int>{ -1, 0, 1 })
			for(int j : std::vector<int>{ -1, 0, 1 }) {
				switch(describe(enemy.coords.x + i, enemy.coords.y + j)) {
				case '@':
					int dmg = rrange<int>(enemy.strength - 4,
											enemy.strength + 4);
											
					if(rrange<int>(0, 100) < 40 &&
						rrange<int>(0, 100) > player.stealth * 5) {	
						alog.log_m("A %s attacks you with %d damage.",
								enemy.name, dmg);

						player.life -= (dmg - player.defense);
					} else {
						alog.log_m("A %s tries to attack you and misses.",
									enemy.name);
					}
					
					break;
				}
			}
	}
}
