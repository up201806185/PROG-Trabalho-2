#include <iostream>
#include "utils.h"
#include "Date.h"
#include "Address.h"
#include "Travelpack.h"
#include "Client.h"

using utils::print;
int main()
{
	Travelpack().load("tp_test.txt");
	Client().load("client_test.txt");

	std::vector<Travelpack*> vect = Travelpack::fetch_by_destination("Veneza");
	for (size_t i = 0; i < vect.size(); i++) {
		vect.at(i)->pprint();
	}
}
