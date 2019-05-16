#include "utils.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <ctype.h>
#include <limits>

size_t char_count(const std::string & input, const char delim);

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

bool utils::read_num(const std::string & str, size_t & input)
{
	if (str[0] == '-')
		return false;

	std::istringstream temp_stream(str);

	temp_stream >> input;

	return (!temp_stream.fail());
}

void utils::wait_for_enter()
{
	std::string temp;

	utils::read_str(std::cin, temp, true);

	return;
}

void utils::print(const bool input, std::ostream & stream, std::string endl)
{
	if (input)
		stream << "True" << std::endl;
	else
		stream << "False" << std::endl;
	
	return;
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

std::string utils::join(const std::vector<std::string>& parts, const std::string & delimiter, size_t begginning_index)
{
	std::string result = "";
	for (size_t i = begginning_index; i < parts.size() - 1; i++)
	{
		result += parts[i];
		result += delimiter;
	}
	//Only the last element remains to be added
	result += parts[parts.size() - 1];

	return result;
}

std::string utils::uppercase(std::string input)
{
	for (char & i : input)
	{
		i = toupper(i);
	}

	return input;
}

std::string utils::lowercase(std::string input)
{
	for (char & i : input)
	{
		i = tolower(i);
	}

	return input;
}

void utils::clear_screen()
{
	system("cls");
}

std::string utils::yes_no_prompt(std::string prompt, std::string default_answer)
{
	std::string temp;

	while (true)
	{
		std::cout << prompt;
		if (!utils::read_str(std::cin, temp, true))
		{
			if (temp == "EOF")
				return "EOF";
			else
			{
				if (default_answer == "YES" || default_answer == "NO")
					return default_answer;
				continue;
			}
		}
		temp = utils::uppercase(temp);
		if (temp == "Y" || temp == "YES" || temp == "YEP")
			return "YES";
		else if (temp == "N" || temp == "NO" || temp == "NOPE")
			return "NO";
		else if (default_answer == "YES" || default_answer == "NO")
			return default_answer;
	}
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

bool utils::sortbysec(const std::pair<std::string, long>& a, const std::pair<std::string, long>& b)
{
	return (a.second > b.second);
}