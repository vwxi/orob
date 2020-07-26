#include "menu.hh"
#include "util.hh"
#include "log.hh"

#include <cstring>

void game_menu::inventory_menu(game_player& p)
{
	erase();
	
	RPRINTW("Inventory:\n");
	
	if(p.inventory.empty()) {
		RPRINTW("Nothing");
		getch();
	} else {
		int i = 0;
		int w_sum = 0;
		
		for(game_item item : p.inventory) {
			RPRINTW("%d)\ta %s (weight: %d)\n", i,
							item.name.c_str(),
							item.weight);
			w_sum += item.weight;
			i++;
		}

		RPRINTW("\nYou are currently carrying %d units of weight.\n", w_sum);
		RPRINTW("You can carry up to %d units of weight.\n", p.weight - w_sum);
		
		RPRINTW("\nWhat to do? (wucq): ");
		
		switch(getch()) {
		case 'w': // wield
			RPRINTW("\nWhat to wield? (0-%d): ", i > 0 ? i - 1 : 0);

			int w;
			w = std::stoi(sgetstr());

			if(w > (int)p.inventory.size() - 1 || w < 0) {
				RPRINTW("\nItem does not exist.\n");
				getch();
			} else if(p.inventory.at(w).type != WEAPON) {
				RPRINTW("\nItem is not a weapon.\n");
				getch();
			} else if(p.current_weapon.name != nothing_item.name) {
				RPRINTW("\nYou are already wielding a weapon.\n");
				getch();
			} else {
				p.current_weapon = p.inventory.at(w);
				p.inventory.erase(p.inventory.begin() + w);

				p.strength += p.current_weapon.power;
				
				alog.log_m(
				string_format(
				"You wield your %s",
				p.current_weapon.name.c_str()
				));
			}
			
			break;
			
		case 'u': // unwield
			if(!p.current_weapon) {
				RPRINTW("\nYou are not currently wielding anything.\n");
				getch();
			} else {
				p.strength -= p.current_weapon.power;
				
				p.inventory.push_back(p.current_weapon);
				p.current_weapon = nothing_item;
			}
			break;
			
		case 'c': // consume
			RPRINTW("\nWhat to consume? (0-%d): ", i > 0 ? i - 1 : 0);

			int c;
			c = std::stoi(sgetstr());

			if(c > (int)p.inventory.size() - 1 || c < 0) {
				RPRINTW("\nItem does not exist.\n");
				getch();
			} else if(p.inventory.at(c).type != CONS) {
				RPRINTW("\nItem is not consumeable.\n");
				getch();
			} else {
				switch(p.inventory.at(c).stat) {
				case NONE: 	default: break;
				case LIFE: 	p.life 		+= p.inventory.at(c).power; break;
				case STR: 	p.strength 	+= p.inventory.at(c).power; break;
				case DEF:	p.defense 	+= p.inventory.at(c).power; break;
				case STH:	p.stealth	+= p.inventory.at(c).power; break;
				}

				p.weight -= p.inventory.at(c).weight;
				p.inventory.erase(p.inventory.begin() + c);				
			}
			
			break;

		case 'q': // quit
			break;
		}
	}
}

void game_menu::item_menu(game_player& pl, game_item& i)
{
	RPRINTW("Interact with item? [y/N]: ");

	switch(getch()) {
	case 'y': case 'Y':
		erase();

		RPRINTW("Item: a %s (weight: %d)\n\n",
				i.name.c_str(),
				i.weight);

		RPRINTW("What to do? (pq): ")
		switch(getch()) {
		case 'p':
			if(i.weight > pl.weight) {
				RPRINTW("\nItem is too heavy.\n");
				break;
			} else {
				pl.weight -= i.weight;
				pl.inventory.push_back(i);
				i = nothing_item;
			}
		}

		break;

	default: case 'n': case 'N':
		break;
	}
}

void game_menu::chest_menu(game_player& pl, game_chest& c)
{
	RPRINTW("\nTry opening chest? [y/N]: ");
	switch(getch()) {
	case 'y': case 'Y':
		if(pl.current_weapon != nothing_item)
			pl.current_weapon.durability -= 1;
		else
			pl.life -= 1;
			
		c.difficulty -= pl.strength;

		if(c.difficulty > pl.strength) {
			RPRINTW("\nThe chest would not open. Try again? [Y/n]: ");

			switch(getch()) {
			default: case 'y': case 'Y':
				chest_menu(pl, c);
			case 'n': case 'N':
				break;
			}
		}
		
		erase();

		RPRINTW("Chest:\n");

		int i;
		i = 0;
		
		for(game_item item : c.items) {
			RPRINTW("%d)\ta %s (weight: %d)\n", 
					i,
					item.name.c_str(),
					item.weight);

			i++;
		}

		RPRINTW("Do what with chest? (pdq): ");
		switch(getch()) {
		case 'p':
			RPRINTW("\nPick up which item? (0-%d): ", i > 0 ? i - 1 : 0);
	
			int p;
			p = std::stoi(std::string(sgetstr()));

			if(p > (int)c.items.size() - 1 || p < 0) {
				RPRINTW("\nItem does not exist.\n");
				getch();
			} else if(c.items.at(p).weight > pl.weight) {
				RPRINTW("\nItem is too heavy.\n");
				getch();
			} else {
				pl.inventory.push_back(c.items.at(p));
				c.items.erase(c.items.begin() + i);

				pl.weight -= pl.inventory.back().weight;
				
				alog.log_m(
				string_format(
				"You pick up a %s",
				pl.inventory.back().name.c_str()
				));
			}

			break;
			
		case 'd':
			int i = 0;

			RPRINTW("\n");
			
			for(game_item& item : pl.inventory)
				RPRINTW("%d)\ta %s (weight: %d)\n",
						i, item.name.c_str(), item.weight);

			RPRINTW("\nDrop which item into chest? (0-%d): ",
						i > 0 ? i - 1 : 0);

			int d = std::stoi(sgetstr());

			if(d > (int)pl.inventory.size() - 1 || d < 0) {
				RPRINTW("\nItem does not exist.\n");
				getch();
			} else {
				c.items.push_back(pl.inventory.at(d));
				pl.inventory.erase(pl.inventory.begin() + d);
			}

			break;
		}
		
		break;
		
	default: case 'n': case 'N':
		return;	
	}
}

void game_menu::player_menu(game_player p)
{
	erase();

	RPRINTW("%s's armor set:\n", p.name.c_str());

	auto C = [](auto i) { 
		return (!i) ? "Nothing" : i.name.c_str();
	};
	
	RPRINTW("Head: %s (weight: %d)\n", 	C(p.armor_set.head), p.armor_set.head.weight);
	RPRINTW("Chest: %s (weight: %d)\n", C(p.armor_set.chest), p.armor_set.chest.weight);
	RPRINTW("Legs: %s (weight: %d)\n", 	C(p.armor_set.leg), p.armor_set.leg.weight);
	RPRINTW("Feet: %s (weight: %d)\n\n",C(p.armor_set.feet), p.armor_set.feet.weight);

	RPRINTW("%s's current weapon: %s (weight: %d)\n",
			p.name.c_str(),
			p.current_weapon.name.c_str(),
			p.current_weapon.weight);
			
	RPRINTW("\n[ Press any key to continue ]\n");

	getch();
}

void game_menu::spawn_menu(game_player& p)
{
	erase();

	RPRINTW("Orob Menu:\n\n"
			"View inventory - i\n"
			"View player - p\n"
			"Quit game - q\n\n"
			"What to do? ");
			
	switch(getch()) {
	case 'i':
		inventory_menu(p);
		break;

	case 'p':
		player_menu(p);
		break;

	case 'q':
		p.death_screen();
		
		endwin();
		exit(0);
		break;
		
	default: break;
	}
}
