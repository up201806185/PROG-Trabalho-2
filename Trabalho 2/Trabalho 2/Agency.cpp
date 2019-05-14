#include "Agency.h"

const std::string FANCY_DELIMITER = std::string(55, '=');
const std::vector<std::string> LABELS =
{
"Agency name      : ",
"NIF              : ",
"Website url      : ",
"Address          : ",
"Clients path     : ",
"Travelpacks path : "
};

Agency::Agency()
{
}


Agency::~Agency()
{
}

std::ostream & operator<<(std::ostream & stream, const Agency & agency)
{
	agency.print(stream);
	return stream;
}

void Agency::load(const std::string & path)
{
	std::ifstream stream(path);

	if (stream.fail())
	{
		utils::print("An error occurred while trying to load the Agency file: the file with path \"" +
			path + "\" could not be opened");
		exit(1);
	}

	//Name
	if (!utils::read_str(stream, name))
	{
		if (name == "EOF")
			utils::print("Unexpected EOF while trying to parse the name of the agency");
		else
			utils::print("Parsing error while trying to parse the name of the agency");
		exit(1);
	}

	//NIF
	if (!utils::read_num(stream, nif))
	{
		if (nif == std::numeric_limits<size_t>::max())
			utils::print("Unexpected EOF while trying to parse the NIF of the agency");
		else
			utils::print("Parsing error while trying to parse the NIF of the agency");	
		exit(1);
	}
	
	//URL
	if (!utils::read_str(stream, url))
	{
		if (url == "EOF")
			utils::print("Unexpected EOF while trying to parse the URL of the agency");
		else
			utils::print("Parsing error while trying to parse the URL of the agency");
		exit(1);
	}

	//Address
	if (!address.parse(stream))
	{
		if (address.get_error() == "EOF")
			utils::print("Unexpected EOF while trying to parse the address of the agency");
		else
			utils::print("Parsing error while trying to parse the address of the agency");
		exit(1);
	}

	//Clients' path
	if (!utils::read_str(stream, clients_path))
	{
		if (clients_path == "EOF")
			utils::print("Unexpected EOF while trying to parse the clients' path of the agency");
		else
			utils::print("Parsing error while trying to parse the clients' path of the agency");
		exit(1);
	}

	//Travelpacks' path
	if (!utils::read_str(stream, travelpacks_path))
	{
		if (travelpacks_path == "EOF")
			utils::print("Unexpected EOF while trying to parse the travelpacks' path of the agency");
		else
			utils::print("Parsing error while trying to parse the travelpacks' path of the agency");
		exit(1);
	}

	Travelpack().load(travelpacks_path);
	Client().load(clients_path);
}

bool Agency::save(const std::string & path)
{
	std::ofstream stream(path);
	if (stream.fail())
	{
		return false;
	}
	utils::print(name, stream);
	utils::print(nif, stream);
	utils::print(url, stream);
	utils::print(address, stream);
	utils::print(clients_path, stream);
	utils::print(travelpacks_path, stream);
	stream.close();
	return Client().save(clients_path) && Travelpack().save(travelpacks_path);
}

void Agency::print(std::ostream & stream) const
{
	stream << LABELS[0] << name << std::endl;
	stream << LABELS[1] << nif << std::endl;
	stream << LABELS[2] << url << std::endl;
	stream << LABELS[3] << address << std::endl;
	stream << LABELS[4] << clients_path << std::endl;
	stream << LABELS[5] << travelpacks_path << std::endl;
}

void Agency::pprint()
{
	utils::print(FANCY_DELIMITER);
	print(std::cout);
	utils::print(FANCY_DELIMITER);
}
