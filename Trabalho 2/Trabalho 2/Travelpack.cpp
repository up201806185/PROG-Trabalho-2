#include "Travelpack.h"
#include "utils.h"

#include <limits>
#include <string>
#include <vector>

const std::string DELIMITER = "::::::::::";
const std::string FANCY_DELIMITER = std::string(55, '=');
std::vector<Travelpack*> Travelpack::travelpacks;
const std::vector<std::string> LABELS = 
{"ID : ",
"Destinations : ", 
"Begginning date : ", 
"Ending date : ", 
"Price(per person) : ",
"Maximum number of tickets : ",
"Purchased tickets : "};

bool want_to_exit()
{
	std::string answer = utils::yes_no_prompt("Do you wish to exit? The changes you made won't be saved(y/n):> ");
	if (answer == "NO")
		return false;
	else
		return true;
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
		travelpacks.push_back(ptr);

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
	travelpacks.push_back(ptr);

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

	for (size_t i = 0; i < travelpacks.size(); i++)
	{
		if (i != 0)
		{
			stream << DELIMITER << std::endl;
		}
		Travelpack * ptr;
		ptr = travelpacks[i];
		stream << *ptr;
	}

	stream.close();
	return true;
}

void Travelpack::new_from_console()
{
	Travelpack new_tp;
	new_tp.id = travelpacks.size() + 1;
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

	if (id != travelpacks.size() + 1)
	{
		return set_error("Incorrect ID: an ID of " + std::to_string(id) + 
			" was read, but an ID of " + std::to_string(travelpacks.size() + 1) + 
			" was expected");
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
		std::cout << "Information of travel pack to be edited:" << std::endl;
		pprint();
	}
	std::cout << std::endl;

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
			std::string answer = utils::yes_no_prompt("Is the pack available? (Y/n):> ", "YES");
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
			std::cout << LABELS[1] << " (a-b,c,d) ";
			if (new_travelpack.parse_destinations(std::cin))
				break;
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
			std::cout << LABELS[2] << " (YYYY/MM/DD) ";
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
			std::cout << LABELS[3] << " (YYYY/MM/DD) ";
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
			std::cout << LABELS[4];
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
			std::cout << LABELS[5];
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
			std::cout << LABELS[6];
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

	//TODO:line 537 from Trabalho 1
}



void Travelpack::pprint(bool with_delimiters)
{
	std::cout << FANCY_DELIMITER << std::endl;
	print(std::cout);
	std::cout << FANCY_DELIMITER << std::endl;
}

void Travelpack::print(std::ostream & stream) const
{
	stream << LABELS[0] << id << std::endl;
	if (available)
		utils::print("Travelpack is available", stream);
	else
		utils::print("Travelpack is not available", stream);
	stream << LABELS[1]; print_destinations(stream);
	stream << LABELS[2] << begginning << std::endl;
	stream << LABELS[3] << end << std::endl;
	stream << LABELS[4] << price_per_person << std::endl;
	stream << LABELS[5] << max_bought_tickets << std::endl;
	stream << LABELS[6] << bought_tickets << std::endl;
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
