#include "Travelpack.h"



Travelpack::Travelpack()
{
}


Travelpack::~Travelpack()
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
