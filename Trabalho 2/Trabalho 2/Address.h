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
	~Address();

	std::string getStreet() const;
	unsigned short getDoorNumber() const;
	std::string getFloor() const;
	std::string getPostalCode() const;
	std::string getLocation() const;

private:
	std::string street; // street name
	unsigned short doorNumber; // doro number
	std::string floor; // floor number ("-" is not applicable)
	std::string postalCode; // postal code
	std::string location; // site

	bool set_error(std::string error_str);
	void reset();
	bool check_validity();
	std::string error_message;
	bool is_valid;
};

#endif