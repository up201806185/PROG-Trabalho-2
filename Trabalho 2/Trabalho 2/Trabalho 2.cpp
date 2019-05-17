#include <iostream>
#include "utils.h"
#include "Date.h"
#include "Address.h"
#include "Travelpack.h"
#include "Client.h"
#include "pack_menu.h"
#include "client_menu.h"
#include "Agency.h"
#include "main_menu.h"

using utils::print;
int main()
{
	Agency agency;
	std::string agency_path;
	while (true)
	{
		std::cout << "Please enter the path of the Agency file:> ";
		if (utils::read_str(std::cin, agency_path))
			break;
	}
	
	agency.load(agency_path);
	main_menu(agency, agency_path);
}
