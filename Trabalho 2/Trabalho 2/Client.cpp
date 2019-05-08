#include "Client.h"



const std::string DELIMITER = "::::::::::";
const std::string FANCY_DELIMITER = std::string(55, '=');
std::set<Client*> Client::clients;

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

inline bool want_to_exit()
{
	std::string answer = utils::yes_no_prompt("Do you wish to exit? The changes you made won't be saved(y/n):> ");
	if (answer == "NO")
		return false;
	else
		return true;
}

Client::Client()
{
}


Client::~Client()
{
}

void Client::load(const std::string & path)
{
	std::ifstream stream(path);
	if (stream.fail())
	{
		utils::print("An error occurred while trying to load the Clients file: the file with path \"" +
			path + "\" could not be opened");
		exit(1);
	}

	for (size_t i = 0;; i++)
	{
		Client temp_client;
		if (!temp_client.parse(stream))
		{
			if (temp_client.error_message == "EOF" && temp_client.additional_error_info == "Good EOF, ditch this object")
				return;
			
			std::cout << "An error occurred while parsing client number #" + std::to_string(i + 1);
			if (temp_client.error_message != "Parsing error while trying to parse the name of a client")
				std::cout << ", called " << temp_client.name;
			std::cout << std::endl;

			std::cout << "Error: " << temp_client.error_message;
			if (temp_client.error_message == "Reference error")
				std::cout << ": " << temp_client.additional_error_info;
			std::cout << std::endl;
			exit(1);
		}

		Client * ptr = new Client;
		*ptr = temp_client;
		clients.insert(ptr);

		std::string temp;
		if (!utils::read_str(stream, temp))
		{
			if (temp == "EOF")//Reached the end;
			{
				return;
			}
			else
			{
				utils::print("An unexpected parsing error occurred after client with name " + temp_client.get_name());
				exit(1);
			}
		}

		if (temp != DELIMITER)
		{
			utils::print('\"' + DELIMITER + "\"was expected after client with name " + temp_client.get_name());
			utils::print("Instead, \"" + temp + "\" was found");
			exit(1);
		}
	}
	return;
}

bool Client::save(const std::string & path)
{
	std::ofstream stream(path);
	if (stream.fail())
	{
		stream.close();
		return false;
	}

	bool first_time = true;
	for (Client * client : clients)
	{
		if (first_time)
		{
			stream << *client;
			first_time = !first_time;
			continue;
		}
		stream << DELIMITER << std::endl;
		stream << *client;
	}

	stream.close();
	return true;
}

void Client::new_from_console()
{
	Client new_client;
	const bool CHANGE_EVERYTHING[] = { false, false, false, false, false, false };
	if (new_client.granular_edit(CHANGE_EVERYTHING, false))
	{
		Client * ptr = new Client;
		*ptr = new_client;
		clients.insert(ptr);
	}

	return;
}

void Client::edit()
{
	utils::clear_screen();

	std::cout << FANCY_DELIMITER << std::endl;
	std::cout << "[1]" << LABELS[0] << name << std::endl;
	std::cout << "[2]" << LABELS[1] << nif << std::endl;
	std::cout << "[3]" << LABELS[2] << f_size << std::endl;
	std::cout << "[4]" << LABELS[3] << address << std::endl;
	std::cout << "[5]" << LABELS[4]; print_packs_purchased(std::cout);
	std::cout << "[6]" << LABELS[5] << total_purchased << std::endl;
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
			if (i == 0 || i >= 7)
			{
				out_of_bounds = true;
				break;
			}
		}

		if (out_of_bounds)
		{
			utils::print("Error, numbers must be between 1 and 6, please try again");
			continue;
		}

		bool what_to_change[] = { true, true, true, true, true, true};
		for (size_t i : choices)
		{
			what_to_change[i - 1] = false;
		}

		std::cout << "Press Enter to enter editing mode :> ";
		utils::wait_for_enter();

		granular_edit(what_to_change, true);
		return;

	}
}

bool Client::set_error(std::string error_str)
{
	is_valid = false;
	error_message = error_str;
	return is_valid;
}

bool Client::parse(std::ifstream & stream)
{
	//Name
	if (!utils::read_str(stream, name))
	{
		if (name == "EOF")
		{
			additional_error_info = "Good EOF, ditch this object";
			return set_error("EOF");
		}
		else
			return set_error("Parsing error while trying to parse the name of a client");
	}

	//NIF
	if (!utils::read_num(stream, nif))
	{
		if (nif == std::numeric_limits<size_t>::max())
			return set_error("EOF");
		else
			return set_error("Parsing error while trying to parse the NIF");
	}

	//Family size
	if (!utils::read_num(stream, f_size))
	{
		if (f_size == std::numeric_limits<unsigned short int>::max())
			return set_error("EOF");
		else
			return set_error("Parsing error while trying to parse the family size");
	}

	//Address
	if (!address.parse(stream))
	{
		if (address.get_error() == "EOF")
			return set_error("EOF");
		else
			return set_error("Parsing error while trying to parse the family size");
	}

	//Purchased travelpacks
	if (!parse_packs_purchased(stream))
		return false;

	//Total purchased
	if (!utils::read_num(stream, total_purchased))
	{
		if (total_purchased == std::numeric_limits<double>::max())
			return set_error("EOF");
		else
			return set_error("Parsing error while trying to parse the family size");
	}

	return is_valid = true;
}

bool Client::parse_packs_purchased(std::istream & stream)
{
	std::vector<size_t> ids;
	std::string input;
	if (!utils::read_str(stream, input))
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

		stream << travelpacks_purchased[i]->get_id();
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

			stream << travelpacks_purchased[i]->get_id();
		}
	}

	stream << std::endl;
}

bool Client::granular_edit(const bool keep_info[], bool edit_mode)
{
	utils::clear_screen();
	Client new_client;
	new_client.is_valid = true;

	if (edit_mode)
	{
		std::cout << "Information of client to be edited:" << std::endl;
		pprint();
		std::cout << std::endl << std::endl;
	}

	utils::print(FANCY_DELIMITER);

	//Client name
	if (keep_info[0])
	{
		std::cout << LABELS[0] << name << std::endl;
		new_client.name = name;
	}
	else
	{
		while (true)
		{
			std::cout << EDIT_LABELS[0];
			if (utils::read_str(std::cin, new_client.name))
				break;
			if (new_client.name == "EOF")
			{
				if (want_to_exit())
					return false;
				else
					continue;
			}
		}
	}

	//NIF
	if (keep_info[1])
	{
		std::cout << LABELS[1] << nif << std::endl;
		new_client.nif = nif;
	}
	else
	{
		while (true)
		{
			std::cout << EDIT_LABELS[1];
			if (utils::read_num(std::cin, new_client.nif))
				break;

			if (new_client.nif == std::numeric_limits<size_t>::max())
			{
				if (want_to_exit())
					return false;
				else
					continue;
			}
		}
	}

	//NIF
	if (keep_info[2])
	{
		std::cout << LABELS[2] << f_size << std::endl;
		new_client.f_size = f_size;
	}
	else
	{
		while (true)
		{
			std::cout << EDIT_LABELS[2];
			if (utils::read_num(std::cin, new_client.f_size))
				break;

			if (new_client.f_size == std::numeric_limits<unsigned short int>::max())
			{
				if (want_to_exit())
					return false;
				else
					continue;
			}
		}
	}

	//Address
	if (keep_info[3] == true)
	{
		std::cout << LABELS[3] << address << std::endl;
		new_client.address = address;
	}
	else
	{
		while (true)
		{
			std::cout << EDIT_LABELS[3];
			if (new_client.address.parse(std::cin))
				break;

			if (new_client.address.get_error() == "EOF")
			{
				if (want_to_exit())
					return false;
				else
					continue;
			}
		}
	}

	//Travelpacks purchased
	if (keep_info[4] == true)
	{
		std::cout << LABELS[4]; print_packs_purchased(std::cout);
		new_client.travelpacks_purchased = travelpacks_purchased;
	}
	else
	{
		while (true)
		{
			std::cout << EDIT_LABELS[4];
			if (new_client.parse_packs_purchased(std::cin))
				break;
			if (new_client.error_message == "EOF")
			{
				if (want_to_exit())
					return false;
				else
					continue;
			}

			if (new_client.error_message == "Reference error")
				std::cout << new_client.additional_error_info << std::endl;
		}
	}

	//Total money paid
	if (keep_info[5])
	{
		std::cout << LABELS[5] << total_purchased << std::endl;
		new_client.total_purchased = total_purchased;
	}
	else
	{
		while (true)
		{
			std::cout << EDIT_LABELS[5];
			if (utils::read_num(std::cin, new_client.total_purchased))
				break;

			if (new_client.total_purchased == std::numeric_limits<double>::max())
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
		std::cout << "Client information BEFORE edit:" << std::endl;
		pprint();
		std::cout << std::endl;

		std::cout << "Client information AFTER edit:" << std::endl;
		new_client.pprint();
		std::cout << std::endl;
	}
	else
	{
		std::cout << "Client information:" << std::endl;
		new_client.pprint();
		std::cout << std::endl;
	}

	//Do you wish to save?
	std::string answer = utils::yes_no_prompt("Do you wish to save(Y/n)? :> ", "YES");
	if (answer == "YES")
	{
		load_state(new_client);
		return true;
	}
	else
	{
		std::cout << "Please press Enter to exit editing mode:> ";
		utils::wait_for_enter();
		return false;
	}
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

void Client::load_state(const Client & donor)
{
	name                  = donor.name;
	nif                   = donor.nif;
	f_size                = donor.f_size;
	address               = donor.address;
	travelpacks_purchased = donor.travelpacks_purchased;
	total_purchased       = donor.total_purchased;
	is_valid              = donor.is_valid;
	error_message         = donor.error_message;
	additional_error_info = donor.additional_error_info;
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

void Client::erase(Client * ptr)
{
	if (clients.erase(ptr) == 0)
	{
		std::cout << "You tried to erase a unexisting client, aborting" << std::endl;
		exit(1);
	}
	delete ptr;
}

std::string Client::get_name() const
{
	return name;
}

size_t Client::get_nif() const
{
	return nif;
}

unsigned short Client::get_f_size() const
{
	return f_size;
}

Address Client::get_address() const
{
	return address;
}

std::vector<Travelpack*> Client::get_packs() const
{
	return travelpacks_purchased;
}

double Client::get_total_purchased() const
{
	return total_purchased;
}

bool Client::valid() const
{
	return is_valid;
}

std::string Client::get_error() const
{
	return error_message;
}
