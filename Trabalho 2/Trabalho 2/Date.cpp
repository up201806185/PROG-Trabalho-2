#include "Date.h"
#include "utils.h"
#include <string>
#include <sstream>
#include <vector>
#include <ctime>

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
	this->day = day;
	return check_validity();
}

bool Date::set_month(const unsigned short int month)
{
	this->month = month;
	return check_validity();
}

bool Date::set_year(const unsigned short int year)
{
	this->year = year;
	return check_validity();
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
	time_t timer = std::time(NULL);
	std::tm bt;
#if defined(__unix__)
	localtime_r(&timer, &bt);
#elif defined(_MSC_VER)
	localtime_s(&bt, &timer);
#else
	static std::mutex mtx;
	std::lock_guard<std::mutex> lock(mtx);
	bt = *std::localtime(&timer);
#endif
	
	year = 1900 + bt.tm_year;
	month = 1 + bt.tm_mon;
	day = bt.tm_mday;

	return;
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

std::string Date::str() const
{
	return std::to_string(year) + '/' + std::to_string(month) + '/' + std::to_string(day);
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
	switch (month)
	{
	case 1: case 3: case 5: case 7: case 8: case 10: case 12:
		return 31;
	case 4: case 6: case 9: case 11:
		return 30;
	case 2:
		if (leap_year())
			return 29;
		else
			return 28;
	}
}

bool Date::leap_year()
{//Stolen from 3.6
	if (year % 400 == 0) return true;

	if ((year % 4 == 0) && (year % 100 != 0)) return true;

	return false;
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
	if (
		((1900 < year) && (year < 2100)) &&
		((1 <= month) && (month <= 12)) &&
		((1 <= day) && (day <= days_in_month()))
		)
		return is_valid = true;
	else
		return set_error("Incoherent date error");
}

bool operator==(const Date & l, const Date & r)
{
	return (l.day == r.day) && (l.month == r.month) && (l.year == r.year);
}

bool operator>(const Date & l, const Date & r)
{
	if      (l.year > r.year) return true;
	else if (l.year < r.year) return false;

	if      (l.month > r.month) return true;
	else if (l.month < r.month) return false;

	if      (l.day > r.day) return true;
	else if (l.day < r.day) return false;

	return false;//They are equal
}

bool operator<(const Date & l, const Date & r)
{
	return !(operator==(l, r) || operator>(l, r));
}

bool operator!=(const Date & l, const Date & r)
{
	return operator==(l, r);
}

bool operator>=(const Date & l, const Date & r)
{
	return operator==(l, r) || operator>(l, r);
}

bool operator<=(const Date & l, const Date & r)
{
	return operator==(l, r) || operator<(l, r);
}

std::ostream & operator<<(std::ostream & stream, const Date & date)
{
	stream << date.str();
	return stream;
}
