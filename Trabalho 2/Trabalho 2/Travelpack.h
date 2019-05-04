#pragma once

#include <string>
#include <vector>
#include "Date.h"

class Travelpack
{
public:
	Travelpack();
	~Travelpack();
private:
	size_t id;
	std::vector<std::string> destinations;
	Date begginning;
	Date end;
	double price_per_person;
	size_t max_bought_tickets;
	size_t bought_tickets;

	bool is_valid;
	std::string error_message;
};

