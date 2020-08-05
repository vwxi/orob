#include "player.hh"
#include "maze.hh"
#include "enemy.hh"
#include "item.hh"
#include "util.hh"
#include "coord.hh"
#include "log.hh"
#include "menu.hh"
#include "score.hh"

#define VERSION "v0.2"
#define CTIME __DATE__ " " __TIME__

void dq2_init()
{
	initscr();
	cbreak();
	noecho();
	keypad(stdscr, TRUE);

	RPRINTW(
	"Orob " VERSION "\n"
	"Written by hunj\n"
	"This game will always be free (as in freedom and free beer)\n\n"
	"Press any key to continue\n"
	);
		
	getch();

	erase();
}

void dq2_end()
{	
	erase();
	endwin();
}

game_player dq2_init_player(game_maze& maze)
{	
	std::string type;
	 
	game_player_type t;
	
	RPRINTW("Name: ");

	std::string name(sgetstr());
	
	RPRINTW("\nChoose character type.\n"
			"1) Warrior\n"
			"2) Wizard\n"
			"3) Rogue\n\n"
			"Choose wisely [Warrior]: ");

	switch(getch()) {
	default:
	case '1':
		t = WARRIOR;
		type = "Warrior";
		break;
	case '2':
		t = WIZARD;
		type = "Wizard";
		break;
	case '3':
		t = ROGUE;
		type = "Rogue";
		break;
	}

	RPRINTW("\n\nYour character:\n"
			"Name: %s\n"
			"Type: %s\n", name.c_str(), type.c_str());

	game_player player(name, t, maze);
	
	RPRINTW("\nIs this good? [y/N]: ");

	switch(getch()) {
	case 'y':
	case 'Y':
		noecho();
		erase();
		return player;
		break;

	default:
		noecho();
		erase();
		return dq2_init_player(maze);
		break;
	}

	return player;
}

int main()
{
	std::cout << "Generating floors..." << std::endl;

	auto G_R = []{ game_maze m; return m; };
	
	for(int i = 0; i < 20; i++) {
		floors.push_back(G_R());
	}

	floors.at(0).up = { -1, -1 };
	
	current = 0;
	
	dq2_init();
		
	game_player player = dq2_init_player(floors.at(current));
	
	while(1) {	
		if(player.life <= 0) {
			erase();
			
			alog.log_m("You are dead.");

			player.death_screen();
			break;
		}

		if(player.life < 100) alog.log_m("You are dying.");
		
		floors.at(current).cycle(player);
				
		player.look(floors.at(current));

		switch(getch()) {
		case 'm':
			menu.spawn_menu(player);
			
		case 'w': case 'W': case KEY_UP:
			player.walk(floors.at(current), 0, -1);
			break;
			
		case 'a': case 'A': case KEY_LEFT:
			player.walk(floors.at(current), -1, 0);
			break;
			
		case 's': case 'S': case KEY_DOWN:
			player.walk(floors.at(current), 0, 1);
			break;
			
		case 'd': case 'D': case KEY_RIGHT:
			player.walk(floors.at(current), 1, 0);
			break;

		#ifdef DEBUG
		case 'l':
			erase();
			for(std::string s : alog.log)
				RPRINTW("%s\n", s.c_str());

			getch();
			
			break;
		#endif
		
		default: break;
		}
	}
		
	dq2_end();

	return 0;
}
