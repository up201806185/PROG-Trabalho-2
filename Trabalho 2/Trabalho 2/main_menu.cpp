#include "main_menu.h"
#include "client_menu.h"
#include "pack_menu.h"

void main_menu(Agency agency, std::string agency_path) {
	while (true)
	{
		std::string input;
		utils::clear_screen();
		std::cout << "                    Agency info" << std::endl;
		agency.pprint();
		std::cout << std::endl;
		std::cout << "  Main Menu" << std::endl;
		std::cout << "[1]: Clients" << std::endl;
		std::cout << "[2]: Travelpacks" << std::endl;
		std::cout << "[3]: Save" << std::endl;
		std::cout << std::endl;
		std::cout << "Please choose an option. If you wish to exit, enter '0' or use ctrl+Z" << std::endl;
		std::cout << ":> ";

		if (!utils::read_str(std::cin, input, true))
		{
			if (input == "EOF")
			{
				agency.save(agency_path);
				break;
			}
			else
			{
				continue;
			}
		}

		if (input == "0") break;
		if (input == "1") client_menu();
		if (input == "2") pack_menu();
		if (input == "3") save_menu(agency, agency_path);

		continue;
	}
}

void save_menu(Agency & agency, std::string agency_path)
{
	while (true)
	{
		std::string input;
		utils::clear_screen();
		std::cout << "Do you wish to save? (Y/n):> ";
		if (!utils::read_str(std::cin, input, true))
		{
			if (input == "EOF") return;
		}

		if (input == "N" || input == "n")
		{
			std::cout << "Save cancelled, press enter to exit:> ";
			utils::read_str(std::cin, input, true);
			return;
		}
		else
		{
			std::cout << "Do you wish to keep the same path?" << std::endl;
			std::cout << "(" << agency_path << ")" << std::endl;
			std::cout << "(Y/n):> ";
			if (!utils::read_str(std::cin, input, true))
			{
				if (input == "EOF") return;
			}

			if (input == "N" || input == "n")
			{
				while (true)
				{
					std::cout << "Please enter the new path:> " << std::endl;
					if (!utils::read_str(std::cin, input))
					{
						if (input == "EOF") return;
						else continue;
					}
					else
					{
						agency_path = input;
						break;
					}
				}
			}

			std::cout << "Saving..." << std::endl;
			if (agency.save(agency_path))
			{
				std::cout << "Success! Press enter to continue:> ";
				utils::read_str(std::cin, input, true);
				break;
			}
			else
			{
				std::cout << "An error ocurred, do you wish to try again?(Y/n):>";
				if (!utils::read_str(std::cin, input, true))
				{
					if (input == "EOF") return;
				}

				if (input == "N" || input == "n")
				{
					return;
				}
				else
				{
					continue;
				}
			}
		}

	}
}