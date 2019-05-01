#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <fstream>
#include <vector>

namespace utils
{
	//Input handling
	bool read_str(std::istream & stream, std::string & input, bool read_only_one_string = false);


	//Usefull string operations
	void trim(std::string & input);
	std::vector<std::string> split(std::string input, const char delimiter, size_t max_splits = -1); // -1 will make it loop back to its maximum value
}

#endif