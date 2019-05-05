#include "Travelpack.h"

Travelpack::Travelpack()
{
}

Travelpack::~Travelpack()
{
}

void Travelpack::load(const std::string & path)
{
}

bool Travelpack::purchase_n_tickets(unsigned short n_tickets)
{
	if (bought_tickets + n_tickets > max_bought_tickets)
	{
		return false;
	}
	else if (bought_tickets + n_tickets == max_bought_tickets)
	{
		bought_tickets += n_tickets;
		available = false;//We're at full capacity
		return true;
	}
	else
	{
		bought_tickets += n_tickets;
		return true;
	}
}

bool Travelpack::check_coherence()
{
	if (begginning > end)
	{
		return set_error("Incoherency error: The beggining date comes later than the finish date");
	}

	if (Date().get_today() > begginning && available)
	{
		return set_error("Incoherency error: The begginning date comes sooner than today, yet the travelpack is set as available");
	}

	if (bought_tickets >= max_bought_tickets && available)
	{
		return set_error("Incoherency error: The travelpack is sold out, yet the travelpack is set as available");
	}

	return is_valid = true;
}

bool Travelpack::set_error(std::string error_str)
{
	is_valid = false;
	error_message = error_str;
	return is_valid;
}
