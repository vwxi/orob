#ifndef UTIL_HH
#define UTIL_HH

#include <vector>
#include <random>
#include <memory>
#include <string>
#include <algorithm>

#include <ncurses.h>

#define RPRINTW(f, ...) \
	do { \
		printw(f, ## __VA_ARGS__); \
		refresh(); \
	} while(0);

std::string sgetstr();
	
template <typename T>
T rrange(T min, T max)
{	
	std::random_device _r;
	std::mt19937 _g(_r());

	return std::uniform_int_distribution<T>
			(min, max + 1)
			(_g);
}

template <typename T>
T relemv(std::vector<T> v)
{
	std::random_device _r;
	std::mt19937 _g(_r());

	std::shuffle(v.begin(), v.end(), _g);

	return v.front();
}

// thank you, internet
template<typename ... Args>
std::string string_format(const std::string& format, Args ... args )
{
	size_t size = snprintf(nullptr, 0, format.c_str(), args ...) + 1;
	if( size <= 0 ) { 
		return "";
	}
	
	std::unique_ptr<char[]> buf(new char[size]); 
	snprintf(buf.get(), size, format.c_str(), args ...);
	return std::string(buf.get(), buf.get() + size - 1);
}

#endif
