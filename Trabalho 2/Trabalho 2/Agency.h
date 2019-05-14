#ifndef AGENCY_H
#define AGENCY_H

#include <string>
#include "Address.h"
#include "Client.h"
#include "Travelpack.h"

//For testing
//Agency agency;
//agency.load("agency_test.txt");
class Agency
{
	friend std::ostream&  operator<<(std::ostream& stream, const Client & client);

public:
	Agency();
	~Agency();

	void load(const std::string & path);
	bool save(const std::string & path);

	void print(std::ostream & stream) const;
	void pprint();

private:
	std::string name;
	size_t nif;
	std::string url;
	Address address;
	std::string clients_path;
	std::string travelpacks_path;
};

#endif