#ifndef CLIENT_H
#define CLIENT_H

#include <fstream>
#include <string>
#include <vector>
#include "Travelpack.h"
#include "Address.h"

class Client
{
	friend std::ostream& operator<<(std::ostream& out, const Client & client);
	friend std::ofstream& operator<<(std::ofstream& out, const Client & client);


public:
	Client();

	~Client();

	static bool read_file(const std::string & path);

	static void new_from_console();
	void        edit();
	void        mark_as_unavailable();

	std::string             get_name();
	size_t                  get_nif();
	unsigned short          get_f_size();
	Address                 get_address();
	std::vector<Travelpack&>/*???*/ get_packs();
	size_t                  get_total_purchased();

private:
	std::string name;
	size_t nif;
	unsigned short f_size;
	Address address;
	std::vector <Travelpack> packs;
	size_t total_purchased;
};

#endif