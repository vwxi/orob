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
	}
	
	int i = 0;
	int w_sum = 0;
	
	for(game_item item : p.inventory) {
		RPRINTW("%d)\ta %s (weight: %d)\n", i,
						item.name.c_str(),
						item.weight);
		w_sum += item.weight;
		i++;
	}

	RPRINTW("\n\nYou are currently carrying %d units of weight.\n", w_sum);
	RPRINTW("You can carry up to %d units of weight.\n", p.weight - w_sum);
	
	RPRINTW("\nWhat to do? (wWRucq): ");

	std::vector<game_item> _inventory;
	
	switch(getch()) {
	case 'W':
		RPRINTW("\nWhat to wear? (0-%d): ", i > 0 ? i - 1 : 0);

		int W;
		W = std::stoi(sgetstr());

		if(W > (int)p.inventory.size() - 1 || W < 0) {
			RPRINTW("Item does not exist.\n");
			getch();	
		} else if(p.inventory.at(W).type != ARMOR) {
			RPRINTW("Item is not armor.\n");
			getch();
		} else {
			armor_menu(p, p.inventory.at(W), WEAR);
			p.inventory.erase(p.inventory.begin() + W);
		}
			
		break;

	case 'R':
		break;
			
	case 'w': // wield
		RPRINTW("\nWhat to wield? (0-%d): ", i > 0 ? i - 1 : 0);

		int w;
		w = std::stoi(sgetstr());

		if(w > (int)p.inventory.size() - 1 || w < 0) {
			RPRINTW("Item does not exist.\n");
			getch();
		} else if(p.inventory.at(w).type != WEAPON) {
			RPRINTW("Item is not a weapon.\n");
			getch();
		} else if(p.current_weapon.name != nothing_item.name) {
			RPRINTW("You are already wielding a weapon.\n");
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
		p.strength -= p.current_weapon.power;
		p.inventory.push_back(p.current_weapon);
		p.current_weapon = nothing_item;

		for(game_item item : p.inventory)
			if(item == nothing_item) _inventory.push_back(item); // what? fix comparison voodoo later

		p.inventory = _inventory;
		_inventory = std::vector<game_item>{};
		
		break;
			
	case 'c': // consume
		RPRINTW("\nWhat to consume? (0-%d): ", i > 0 ? i - 1 : 0);

		int c;
		c = std::stoi(sgetstr());

		if(c > (int)p.inventory.size() - 1 || c < 0) {
			RPRINTW("Item does not exist.\n");
			getch();
		} else if(p.inventory.at(c).type != CONS) {
			RPRINTW("Item is not consumeable.\n");
			getch();
		} else {
			switch(p.inventory.at(c).stat) {
			case CNONE: default: break;
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

void game_menu::armor_menu(game_player& pl, game_item& i, game_armor_action a)
{
	erase();

	if(a == WEAR)
		RPRINTW("Add %s (weight: %d) as what? (hclf): ", 
				i.name.c_str(), i.weight)
	else
		RPRINTW("Remove what? (hclf): ");
		
	// this is sloppy.
	switch(getch()) {
	case 'h':
		if(pl.armor_set.head != nothing_item) {
			RPRINTW("\nYou are already wearing something on your head.\n");
			pl.inventory.push_back(i);
			getch();
		} else if(pl.weight < i.weight) {
			RPRINTW("\nItem is too heavy.\n");
			pl.inventory.push_back(i);
			getch();
		} else if(i.armor_type != HEAD) {
			RPRINTW("\nItem is not made for wearing on head.\n");
			pl.inventory.push_back(i);
			getch();
		} else {
			if(a == WEAR) {
				pl.armor_set.head = i;
				pl.defense += pl.armor_set.head.power;
			} else {
				pl.defense -= pl.armor_set.head.power;
				pl.inventory.push_back(pl.armor_set.head);

				pl.armor_set.head = nothing_item;
			}
		}
		
		break;

	case 'c': 
		if(pl.armor_set.chest != nothing_item) {
			RPRINTW("\nYou are already wearing something on your chest.\n");
			pl.inventory.push_back(i);
			getch();
		} else if(pl.weight < i.weight) {
			RPRINTW("\nItem is too heavy.\n");
			pl.inventory.push_back(i);
			getch();
		} else if(i.armor_type != CHEST) {
			RPRINTW("\nItem is not made for wearing on chest.\n");
			pl.inventory.push_back(i);
			getch();
		} else {
			if(a == WEAR) {
				pl.armor_set.chest = i;
				pl.defense += pl.armor_set.chest.power;
			} else {
				pl.defense -= pl.armor_set.chest.power;
				pl.inventory.push_back(pl.armor_set.chest);

				pl.armor_set.chest = nothing_item;
			}
		}
		
		break;

	case 'l':
		if(pl.armor_set.chest != nothing_item) {
			RPRINTW("\nYou are already wearing something on your legs.\n");
			pl.inventory.push_back(i);
			getch();
		} else if(pl.weight < i.weight) {
			RPRINTW("\nItem is too heavy.\n");
			pl.inventory.push_back(i);
			getch();
		} else if(i.armor_type != LEG) {
			RPRINTW("\nItem is not made for wearing on legs.\n");
			pl.inventory.push_back(i);
			getch();
		} else {
			if(a == WEAR) {
				pl.armor_set.leg = i;
				pl.defense += pl.armor_set.leg.power;
			} else {
				pl.defense -= pl.armor_set.leg.power;
				pl.inventory.push_back(pl.armor_set.leg);

				pl.armor_set.leg = nothing_item;
			}
		}
		
		break;

	case 'f':
		if(pl.armor_set.chest != nothing_item) {
			RPRINTW("\nYou are already wearing something on your feet.\n");
			pl.inventory.push_back(i);
			getch();
		} else if(pl.weight < i.weight) {
			RPRINTW("\nItem is too heavy.\n");
			pl.inventory.push_back(i);
			getch();
		} else if(i.armor_type != FEET) {
			RPRINTW("\nItem is not made for wearing on feet.\n");
			pl.inventory.push_back(i);
			getch();
		} else {
			if(a == WEAR) {
				pl.armor_set.feet = i;
				pl.defense += pl.armor_set.feet.power;
			} else {
				pl.defense -= pl.armor_set.feet.power;
				pl.inventory.push_back(pl.armor_set.feet);

				pl.armor_set.feet = nothing_item;
			}
		}

		break;

	default:
		break;
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

	default:
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
			default:
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
				c.items.erase(c.items.begin() + p);

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
		
	default:
		return;	
	}
}

void game_menu::player_menu(game_player p)
{
	erase();

	RPRINTW("%s's armor set:\n", p.name.c_str());

	
	RPRINTW("Head: %s (weight: %d)\n", 	
			p.armor_set.head.name.c_str(), p.armor_set.head.weight);
	RPRINTW("Chest: %s (weight: %d)\n", 
			p.armor_set.chest.name.c_str(), p.armor_set.chest.weight);
	RPRINTW("Legs: %s (weight: %d)\n", 	
			p.armor_set.leg.name.c_str(), p.armor_set.leg.weight);
	RPRINTW("Feet: %s (weight: %d)\n\n",
			p.armor_set.feet.name.c_str(), p.armor_set.feet.weight);

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
