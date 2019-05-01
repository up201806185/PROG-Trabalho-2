#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <fstream>
#include <vector>

namespace utils
{
	/**
	The first line in 'stream' that isn't whitespace is trimmed and assigned to 'input'
	You may optionally set the third argument to 'true' so that only one line is read (useful for input prompts)
	Returns true if no error ocurred. Returns false if an error occurs. If input == "EOF", an EOF happened while consuming the 'stream'
	Note: only check for EOF if utils::read_str returns false
	*/
	bool read_str(std::istream & stream, std::string & input, bool read_only_one_string = false);


	/**
	Trims all whitespace at the beggining and end of the string 'input'
	Example: input = "  \tMIEIC   \v\n"; utils::trim(input); Now, input == "MIEIC"
	*/
	void trim(std::string & input);

	/**
	Returns a vector made of partitions of string 'input' that were seperated by the char 'delimiter'. Note: the partitions are automatically trimmed
	Example: utils::split("a, ,b, c", ',') will return {"a", "", "b", "c"}, notice how the whitespace has been automatically trimmed

	There is an optional third argument, 'max_splits' which indicate the maximum number of splits that can happen
	Example: utils::split("a, ,b, c", ',', 1) will return {"a", ",b, c"}
	*/
	std::vector<std::string> split(std::string input, const char delimiter, size_t max_splits = -1); // -1 will make it loop back to its maximum value
}

#endif