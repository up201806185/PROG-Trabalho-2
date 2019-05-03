#include "Date.h"
#include "utils.h"
#include <string>
#include <sstream>
#include <vector>

Date::Date()
{
	reset();
}

Date::Date(std::istream & stream)
{
	parse(stream);
}

Date::Date(std::string input)
{
	parse(input);
}

Date::Date(unsigned short int day, unsigned short int month, unsigned short int year)
{
	this->day = day;
	this->month = month;
	this->year = year;
	check_validity();
	return;
}


Date::~Date()
{
}

bool Date::set_day(const unsigned short int day)
{
	return false;
}

bool Date::set_month(const unsigned short int month)
{
	return false;
}

bool Date::set_year(const unsigned short int year)
{
	return false;
}

bool Date::parse(std::istream & stream)
{
	std::string input;
	if (!utils::read_str(stream, input))
	{
		if (input == "EOF")
			return set_error("EOF");
		else
			return set_error("Parsing error");
	}

	return parse(input);
}

bool Date::parse(std::string input)
{
	std::vector<std::string> parts = utils::split(input, '/');
	if (parts.size() > 3)
	{
		set_error("Parsing error");
	}

	if (utils::read_num(parts[0], year) && utils::read_num(parts[1], month) && utils::read_num(parts[2], day))
	{
		return check_validity();
	}
	else
	{
		return set_error("Parsing error");
	}
}

void Date::set_today()//Help me Rafael
{
}

unsigned short int Date::get_day()
{
	return day;
}

unsigned short int Date::get_month()
{
	return month;
}

unsigned short int Date::get_year()
{
	return year;
}

std::string Date::get_date()
{
	return std::string();
}

bool Date::valid()
{
	return this->is_valid;
}

bool Date::set_error(std::string error_str)
{
	is_valid = false;
	error_message = error_str;
	return is_valid;
}

unsigned short int Date::days_in_month()
{
	return 0;
}

unsigned short int Date::days_in_feb()
{
	return 0;
}

void Date::reset()
{
	day   = 0;
	month = 0;
	year  = 0;
	set_error("Uninitialized date");
}

bool Date::check_validity()
{
	
}
