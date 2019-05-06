#include "Travelpack.h"
#include "utils.h"

#include <limits>
#include <string>
#include <vector>

const std::string DELIMITER = "::::::::::";
std::vector<Travelpack*> Travelpack::travelpacks;
const std::vector<std::string> LABELS = 
{"ID : ",
"Destination : ", 
"Begginning date : ", 
"Ending date : ", 
"Price(per person) : ",
"Maximum number of tickets : ",
"Purchased tickets : "};

Travelpack::Travelpack()
{
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

size_t Travelpack::get_bought_tickets()
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

std::ostream & operator<<(std::ostream & stream, const Travelpack & travelpack)
{
	stream << LABELS[0] << travelpack.id                 << std::endl;
	stream << LABELS[1]; travelpack.print_destinations(stream);
	stream << LABELS[2] << travelpack.begginning         << std::endl;
	stream << LABELS[3] << travelpack.end                << std::endl;
	stream << LABELS[4] << travelpack.price_per_person   << std::endl;
	stream << LABELS[5] << travelpack.max_bought_tickets << std::endl;
	stream << LABELS[6] << travelpack.bought_tickets     << std::endl;

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

size_t Travelpack::get_id()
{
	return id;
}

bool Travelpack::get_available()
{
	return available;
}

std::vector<std::string> Travelpack::get_destinations()
{
	return destinations;
}

Date Travelpack::get_begginning()
{
	return begginning;
}

Date Travelpack::get_end()
{
	return end;
}

double Travelpack::get_price_per_person()
{
	return price_per_person;
}

size_t Travelpack::get_max_bought_tickets()
{
	return max_bought_tickets;
}
