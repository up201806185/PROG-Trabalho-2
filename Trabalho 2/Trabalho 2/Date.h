#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <istream>
class Date
{
//friend:
//<< operator
//== operator
//< operator
//> operator
public:
	Date();
	Date(std::istream & stream);
	Date(std::string input);
	Date(unsigned short int day, unsigned short int month, unsigned short int year);

	~Date();

	bool set_day(const unsigned short int day);
	bool set_month(const unsigned short int month);
	bool set_year(const unsigned short int year);
	bool parse(std::istream & stream);
	bool parse(std::string input);
	void set_today();

	void get_day();
	void get_month();
	void get_year();
	std::string get_date();
private:
	unsigned short int days_in_month();
	unsigned short int days_in_feb();
	void reset_date();
	std::string error_message;
	bool is_valid;
	unsigned short int day;
	unsigned short int month;
	unsigned short int year;
};
/*
tm currentDate(time_t timer) {
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
	return bt;
}
*/

#endif