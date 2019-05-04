#pragma once

#include <fstream>
#include <string>
#include <vector>
#include "Travelpack.h"

class Client
{
	friend std::ostream& operator<<(std::ostream& out, const Client & client);

public:
	Client();
	Client(std::string name, size_t nif, unsigned short f_size, Address address);
	Client(std::string name, size_t nif, unsigned short f_size, Address address, std::vector<Travelpack> &packs, size_t total_purchased);

	~Client();

	std::string get_name();
	size_t get_nif();
	unsigned short get_f_size();
	Address get_address();
	std::vector<Travelpack> get_packs();
	size_t get_total_purchased();

	void set_name(std::string name);
	void set_nif(size_t nif);
	void set_f_size(unsigned short f_size);
	void set_address(Address address);
	void set_packs(std::vector<Travelpack> &packs);
	void set_total_purchased(size_t total_purchased);

private:
	std::string name;
	size_t nif;
	unsigned short f_size;
	Address address;
	std::vector <Travelpack> packs;
	size_t total_purchased;
};

