#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <fstream>

namespace utils
{
	bool read_str(std::istream & stream, std::string & input, bool read_only_one_string = false);

}

#endif