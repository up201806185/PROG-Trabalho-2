#include "Client.h"



const std::string DELIMITER = "::::::::::";
const std::string FANCY_DELIMITER = std::string(55, '=');
std::vector<Client*> Client::clients;

const std::vector<std::string> LABELS =
{
"Client name                        : ",
"NIF                                : ",
"Number of close relatives          : ",
"Address                            : ",
"Travelpacks bought                 : ",
"Maximum number of tickets          : "
};

const std::vector<std::string> EDIT_LABELS =
{
"Client name                        : ",
"NIF                                : ",
"Number of close relatives          : ",
"Address                            : ",
"Packs bought(ex: 1;2;5, 0 if none) : ",
"Maximum number of tickets          : "
};

Client::Client()
{
}


Client::~Client()
{
}

bool Client::save(const std::string & path)
{
	std::ofstream stream(path);
	if (stream.fail())
	{
		stream.close();
		return false;
	}

	for (size_t i = 0; i < clients.size(); i++)
	{
		if (i != 0)
		{
			stream << DELIMITER << std::endl;
		}
		Client * ptr;
		ptr = clients[i];
		stream << *ptr;
	}

	stream.close();
	return true;
}

bool Client::set_error(std::string error_str)
{
	is_valid = false;
	error_message = error_str;
	return is_valid;
}

bool Client::parse_packs_purchased(std::istream & stream)
{
	std::vector<size_t> ids;
	std::string input;
	if (!utils::read_str(std::cin, input))
	{
		if (input == "EOF")
			return set_error("EOF");
		else
			return set_error("Parsing error while trying to parse the ids of the travelpacks purchased");
	}

	if (!utils::parse_vector_of_nums(ids, input, ';'))
		return set_error("Parsing error while trying to parse the ids of the travelpacks purchased");
	
	travelpacks_purchased.resize(0);
	is_valid = true;
	
	for (size_t id : ids)
	{
		if (Travelpack().id_exists(id))
			travelpacks_purchased.push_back( Travelpack().get_pointer_from_id(id) );
		else
		{
			if (is_valid)
			{
				additional_error_info = "These ids don't exist: " + std::to_string(id);
				is_valid = false;
			}
			else
				additional_error_info += ", " + std::to_string(id);
		}
	}

	if (!is_valid)
		return set_error("Reference error");
	else
		return is_valid;
}

void Client::print_packs_purchased(std::ostream & stream) const
{
	for (size_t i = 0; i < travelpacks_purchased.size(); i++)
	{
		if (i != 0)
			stream << "; ";

		stream << travelpacks_purchased[i]->get_id;
	}
	stream << std::endl;
}

void Client::print_packs_purchased(std::ofstream & stream) const
{
	if (travelpacks_purchased.size() == 0)
		stream << 0;
	else
	{
		for (size_t i = 0; i < travelpacks_purchased.size(); i++)
		{
			if (i != 0)
				stream << "; ";

			stream << travelpacks_purchased[i]->get_id;
		}
	}

	stream << std::endl;
}

void Client::print(std::ostream & stream) const
{
	stream << LABELS[0] << name << std::endl;
	stream << LABELS[1] << nif  << std::endl;
	stream << LABELS[2] << f_size << std::endl;
	stream << LABELS[3] << address << std::endl;
	stream << LABELS[4]; print_packs_purchased(stream);
	stream << LABELS[5] << total_purchased << std::endl;
}

void Client::pprint()
{
	std::cout << FANCY_DELIMITER << std::endl;
	print(std::cout);
	std::cout << FANCY_DELIMITER << std::endl;
}

std::ostream & operator<<(std::ostream & stream, const Client & client)
{
	client.print(stream);
	return stream;
}

std::ofstream & operator<<(std::ofstream & stream, const Client & client)
{
	stream << client.name << std::endl;
	stream << client.nif << std::endl;
	stream << client.f_size << std::endl;
	stream << client.address << std::endl;
	client.print_packs_purchased(stream);
	stream << client.total_purchased << std::endl;

	return stream;
}
