#pragma once

#include <fstream>
#include <string>
#include <vector>
#include "Travelpack.h"

class Client
{
	friend ostream& operator<<(ostream& out, const Client & client);

public:
	Client(std::string name, size_t nif, unsigned short f_size, Address address);
	Client(std::string name, size_t nif, unsigned short f_size, Address address, std::vector<TravelPack> &packs, size_t total_purchased);

	~Client();

	std::string get_name() const;
	size_t get_nif() const;
	unsigned short get_f_size() const;
	Address get_address() const;
	std::vector<TravelPack> get_packs() const;
	size_t get_total_purchased() const;

	void set_name(std::string name);
	void set_nif(unsigned nif);
	void set_f_size(unsigned short f_size);
	void set_address(Address address);
	void set_packs(vector<TravelPack> &packs);
	void setTotalPurchased(unsigned total_purchased);

private:
	std::string name;
	size_t nif;
	unsigned short f_size;
	Address address;
	std::vector <TravelPack> packs;
	size_t total_purchased;
};

