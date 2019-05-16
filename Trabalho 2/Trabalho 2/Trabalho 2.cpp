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
	
	//std::cout << "Please enter the path of the Agency file:> ";
	//utils::read_str(std::cin, agency_path);
	agency_path = "agency_test.txt";
	agency.load(agency_path);
	main_menu(agency, agency_path);
}

///////////////fix show all sold packs aesthetic
//client creation details