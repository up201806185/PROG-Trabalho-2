#ifndef CLIENT_H
#define CLIENT_H

#include <fstream>
#include <string>
#include <vector>
#include <set>
#include "Travelpack.h"
#include "Address.h"

class Client
{
	friend std::ostream&  operator<<(std::ostream& stream, const Client & client);
	friend std::ofstream& operator<<(std::ofstream& stream, const Client & client);
public:
	Client();
	~Client();

	static void load(const std::string & path);
	static bool save(const std::string & path);

	static void new_from_console();
	void        edit();
	void        erase();

	std::string              get_name() const;
	size_t                   get_nif() const;
	unsigned short           get_f_size() const;
	Address                  get_address() const;
	std::vector<Travelpack*> get_packs() const;
	double                   get_total_purchased() const;

	bool valid() const;
	std::string get_error() const;

private:
	bool set_error(std::string error_str);

	/**Returns true if the parsing went ok and the data is coherent
	Else returns false. Use the get_error method to see what happened
	*/
	bool parse(std::ifstream & stream);

	bool parse_packs_purchased(std::istream & stream);
	void print_packs_purchased(std::ostream & stream) const;
	void print_packs_purchased(std::ofstream & stream) const;

	bool granular_edit(const bool keep_info[], bool edit_mode);

	void print(std::ostream & stream) const;
	void pprint();

	void load_state(const Client & donor);

	static std::vector<Client*> clients;

	std::string name;
	size_t nif;
	unsigned short f_size;
	Address address;
	std::vector <Travelpack*> travelpacks_purchased;
	double total_purchased;

	bool is_valid;
	std::string error_message;

	std::string additional_error_info;
};
#endif