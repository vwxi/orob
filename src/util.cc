#include "util.hh"

std::string sgetstr()
{
	char *s = new char[1024];

	echo();
	getstr(s);
	noecho();

	return std::string(s);
}
