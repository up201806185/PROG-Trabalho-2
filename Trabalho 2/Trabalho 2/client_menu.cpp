#include "client_menu.h"
#include "Client.h"

void client_submenu(Client * ptr)
{
	if (ptr == nullptr) return;

	while (true) {
		utils::clear_screen();
		ptr->pprint();

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
			ptr->edit();
		}
		if (input == "2")
		{
			Client::erase(ptr);
			return;
		}
		if (input == "3")
		{
			make_purchase(ptr);

		}
		if (input == "4")
		{
			utils::clear_screen();

			std::cout << "Bought Travel Packs" << std::endl;
			ptr->show_travelpacks();

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
			utils::clear_screen();
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
			show_all_recommendations();
		}

		continue;
	}
}

void show_all_recommendations()
{
	utils::clear_screen();
	
	std::vector<std::pair<std::string, long>> top_packs = Travelpack::get_n_most_visited_vector();

	if (top_packs.size() == 0) return;

	utils::clear_screen();

	std::cout << std::left << std::setw(92) << "Clients" << "Recommendations" << std::endl;
	std::cout << utils::FANCY_DELIMITER << "  " << utils::FANCY_DELIMITER << std::endl;

	std::set<Client*>::iterator i;

	for (i = Client::clients.begin(); i != Client::clients.end(); i++) {
		Client selected_client = **i;

		Travelpack* selected_pack = nullptr;

		for (size_t j = 0; j < top_packs.size(); j++) {
			std::string selected_destination = top_packs.at(j).first;
			selected_destination = utils::uppercase(selected_destination);

			std::pair <std::multimap<std::string, Travelpack*>::iterator, std::multimap<std::string, Travelpack*>::iterator> ret = Travelpack::destination_to_travelpack_map.equal_range(selected_destination);

			for (auto k = ret.first; k != ret.second; k++) {
				Travelpack* temp_p = k->second;
				std::vector<Travelpack*> client_packs = selected_client.get_packs();
				if (std::find(client_packs.begin(), client_packs.end(), temp_p) == client_packs.end() && temp_p->get_available() && (temp_p->get_max_bought_tickets() - temp_p->get_bought_tickets() >= selected_client.get_f_size()))
				{
					//pack mustn't be found in clients' bought packs, has to be available and be enough tickets for the client to buy
					selected_pack = temp_p;
					break;
				}
			}
		}
		if (selected_pack != nullptr) selected_client.side_by_side_print(selected_pack);
		else {
			selected_client.no_recommendation_print();
		}
	}

	std::cout << "\nPress enter to return:> ";
	utils::wait_for_enter();
}

void make_purchase(Client* selected_client)
{
	Travelpack *selected_pack = Travelpack::select_pack();

	std::vector<Travelpack*> client_packs = selected_client->get_packs();

	if (selected_pack == nullptr) return;

	if (!selected_pack->get_available()) {
		utils::print("Pack is not available, cancelling purchase");
	}
	else if (!selected_pack->purchase_n_tickets(selected_client->get_f_size())) {
		utils::print("There aren't enough tickets left, cancelling purchase");
	}
	else if (std::find(client_packs.begin(), client_packs.end(), selected_pack) != client_packs.end()) {
		utils::print("Client already bought this pack, cancelling purchase");
	}
	else {
		selected_client->push_new_pack(Travelpack::get_pointer_from_id(selected_pack->get_id()));
		selected_client->update_total_purchased(selected_pack->get_price_per_person());
		utils::print("Purchase successful");
	}

	std::cout << "\nPress enter to continue:> ";
	utils::wait_for_enter();
	return;
}
