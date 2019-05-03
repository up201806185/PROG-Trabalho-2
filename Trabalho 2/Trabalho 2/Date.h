#pragma once
#include <string>
class Date
{
public:
	Date();
	~Date();
private:
	std::string error_message;
	bool is_valid;
	unsigned short int day;
	unsigned short int month;
	unsigned short int year;
};
