#include "client_menu.h"
#include "Client.h"

void client_submenu(Client * ptr)
{
	if (ptr == nullptr) return;

	while (true) {
		Client selected_client = *ptr;

		utils::clear_screen();
		selected_client.pprint();

		std::cout << "Options" << std::endl;
		std::cout << "[1]: Edit costumer" << std::endl;
		std::cout << "[2]: Delete costumer" << std::endl;
		std::cout << "[3]: Make a purchase" << std::endl;
		std::cout << "[4]: Show bought packs' information" << std::endl;
		std::cout << std::endl;
		std::cout << "Please choose an option. If you wish to exit, enter '0' or use ctrl+Z" << std::endl;
		std::cout << ":> ";

		std::string input;

		if (!utils::read_str(std::cin, input, true))
		{
			if (input == "EOF")
			{
				break;
			}
			else
			{
				continue;
			}
		}

		if (input == "0") break;
		if (input == "1")
		{
			selected_client.edit();
		}
		if (input == "2")
		{
			Client::erase(ptr);
			return;
		}
		if (input == "3")
		{
			/*size_t pack_index = select_travel_pack(agency);
			if (!agency.travel_packs[pack_index].is_available || agency.travel_packs[pack_index].sold_packs + 1 > agency.travel_packs[pack_index].total_packs)
			{
				std::cout << "Pack is not available, cancelling purchase" << std::endl;
				std::cout << "Press enter to continue:> ";
				utils::read_str(std::cin, input, true);
				continue;
			}
			else
			{
				agency.travel_packs[pack_index].sold_packs++;
				agency.clients[index].travel_packs_bought_ids.push_back(agency.travel_packs[pack_index].id);
				std::cout << "Purchase successful" << std::endl;
				std::cout << "Press enter to continue:> ";
				utils::read_str(std::cin, input, true);
				continue;
			}*/
		}
		if (input == "4")
		{
			utils::clear_screen();

			std::cout << "Bought Travel Packs" << std::endl;
			selected_client.show_travelpacks();

			std::cout << "Press enter to return:> ";
			utils::wait_for_enter();
		}

		continue;
	}
}

void client_menu()
{
	while (true) {
		std::string input;
		utils::clear_screen();
		std::cout << "Client Menu" << std::endl;
		std::cout << "[1]: Display all clients" << std::endl;
		std::cout << "[2]: Create new client" << std::endl;
		std::cout << "[3]: Select a client" << std::endl;
		std::cout << "[4]: Display all travel pack recommendations" << std::endl;
		std::cout << std::endl;
		std::cout << "Please choose an option. If you wish to exit, enter '0' or use ctrl+Z" << std::endl;
		std::cout << ":> ";

		if (!utils::read_str(std::cin, input, true))
		{
			if (input == "EOF")
			{
				break;
			}
			else
			{
				continue;
			}
		}

		if (input == "0") break;
		if (input == "1")
		{
			std::cout << "All Clients" << std::endl;
			Client::print_all();
			std::cout << std::endl;
		}
		if (input == "2")
		{
			Client::new_from_console();
		}
		if (input == "3")
		{
			client_submenu(Client::select_client());
		}
		if (input == "4")
		{
			//////////
		}

		continue;
	}
}

void show_all_clients()
{
	utils::print("All Clients");
	Client::print_all();
}
