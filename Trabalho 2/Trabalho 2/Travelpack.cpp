#include "Travelpack.h"
#include "utils.h"

#include <limits>
#include <string>
#include <vector>

const std::string DELIMITER = "::::::::::";
const std::string FANCY_DELIMITER = std::string(55, '=');
std::map<size_t, Travelpack*> Travelpack::travelpacks;
std::multimap<std::string, Travelpack*> Travelpack::destination_to_travelpack_map;
std::vector< std::pair<std::string, long>> Travelpack::destination_visits_vector;
const std::vector<std::string> LABELS = 
{
"ID                         : ",
"Destinations               : ", 
"Begginning date            : ", 
"Ending date                : ", 
"Price(per person)          : ",
"Maximum number of tickets  : ",
"Purchased tickets          : "
};

const std::vector<std::string> EDIT_LABELS =
{
"ID                         : ",
"Destinations (a - b, c, d) : ",
"Begginning date(YYYY/MM/DD): ",
"Ending date    (YYYY/MM/DD): ",
"Price(per person)          : ",
"Maximum number of tickets  : ",
"Purchased tickets          : "
};

inline bool want_to_exit()
{
	std::string answer = utils::yes_no_prompt("Do you wish to exit? The changes you made won't be saved(y/n):> ");
	if (answer == "NO")
		return false;
	else
		return true;
}

size_t get_fresh_id()
{
	size_t counter = 1;

	while (true)
	{
		if (!Travelpack().id_exists(counter))
			return counter;
		else
			counter++;
	}
}

Travelpack::Travelpack()
{
	set_error("Uninitialized travelpack");
}

Travelpack::~Travelpack()
{
}

void Travelpack::load(const std::string & path)
{
	std::ifstream stream(path);
	if (stream.fail())
	{
		utils::print("An error occurred while trying to load the Travelpacks file: the file with path \"" +
			path + "\" could not be opened");
		exit(1);
	}

	size_t n_travelpacks;
	if (!utils::read_num(stream, n_travelpacks))
	{
		utils::print("An error occurred while trying to read the first line of the Travelpacks file (path: \"" +
			path + '\"');
		exit(1);
	}

	for (size_t i = 0; i < n_travelpacks - 1; i++)
	{
		Travelpack temp_tp;
		if (!temp_tp.parse(stream))
		{
			utils::print("An error occurred while parsing travelpack number " + std::to_string(i + 1));
			utils::print(temp_tp.get_error());
			exit(1);
		}

		
		Travelpack * ptr = new Travelpack;
		*ptr = temp_tp;
		ptr->add_destinations_to_map();
		travelpacks.insert(std::pair<size_t, Travelpack *>(temp_tp.id, ptr));

		std::string temp;
		if (!utils::read_str(stream, temp))
		{
			if (temp == "EOF")
			{
				utils::print("An unexpected EOF occurred after travelpack number " + std::to_string(i + 1));
				utils::print(std::to_string(n_travelpacks - i - 1) + " more travelpacks were expected");
				exit(1);
			}
			else
			{
				utils::print("An unexpected parsing error occurred after travelpack number " + std::to_string(i + 1));
				exit(1);
			}
		}

		if (temp != DELIMITER)
		{
			utils::print('\"' + DELIMITER + "\"was expected after travelpack number " + std::to_string(i + 1));
			utils::print("Instead, \"" + temp + "\" was found");
			exit(1);
		}
	}

	Travelpack temp_tp;
	if (!temp_tp.parse(stream))
	{
		utils::print("An error occurred while parsing travelpack number " + std::to_string(n_travelpacks));
		utils::print(temp_tp.get_error());
		exit(1);
	}

	Travelpack * ptr = new Travelpack;
	*ptr = temp_tp;
	ptr->add_destinations_to_map();
	travelpacks.insert(std::pair<size_t, Travelpack *>(temp_tp.id, ptr));

	stream.close();
	return;
}

bool Travelpack::save(const std::string & path)
{
	std::ofstream stream(path);
	if (stream.fail())
	{
		stream.close();
		return false;
	}

	utils::print(travelpacks.size(), stream);

	std::map<size_t, Travelpack*>::iterator it;

	for (it = travelpacks.begin(); it != travelpacks.end(); it++) {
		if (it != travelpacks.begin())
			utils::print(DELIMITER, stream);

		Travelpack temp = *it->second;
		stream << temp;
	}

	stream.close();
	return true;
}

bool Travelpack::id_exists(size_t id)
{
	return (id != 0 && travelpacks.find(id) != travelpacks.end());
}

Travelpack * Travelpack::get_pointer_from_id(size_t id)
{
	if (!id_exists(id))
	{
		std::cout << "You are trying to access an ID that doesn't exist" << std::endl;
		std::cout << "ID: " << id << std::endl;
		exit(1);
	}
	else
		return travelpacks[id];
}

bool Travelpack::is_between_dates(Date start, Date end) const
{
	return (this->begginning >= start) && (this->end <= end);
}

void Travelpack::new_from_console()
{
	Travelpack new_tp;
	new_tp.id = get_fresh_id();
	const bool CHANGE_EVERYTHING[] = { false, false, false, false, false, false, false };
	if (new_tp.granular_edit(CHANGE_EVERYTHING, false))
	{
		Travelpack * ptr = new Travelpack;
		*ptr = new_tp;
		ptr->add_destinations_to_map();
		travelpacks.insert(std::pair<size_t, Travelpack *>(new_tp.id, ptr));
	}

	return;
}

void Travelpack::edit()
{
	utils::clear_screen();

	std::cout << FANCY_DELIMITER << std::endl;
	std::cout << "[-] " << LABELS[0] << id << std::endl;
	if (available)
		utils::print("[1] Travelpack                   is available");
	else
		utils::print("[1] Travelpack                   is not available");
	std::cout << "[2] " << LABELS[1]; print_destinations(std::cout);
	std::cout << "[3] " << LABELS[2] << begginning << std::endl;
	std::cout << "[4] " << LABELS[3] << end << std::endl;
	std::cout << "[5] " << LABELS[4] << price_per_person << std::endl;
	std::cout << "[6] " << LABELS[5] << max_bought_tickets << std::endl;
	std::cout << "[7] " << LABELS[6] << bought_tickets << std::endl;
	std::cout << FANCY_DELIMITER << std::endl;
	std::cout << std::endl << std::endl;

	while (true)
	{
		std::cout << "Please enter the numbers of the properties you wish to change (ex: 1,2,4) :> ";
		std::vector<size_t> choices;
		std::string input;
		if (!utils::read_str(std::cin, input))
		{
			if (input == "EOF")
			{
				std::string answer = utils::yes_no_prompt("Do you wish to exit? (Y/n):> ", "YES");
				if (answer == "YES")
					return;
			}
			continue;
		}

		if (!utils::parse_vector_of_nums(choices, input, ','))
			continue;

		bool out_of_bounds = false;
		for (size_t i : choices)
		{
			if (i == 0 || i >= 8)
			{
				out_of_bounds = true;
				break;
			}
		}

		if (out_of_bounds)
		{
			utils::print("Error, numbers must be between 1 and 7, please try again");
			continue;
		}

		bool what_to_change[] = { true, true, true, true, true, true, true };
		for (size_t i : choices)
		{
			what_to_change[i - 1] = false;
		}

		std::cout << "Press Enter to enter editing mode :> ";
		utils::wait_for_enter();

		remove_destinations_from_map();
		granular_edit(what_to_change, true);
		add_destinations_to_map();
		return;

	}
}

Travelpack * Travelpack::select_pack()
{
	utils::clear_screen();

	std::vector <Travelpack*> refs = select_pack_vector();

	utils::clear_screen();

	if (refs.size() == 0) {
		utils::print("No packs found");
		std::cout << "Press enter to return:> ";
		utils::wait_for_enter();
		return nullptr;
	}

	for (size_t i = 0; i < refs.size(); i++) {
		Travelpack temp = *refs.at(i);

		std::cout << "[" << i + 1 << "]" << std::endl;
		temp.pprint();
		std::cout << std::endl;
	}

	std::cout << "Please choose a travel pack:>";
	size_t choice;
	if (!utils::read_num(std::cin, choice)) {

		if (choice == std::numeric_limits<size_t>::max()) return nullptr;
	}

	return refs.at(choice - 1);
}

std::vector<Travelpack*> Travelpack::select_pack_vector()
{
	bool date, destination;
	Date begin, end;
	std::string input, main_destination;
	size_t packs_found = 0;
	std::vector<Travelpack*> filtered_packs;

	std::cout << "Do you wish to filter by starting and ending date?(Y/n):> ";
	utils::read_str(std::cin, input, true);
	if (input == "N" || input == "n")
	{
		date = false;
	}
	else
	{
		date = true;
		while (true) 
		{
			while (true)
			{
				std::cout << "Starting date (YYYY/MM/DD):> ";
				if (begin.parse(std::cin))
				{
					break;
				}
			}

			while (true)
			{
				std::cout << "Ending date (YYYY/MM/DD):> ";
				if (end.parse(std::cin))
				{
					break;
				}
			}
			if (begin > end) utils::print("Incoherency error: The beggining date comes later than the finish date");
			else break;
		}
	}

	std::cout << "Do you wish to filter by main destination?(Y/n):> ";
	utils::read_str(std::cin, input, true);
	if (input == "N" || input == "n")
	{
		destination = false;
	}
	else
	{
		destination = true;

		while (true)
		{
			std::cout << "Main destination:> ";
			if (utils::read_str(std::cin, main_destination))
			{
				break;
			}
		}
	}
	
	if (date && destination) return fetch_by_date_and_destination(begin, end, main_destination);
	if (date) return fetch_by_date(begin, end);
	if (destination) return fetch_by_destination(main_destination);
	if (!date && !destination) return fetch_all();
}

void Travelpack::get_destination_visits_vector()
{
	destination_visits_vector.clear();
	std::map<size_t, Travelpack*>::iterator i;

	for (i = travelpacks.begin(); i != travelpacks.end(); i++) {
		Travelpack temp = *i->second;
		std::vector <std::string> dests = temp.destinations;
		
		for (size_t j = 0; j < dests.size(); j++) {
			auto it = std::find_if(destination_visits_vector.begin(), destination_visits_vector.end(), [&dests, &j](const std::pair<std::string, double>& element) { return element.first == dests.at(j); });

			if (it != destination_visits_vector.end()) it->second += temp.bought_tickets; //if destination is already in vector, add this packs' bought tickets

			else destination_visits_vector.push_back(std::pair<std::string, long> (dests.at(j), temp.bought_tickets)); //create entry in vector for this destination
		}
	}
	sort(destination_visits_vector.begin(), destination_visits_vector.end(), utils::sortbysec);
}

std::vector<std::pair<std::string, long>> Travelpack::get_n_most_visited_vector()
{
	get_destination_visits_vector();

	std::vector<std::pair<std::string, long>> result;
	
	utils::print("What is the length (N) of the list? (Type 0 or CTRL + Z to exit)");
	std::size_t input;
	while (true)
	{
		if (!utils::read_num(std::cin, input))
		{
			if (input == std::numeric_limits<size_t>::max())
			{
				return {};
			}
			else
				continue;
		}
		else
			break;
	}

	if (input == 0) return {};

	if (input > Travelpack::destination_visits_vector.size()) input = Travelpack::destination_visits_vector.size();

	result = destination_visits_vector;
	result.resize(input);

	return result;
}

std::vector<Travelpack*> Travelpack::fetch_by_date(const Date start, const Date end)
{
	std::vector<Travelpack*> filtered_packs;

	std::map<size_t, Travelpack*>::iterator it;

	for (it = travelpacks.begin(); it != travelpacks.end(); it++) {
		Travelpack temp = *it->second;
		if (temp.is_between_dates(start, end)) filtered_packs.push_back(it->second);
	}

	return filtered_packs;
}

std::vector<Travelpack*> Travelpack::fetch_by_date(const Date start, const Date end, const std::vector<Travelpack*>& packs)
{
	std::vector<Travelpack*> filtered_packs;

	for (Travelpack * i : packs) {
		Travelpack temp = *i;
		if(temp.is_between_dates(start, end))
			filtered_packs.push_back(i);
	}
		
	return filtered_packs;
}

std::vector<Travelpack*> Travelpack::fetch_by_destination(std::string dest)
{
	dest = utils::uppercase(dest);

	std::vector<Travelpack*> filtered_packs;

	std::pair <std::multimap<std::string, Travelpack*>::iterator, std::multimap<std::string, Travelpack*>::iterator> ret;
	ret = destination_to_travelpack_map.equal_range(dest);

	for (std::multimap<std::string, Travelpack*>::iterator it = ret.first; it != ret.second; it++) {
		filtered_packs.push_back(it->second);
	}

	return filtered_packs;
}

std::vector<Travelpack*> Travelpack::fetch_by_date_and_destination(const Date start, const Date end, std::string dest)
{
	std::vector<Travelpack*> filtered_packs = fetch_by_destination(dest);
	return fetch_by_date(start, end, filtered_packs);
}

std::vector<Travelpack*> Travelpack::fetch_all()
{
	std::vector<Travelpack*> filtered_packs;

	std::map<size_t, Travelpack*>::iterator it;

	for (it = travelpacks.begin(); it != travelpacks.end(); it++) {
		Travelpack temp = *it->second;
		filtered_packs.push_back(it->second);
	}
	
	return filtered_packs;
}

void Travelpack::mark_as_unavailable()
{
	utils::clear_screen();
	available = false;
	utils::print("The travelpack has been set as unavailable, press Enter to continue:> ");
	utils::wait_for_enter();
	return;
}

size_t Travelpack::get_bought_tickets() const
{
	return bought_tickets;
}

bool Travelpack::valid() const
{
	return is_valid;
}

std::string Travelpack::get_error() const
{
	return error_message;
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

	if (price_per_person < 0)
	{
		return set_error("Incoherency error: The price can not be negative");
	}

	return is_valid = true;
}

bool Travelpack::set_error(std::string error_str)
{
	is_valid = false;
	error_message = error_str;
	return is_valid;
}

bool Travelpack::parse(std::ifstream & stream)
{
	//Parse ID and availability
	int id_plus_availability;
	if (!utils::read_num(stream, id_plus_availability))
	{
		if (id_plus_availability == std::numeric_limits<int>::max())
			return set_error("EOF");
		else
			return set_error("Parsing error while trying to parse the ID");
	}

	if (id_plus_availability < 0)
	{
		available = false;
		id = -id_plus_availability;
	}
	else
	{
		available = true;
		id = id_plus_availability;
	}

	if (id_exists(id))
	{
		return set_error("ID already in use: two travelpacks with the same id (" + std::to_string(id) + ") were found");
	}

	//Destinations
	if (!parse_destinations(stream))
		return false;

	//Beggining date
	if (!begginning.parse(stream))
	{
		if (begginning.get_error() == "EOF")
			return set_error("EOF");
		else
			return set_error("Parsing error while trying to parse the begginning date");
	}

	//End date
	if (!end.parse(stream))
	{
		if (end.get_error() == "EOF")
			return set_error("EOF");
		else
			return set_error("Parsing error while trying to parse the ending date");
	}

	//Price
	if (!utils::read_num(stream, price_per_person))
	{
		if (id_plus_availability == std::numeric_limits<double>::max())
			return set_error("EOF");
		else
			return set_error("Parsing error while trying to parse the Price");
	}

	//Maximum bought tickets
	if (!utils::read_num(stream, max_bought_tickets))
	{
		if (id_plus_availability == std::numeric_limits<size_t>::max())
			return set_error("EOF");
		else
			return set_error("Parsing error while trying to parse the maximum number of tickets");
	}

	//Bought tickets
	if (!utils::read_num(stream, bought_tickets))
	{
		if (id_plus_availability == std::numeric_limits<size_t>::max())
			return set_error("EOF");
		else
			return set_error("Parsing error while trying to parse the number of purchased tickets");
	}

	//Check the coherency
	return check_coherence();
}

bool Travelpack::parse_destinations(std::istream & stream)
{
	std::string temp;
	if (!utils::read_str(stream, temp))
	{
		if (temp == "EOF")
			return set_error("EOF");
		else
			return set_error("Parsing error while trying to parse the destinations");
	}

	std::vector<std::string> parts = utils::split(temp, '-', 1);
	destinations.push_back(parts[0]);

	if (parts.size() == 2)
	{
		std::vector<std::string> sec_destinations = utils::split(parts[1], ',');
		destinations.insert(std::end(destinations), std::begin(sec_destinations), std::end(sec_destinations));
	}

	return true;
}

void Travelpack::print_destinations(std::ostream & stream) const
{
	stream << destinations[0];
	if (destinations.size() > 1)
	{
		stream << " - " << utils::join(destinations, " , ", 1);
	}
	stream << std::endl;
	return;
}

bool Travelpack::granular_edit(const bool keep_info[], bool edit_mode)
{
	utils::clear_screen();
	Travelpack new_travelpack;

	if (edit_mode)
	{
		std::cout << "Information of travelpack to be edited:" << std::endl;
		pprint();
		std::cout << std::endl << std::endl;
	}


	utils::print(FANCY_DELIMITER);
	//Travelpack ID
	std::cout << LABELS[0] << id << std::endl;
	new_travelpack.id = id;
	
	//Is the pack available?
	if (keep_info[0])
	{
		if (available)
			utils::print("Pack is available");
		else
			utils::print("Pack is not available");
		new_travelpack.available = available;
	}
	else
	{
		while (true)
		{
			std::string answer = utils::yes_no_prompt("Is the pack available?(Y/n): ", "YES");
			if (answer == "EOF")
			{
				if (want_to_exit())
					return false;
				else
					continue;
			}
			if (answer == "YES")
				new_travelpack.available = true;
			else if (answer == "NO")
				new_travelpack.available = false;
			break;
		}
	}

	//Destinations
	if (keep_info[1] == true)
	{
		std::cout << LABELS[1]; print_destinations(std::cout);
		new_travelpack.destinations = destinations;
	}
	else
	{
		while (true)
		{
			std::cout << EDIT_LABELS[1];
			if (new_travelpack.parse_destinations(std::cin))
			{
				if (new_travelpack.get_destinations_str.length() > 55)
				{
					utils::print("Destinations vector is too large. Please use abreviations if possible");
					continue;
				}
				else break;
			}
			
			if (new_travelpack.error_message == "EOF")
			{
				if (want_to_exit())
					return false;
				else
					continue;
			}
		}
	}

	//Beggining date
	if (keep_info[2] == true)
	{
		std::cout << LABELS[2] << begginning << std::endl;
		new_travelpack.begginning = begginning;
	}
	else
	{
		while (true)
		{
			std::cout << EDIT_LABELS[2];
			if (new_travelpack.begginning.parse(std::cin))
				break;

			if (new_travelpack.begginning.get_error() == "EOF")
			{
				if (want_to_exit())
					return false;
				else
					continue;
			}
		}
	}

	//Ending date
	if (keep_info[3] == true)
	{
		std::cout << LABELS[3] << end << std::endl;
		new_travelpack.end = end;
	}
	else
	{
		while (true)
		{
			std::cout << EDIT_LABELS[3];
			if (new_travelpack.end.parse(std::cin))
				break;

			if (new_travelpack.end.get_error() == "EOF")
			{
				if (want_to_exit())
					return false;
				else
					continue;
			}
		}
	}

	//Price per person
	if (keep_info[4])
	{
		std::cout << LABELS[4] << price_per_person << std::endl;
		new_travelpack.price_per_person = price_per_person;
	}
	else
	{
		while (true)
		{
			std::cout << EDIT_LABELS[4];
			if (utils::read_num(std::cin, new_travelpack.price_per_person))
				break;

			if (new_travelpack.price_per_person == std::numeric_limits<double>::max())
			{
				if (want_to_exit())
					return false;
				else
					continue;
			}
		}
	}

	//Maximum bought tickets
	if (keep_info[5])
	{
		std::cout << LABELS[5] << max_bought_tickets << std::endl;
		new_travelpack.max_bought_tickets = max_bought_tickets;
	}
	else
	{
		while (true)
		{
			std::cout << EDIT_LABELS[5];
			if (utils::read_num(std::cin, new_travelpack.max_bought_tickets))
				break;

			if (new_travelpack.max_bought_tickets == std::numeric_limits<size_t>::max())
			{
				if (want_to_exit())
					return false;
				else
					continue;
			}
		}
	}

	//Bought tickets
	if (keep_info[6])
	{
		std::cout << LABELS[6] << bought_tickets << std::endl;
		new_travelpack.bought_tickets = bought_tickets;
	}
	else
	{
		while (true)
		{
			std::cout << EDIT_LABELS[6];
			if (utils::read_num(std::cin, new_travelpack.bought_tickets))
				break;

			if (new_travelpack.bought_tickets == std::numeric_limits<size_t>::max())
			{
				if (want_to_exit())
					return false;
				else
					continue;
			}
		}
	}
	utils::print(FANCY_DELIMITER);

	//Display "before" travelpack and "after" travelpack
	utils::clear_screen();

	if (edit_mode)
	{
		std::cout << "Travelpack information BEFORE edit:" << std::endl;
		pprint();
		std::cout << std::endl;

		std::cout << "Travelpack information AFTER edit:" << std::endl;
		new_travelpack.pprint();
		std::cout << std::endl;
	}
	else
	{
		std::cout << "Travelpack information:" << std::endl;
		new_travelpack.pprint();
		std::cout << std::endl;
	}

	//Check the coherence
	std::cout << "Please press Enter to check for coherence errors:> ";
	utils::wait_for_enter();
	
	while (!new_travelpack.check_coherence())
	{
		utils::clear_screen();
		utils::print("A coherency error was detected:");
		utils::print(new_travelpack.error_message);

		const bool CHANGE_DATES[] =        { true, true, false, false, true, true, true};
		const bool CHANGE_MAX_TICKETS[] =  { true, true, true, true, true, false, true };
		const bool CHANGE_TICKET_PRICE[] = { true, true, true, true, false, true, true };

		if (new_travelpack.error_message == "Incoherency error: The beggining date comes later than the finish date")
		{
			std::cout << "Please change the dates, press Enter to continue:> ";
			utils::wait_for_enter();
			if (new_travelpack.granular_edit(CHANGE_DATES, true))
			{
				load_state(new_travelpack);
				return true;
			}
			else
				return false;
		}

		if (new_travelpack.error_message == "Incoherency error: The begginning date comes sooner than today, yet the travelpack is set as available")
		{
			while (true)
			{
				utils::print("What do you wish to do?");
				utils::print("Enter '1' if you wish to set the travelpack as unavailable");
				utils::print("Enter '2' if you wish to change the dates");
				std::cout << ":> ";
				std::string answer;
				if (!utils::read_str(std::cin, answer))
				{
					if (answer == "EOF")
					{
						if (want_to_exit())
							return false;
					}
					continue;
				}
				
				if (answer == "1")
				{
					new_travelpack.available = false;
					std::cout << "The travelpack has been set as unavailable, press Enter to continue:> ";
					utils::wait_for_enter();
					break;
				}
				if (answer == "2")
				{
					std::cout << "Entering editing mode, press Enter to continue:> ";
					utils::wait_for_enter();
					if (new_travelpack.granular_edit(CHANGE_DATES, true))
					{
						load_state(new_travelpack);
						return true;
					}
					else
						return false;
				}
			}
		}

		if (new_travelpack.error_message == "Incoherency error: The travelpack is sold out, yet the travelpack is set as available")
		{
			while (true)
			{
				utils::print("What do you wish to do?");
				utils::print("Enter '1' if you wish to set the travelpack as unavailable");
				utils::print("Enter '2' if you wish to change maximum number of tickets");
				std::cout << ":> ";
				std::string answer;
				if (!utils::read_str(std::cin, answer))
				{
					if (answer == "EOF")
					{
						if (want_to_exit())
							return false;
					}
					continue;
				}

				if (answer == "1")
				{
					new_travelpack.available = false;
					std::cout << "The travelpack has been set as unavailable, press Enter to continue:> ";
					utils::wait_for_enter();
					break;
				}
				if (answer == "2")
				{
					std::cout << "Entering editing mode, press Enter to continue:> ";
					utils::wait_for_enter();
					if (new_travelpack.granular_edit(CHANGE_MAX_TICKETS, true))
					{
						load_state(new_travelpack);
						return true;
					}
					else
						return false;
				}
			}
		}

		if (new_travelpack.error_message == "Incoherency error: The price can not be negative")
		{
			std::cout << "Please change the price, press Enter to continue:> ";
			utils::wait_for_enter();
			if (new_travelpack.granular_edit(CHANGE_TICKET_PRICE, true))
			{
				load_state(new_travelpack);
				return true;
			}
			else
				return false;
		}
	}

	utils::clear_screen();
	utils::print("No errors were detected, press enter to continue:> ");
	utils::wait_for_enter();

	//Do you wish to save?
	std::string answer = utils::yes_no_prompt("Do you wish to save(Y/n)? :> ", "YES");
	if (answer == "YES")
	{
		load_state(new_travelpack);
		return true;
	}
	else
	{
		std::cout << "Please press Enter to exit editing mode:> ";
		utils::wait_for_enter();
		return false;
	}
}



void Travelpack::pprint()
{
	std::cout << FANCY_DELIMITER << std::endl;
	print(std::cout);
	std::cout << FANCY_DELIMITER << std::endl;
}

void Travelpack::central_pprint()
{
	std::cout << "\t\t" << FANCY_DELIMITER << std::endl;
	central_print(std::cout);
	std::cout << "\t\t" << FANCY_DELIMITER << std::endl;
}

void Travelpack::load_state(const Travelpack & donor)
{
	id                 = donor.id;
	available          = donor.available;
	destinations       = donor.destinations;
	begginning         = donor.begginning;
	end                = donor.end;
	price_per_person   = donor.price_per_person;
	max_bought_tickets = donor.max_bought_tickets;
	bought_tickets     = donor.bought_tickets;
	is_valid           = donor.is_valid;
	error_message      = donor.error_message;
}

void Travelpack::remove_destinations_from_map()
{
	for (std::multimap<std::string, Travelpack*>::iterator i : map_iterators)
	{
		destination_to_travelpack_map.erase(i);
	}
	map_iterators.resize(0);
}

void Travelpack::add_destinations_to_map()
{
	map_iterators.resize(0);
	for (const std::string & i : destinations)
	{
		map_iterators.push_back(destination_to_travelpack_map.insert(std::pair<std::string, Travelpack*>(utils::uppercase(i), this)));
	}
	return;
}

void Travelpack::print_all()
{
	std::map<size_t, Travelpack*>::iterator it;

	utils::print(FANCY_DELIMITER);
	for (it = travelpacks.begin(); it != travelpacks.end(); it++) {
		Travelpack temp = *it->second;
		std::cout << temp;
		utils::print(FANCY_DELIMITER);
	}
}

void Travelpack::print(std::ostream & stream) const
{
	stream << LABELS[0] << id << std::endl;
	if (available)
		utils::print("Travelpack                 : is available", stream);
	else
		utils::print("Travelpack                 : is not available", stream);
	stream << LABELS[1]; print_destinations(stream);
	stream << LABELS[2] << begginning << std::endl;
	stream << LABELS[3] << end << std::endl;
	stream << LABELS[4] << price_per_person << std::endl;
	stream << LABELS[5] << max_bought_tickets << std::endl;
	stream << LABELS[6] << bought_tickets << std::endl;
}

void Travelpack::central_print(std::ostream & stream) const
{
	stream << "\t\t\t" << LABELS[0] << id << std::endl;
	if (available)
		utils::print("\t\t\tTravelpack                 : is available", stream);
	else
		utils::print("\t\t\tTravelpack                 : is not available", stream);
	stream << "\t\t\t" << LABELS[1]; print_destinations(stream);
	stream << "\t\t\t" << LABELS[2] << begginning << std::endl;
	stream << "\t\t\t" << LABELS[3] << end << std::endl;
	stream << "\t\t\t" << LABELS[4] << price_per_person << std::endl;
	stream << "\t\t\t" << LABELS[5] << max_bought_tickets << std::endl;
	stream << "\t\t\t" << LABELS[6] << bought_tickets << std::endl;
}

std::ostream & operator<<(std::ostream & stream, const Travelpack & travelpack)
{
	travelpack.print(stream);
	return stream;
}

std::ofstream & operator<<(std::ofstream & stream, const Travelpack & travelpack)
{
	if (!travelpack.available)
		stream << '-';
	stream << travelpack.id                 << std::endl;
	travelpack.print_destinations(stream);
	stream << travelpack.begginning         << std::endl;
	stream << travelpack.end                << std::endl;
	stream << travelpack.price_per_person   << std::endl;
	stream << travelpack.max_bought_tickets << std::endl;
	stream << travelpack.bought_tickets     << std::endl;

	return stream;
}

size_t Travelpack::get_id() const
{
	return id;
}

bool Travelpack::get_available() const
{
	return available;
}

std::vector<std::string> Travelpack::get_destinations() const
{
	return destinations;
}

std::string Travelpack::get_destinations_str() const
{
	std::string result;

	result = destinations.at(0) + " - ";
	for (size_t i = 1; i < destinations.size() - 1; i++) {
		result += destinations.at(i) + " , ";
	}
	result += destinations.at(destinations.size() - 1);
	
	return result;
}

Date Travelpack::get_begginning() const
{
	return begginning;
}

Date Travelpack::get_end() const
{
	return end;
}

double Travelpack::get_price_per_person() const
{
	return price_per_person;
}

size_t Travelpack::get_max_bought_tickets() const
{
	return max_bought_tickets;
}
