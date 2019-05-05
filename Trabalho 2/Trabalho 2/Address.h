#ifndef ADDRESS_H
#define ADDRESS_H

#include <string>
#include <vector>
#include "utils.h"
#include <ostream>
class Address
{
	friend std::ostream& operator<<(std::ostream& out, const Address & address);

public:
	Address();
	Address(std::istream & stream);
	Address(std::string input);
	Address(std::string street, unsigned short door_number, std::string floor, std::string postal_code, std::string location);
	~Address();

	bool parse(std::istream & stream);
	bool parse(std::string input);

	std::string get_street() const;
	unsigned short get_door_number() const;
	std::string get_floor() const;
	std::string get_postal_code() const;
	std::string get_location() const;
	std::string str() const;
	bool valid() const;
	std::string get_error() const;
private:
	std::string street; // street name
	unsigned short door_number; // door number
	std::string floor; // floor number ("-" is not applicable)
	std::string postal_code; // postal code
	std::string location; // site

	bool set_error(std::string error_str);
	void reset();
	std::string error_message;
	bool is_valid;
};

#endif