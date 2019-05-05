#ifndef DATE_H
#define DATE_H

#include <string>
#include <ostream>
class Date
{
	friend bool operator==(const Date & l, const Date & r);
	friend bool operator> (const Date & l, const Date & r);
	friend bool operator< (const Date & l, const Date & r);
	friend bool operator!=(const Date & l, const Date & r);
	friend bool operator>=(const Date & l, const Date & r);
	friend bool operator<=(const Date & l, const Date & r);

	friend std::ostream& operator<<(std::ostream& stream, const Date & date);
public:
	Date();
	Date(std::istream & stream);
	Date(std::string input);
	Date(unsigned short int day, unsigned short int month, unsigned short int year);

	~Date();

	bool parse(std::istream & stream);
	bool parse(std::string input);
	void set_today();

	unsigned short int get_day();
	unsigned short int get_month();
	unsigned short int get_year();
	std::string str() const;
	bool valid();
	std::string get_error();
private:
	bool set_error(std::string error_str);
	unsigned short int days_in_month();
	bool leap_year();
	void reset();
	bool check_coherence();
	std::string error_message;
	bool is_valid;
	unsigned short int day;
	unsigned short int month;
	unsigned short int year;
};

#endif