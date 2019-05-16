#include"Client.h"
#include "pack_menu.h"
#include "utils.h"
#include "Travelpack.h"


void pack_menu()
{
	while (true)
	{
		std::string input;
		utils::clear_screen();
		std::cout << "Travelpack Menu" << std::endl;
		std::cout << "[1]: Display all travelpacks" << std::endl;
		std::cout << "[2]: Create new travelpack" << std::endl;
		std::cout << "[3]: Select a travelpack" << std::endl;
		std::cout << "[4]: Show total profits" << std::endl;
		std::cout << "[5]: Show N most visited places" << std::endl;
		std::cout << "[6]: Show all sold packs" << std::endl;
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
		if (input == "1") show_all_packs();
		if (input == "2") Travelpack::new_from_console();
		if (input == "3") pack_submenu(Travelpack::select_pack());
		if (input == "4") show_profits();
		if (input == "5") show_n_most_visited();
		if (input == "6") show_sold_packs();

		continue;

	}
}

void pack_submenu(Travelpack *ptr)
{
	if (ptr == nullptr) return;

	while (true) {
		Travelpack selected_pack = *ptr;

		utils::clear_screen();
		selected_pack.pprint();

		std::cout << "Options" << std::endl;
		std::cout << "[1]: Edit pack" << std::endl;
		std::cout << "[2]: Mark as unavailable" << std::endl;
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
		if (input == "1") selected_pack.edit();
		if (input == "2") selected_pack.mark_as_unavailable();

		continue;
	}
}

void show_all_packs()
{
	utils::clear_screen();
	utils::print("All Travel Packs");

	Travelpack::print_all();

	std::cout << "Press enter to return:> ";
	utils::wait_for_enter();
}

void show_profits()
{
	utils::clear_screen();

	utils::print("Total profits");

	double total_profit = 0, total_sales = 0;

	std::map<size_t, Travelpack*>::iterator it;

	for (it = Travelpack::travelpacks.begin(); it != Travelpack::travelpacks.end(); it++) {
		Travelpack temp = *it->second;
		std::cout << "Pack #" << temp.get_id() << ": ";
		std::cout << "Sold " << temp.get_bought_tickets() << " at " << temp.get_price_per_person() << "$" << std::endl;
		total_sales += temp.get_price_per_person();
		total_profit += (temp.get_price_per_person()  * temp.get_bought_tickets());
	}
	std::cout << "\nIn total:" << std::endl;
	std::cout << total_sales << " packs sold" << std::endl;
	std::cout << total_profit << "$ of profit" << std::endl << std::endl;

	std::cout << "Press enter to return:> ";
	utils::wait_for_enter();
}

void show_n_most_visited()
{
	utils::clear_screen();

	std::vector<std::pair<std::string, long>> places = Travelpack::get_n_most_visited_vector();

	utils::clear_screen();

	utils::print("Most visited places");

	size_t i = 0;
	std::vector<std::pair<std::string, long>>::iterator it;

	for (it = places.begin(); it != places.end(); it++) {
		std::cout << "[" << i + 1 << "]: " << it->first << ": " << it->second << std::endl << std::endl;
		i++;
	}

	std::cout << "Press enter to return:> ";
	utils::wait_for_enter();
}

void show_sold_packs()
{
	utils::clear_screen();
	
	utils::print("All sold packs' clients");

	std::map<size_t, Travelpack*>::iterator it;

	for (it = Travelpack::travelpacks.begin(); it != Travelpack::travelpacks.end(); ++it) {
		Travelpack temp_p = *it->second;
		temp_p.central_pprint();

		std::set<Client*>::iterator jt;
		for (jt = Client::clients.begin(); jt != Client::clients.end(); ++jt) {
			Client temp_c = **jt;
			std::vector<Travelpack*> temp_c_packs = temp_c.get_packs();
			if (std::find(temp_c_packs.begin(), temp_c_packs.end(), it->second) != temp_c_packs.end())
			{
				temp_c.pprint();
			}
		}
	}

	std::cout << "\nPress enter to return:> ";
	utils::wait_for_enter();
}
