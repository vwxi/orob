#ifndef SCORE_HH
#define SCORE_HH

#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
#include <iterator>

#include "util.hh"
#include "player.hh"

#define SCOREFILE "scores"

struct game_score
{
	std::ifstream in;
	std::ofstream out;

	void update(game_player&);
	std::vector<game_player> get();
} static hscore;

std::string N(game_player_type);
game_player_type T(std::string);

#endif
