#ifndef LOG_HH
#define LOG_HH

#include <vector>
#include <string>

#include "util.hh"

struct _log
{
	std::vector<std::string> log;

	_log()
	{
		log.push_back("You start your journey.");
	}

	template <class ... Args>
	void log_m(std::string m, Args ... args)
	{
		log.push_back(string_format(m, args...));	
	}
} static alog;

#endif
