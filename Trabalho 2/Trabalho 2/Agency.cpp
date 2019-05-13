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
