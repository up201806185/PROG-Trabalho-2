#include "Address.h"
#include "utils.h"


Address::Address()
{
	reset();
}

Address::Address(std::istream & stream)
{
	parse(stream);
}

Address::Address(std::string input)
{
	parse(input);
}

Address::Address(std::string street, unsigned short door_number, std::string floor, std::string postal_code, std::string location)
{
	this->street = street;
	this->door_number = door_number;
	this->floor = floor;
	this->postal_code = postal_code;
	this->location = location;
	this->is_valid = true;
}


Address::~Address()
{
}

bool Address::parse(std::istream & stream)
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

bool Address::parse(std::string input)
{
	std::vector<std::string> parts = utils::split(input, '/');
	if (parts.size() != 5)
	{
		return set_error("Parsing error");
	}
	street = parts[0];

	if (!utils::read_num(parts[1], door_number))
		return set_error("Parsing error");
	floor = parts[2];
	postal_code = parts[3];
	location = parts[4];
	return is_valid = true;
}

std::string Address::get_street() const
{
	return street;
}

unsigned short Address::get_door_number() const
{
	return door_number;
}

std::string Address::get_floor() const
{
	return floor;
}

std::string Address::get_postal_code() const
{
	return postal_code;
}

std::string Address::get_location() const
{
	return location;
}

std::string Address::str() const
{
	return street + " / " + std::to_string(door_number) + " / " + floor + " / " + postal_code + " / " + location;
}

bool Address::valid() const
{
	return is_valid;
}

std::string Address::get_error() const
{
	return error_message;
}

bool Address::set_error(std::string error_str)
{
	is_valid = false;
	error_message = error_str;
	return is_valid;
}

void Address::reset()
{
	street = ""; 
	door_number = 0;
	floor = "";
	postal_code = "";
	location = "";
	set_error("Uninitialized address");
}

std::ostream & operator<<(std::ostream & stream, const Address & address)
{
	stream << address.str();
	return stream;
}
