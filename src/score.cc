#include "score.hh"

// [NAME] [TYPE] [LEVEL] [SCORE] [LIFE] [STRENGTH] [DEFENSE] [STEALTH]

std::string N(game_player_type type)
{
	switch(type) {
	default: case WARRIOR: return "Warrior";
	case WIZARD: return "Wizard";
	case ROGUE: return "Rogue";
	}
}

game_player_type T(std::string type)
{
	if(!type.compare("Warrior"))
		return WARRIOR;
	else if(!type.compare("Wizard"))
		return WIZARD;
	else if(!type.compare("Rogue"))
		return ROGUE;
	else
		return WARRIOR;
}

void game_score::update(game_player& player)
{
	out.open(SCOREFILE, std::ofstream::out | std::ofstream::app);
	
	out << string_format("%s %s %d %d %d %d %d %d",
						player.name.c_str(),
						N(player.type).c_str(),
						player.level,
						player.score,
						player.life,
						player.strength,
						player.defense,
						player.stealth) << std::endl;

	out.close();
}

game_player plparse(std::string line)
{
	game_maze _maze;
	game_player pl("UNINIT", WARRIOR, _maze);
	
	std::string n;
	std::istringstream is(line);
	
	is >> pl.name >> n;
	pl.type = T(n);
	
	is 	>> pl.level 
		>> pl.score 
		>> pl.life
		>> pl.strength 
		>> pl.defense 
		>> pl.stealth;

	return pl;
}

std::vector<game_player> game_score::get()
{
	in.open(SCOREFILE);
	
	std::vector<game_player> stats;

	if(in.is_open()) {
		std::string n;

		while(std::getline(in, n)) {
			stats.push_back(plparse(n));
		}
	}

	in.close();

	std::sort(stats.begin(), stats.end(), 
			[](auto a, auto b)
			{
				return a.score > b.score;
			});
			
	return stats;
}
