#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <string>
#include <ostream>
#include <fstream>
#include <sstream>
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
	Assigns the first integer it finds in 'stream' to 'input'
	Returns true if no error ocurred. Returns false if an error occurs. If input == the maximum possible representable datatype,
	an EOF happened while consuming the 'stream'
	Note: the entire line is consumed, therefore it will read the first integer from the line and discard the rest of the line
	Helpful tip: use this function from <limits> to get the maximum possible integer
	std::numeric_limits<your_datatype>::max();
	WARNING: The datatypes you can or cannot use with this function are not enforced, be careful
	*/
	template<class T>
	bool read_num(std::istream & stream, T & input);
	
	/**
	Assigns the first integer it finds in 'str' to 'input'
	Returns true if no error ocurred. Returns false if an error occurs. If input == the maximum possible representable datatype,
	an EOF happened while reading the 'str'
	Helpful tip: use this function from <limits> to get the maximum possible integer
	std::numeric_limits<your_datatype>::max();
	WARNING: The datatypes you can or cannot use with this function are not enforced, be careful
	*/
	template<class T>
	bool read_num(const std::string & str, T & input);

	/**
	Prints the entity to 'stream' (std::cout by default) followed by 'endl' ("\n") by default
	There is an optional second argument you can use to specify the output stream
	There is an optional third argument you can use to change 'endl' to whatever you wish
	*/
	void print(const bool input, std::ostream & stream = std::cout, std::string endl = "\n");

	/**
	Prints the entity to 'stream' (std::cout by default) followed by 'endl' ("\n") by default
	There is an optional second argument you can use to specify the output stream
	There is an optional third argument you can use to change 'endl' to whatever you wish
	*/
	template<class T>
	void print(const T & input, std::ostream & stream = std::cout, std::string endl = "\n");

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

	/**
	Returns a string formed by all the elements in 'parts' with the string 'delimiter' in between them
	More or less the opposite of utils::split
	You may optionally choose to concatenate only a part of the vector by choosing the begginning index as the third argument
	*/
	std::string join(const std::vector<std::string> & parts, const std::string & delimiter, size_t begginning_index = 0);

	/**
	Returns an uppercase version of the string 'input'
	Example: utils::uppercase("mieic") returns "MIEIC"
	*/
	std::string uppercase(std::string input);

	/**
	Returns an lowercase version of the string 'input'
	Example: utils::lowercase("MIEIC") returns "mieic"
	*/
	std::string lowercase(std::string input);
	
	/**
	Clears the console screen
	*/
	void clear_screen();
}

template <class T>
bool utils::read_num(std::istream & stream, T & input)
{
	input = 0;

	std::string temp_string;
	if (!read_str(stream, temp_string))
	{
		if (temp_string == "EOF" )
		{
			input = std::numeric_limits<T>::max();
		}
		return false;
	}

	return utils::read_num(temp_string, input);
}

template <class T>
bool utils::read_num(const std::string & str, T & input)
{
	std::istringstream temp_stream(str);

	temp_stream >> input;

	return (!temp_stream.fail());
}

template<class T>
void utils::print(const T & input, std::ostream & stream , std::string endl)
{
	stream << input << endl;
	return;
}


#endif