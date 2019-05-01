#include "utils.h"

#include <fstream>
#include <string>
#include <sstream>

bool utils::read_str(std::istream & stream, std::string & input, bool read_only_one_string)
{
	input = "";

	while (true)
	{
		std::getline(stream, input);

		if (stream.fail())
		{
			if (stream.eof())
			{
				input = "EOF";//This is merely a way to know wether ctr+Z was pressed or not
				stream.clear();//Ignore isn't needed
			}
			else
			{
				stream.clear();
				stream.ignore(100000, '\n');
			}
			return false;

		}

		utils::trim(input);

		if (input != "" || read_only_one_string)//if read_only_one_string is true, the loop will only execute once
		{
			return true;
		}
	}
}

void utils::trim(std::string & input)
{
	const std::string WHITE_SPACE = " \t\n\v\f\r";//C locale white space

	input.erase(0, input.find_first_not_of(WHITE_SPACE));//Left

	input.erase(input.find_last_not_of(WHITE_SPACE) + 1, std::string::npos);//Right

	return;
}

std::vector<std::string> utils::split(std::string input, const char delimiter, size_t max_splits)
{
	input += '\n';//This is done so that I can get the last portion of the string by using "std::getline(line_stream, temp, '\n');"
	std::string temp;
	std::vector<std::string> result;
	std::stringstream line_stream(input);

	size_t delim_count = char_count(input, delimiter);

	if (max_splits < delim_count) delim_count = max_splits;

	for (size_t i = 0; i < delim_count; i++)
	{
		std::getline(line_stream, temp, delimiter);
		utils::trim(temp);
		result.push_back(temp);
	}

	std::getline(line_stream, temp, '\n');
	utils::trim(temp);
	result.push_back(temp);

	return result;
}


/**
Returns the number of ocurrences of char 'delim' in the string 'input'
*/
size_t char_count(const std::string & input, const char delim)
{
	size_t count = 0;
	for (char i : input)
	{
		if (i == delim) count++;
	}

	return count;
}