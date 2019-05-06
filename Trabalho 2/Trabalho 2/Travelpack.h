#ifndef TRAVELPACK_H
#define TRAVELPACK_H

#include <string>
#include <vector>
#include "Date.h"
#include <fstream>

class Travelpack
{
	friend std::ostream&  operator<<(std::ostream&  stream, const Travelpack & travelpack);
	friend std::ofstream& operator<<(std::ofstream& stream, const Travelpack & travelpack);

public:
	Travelpack();
	~Travelpack();

	static void load(const std::string & path);
	static bool save(const std::string & path);

	static void new_from_console();
	void        edit();
	void        mark_as_unavailable();

	size_t                   get_id();
	bool					 get_available();
	std::vector<std::string> get_destinations();
	Date                     get_begginning();
	Date                     get_end();
	double                   get_price_per_person();
	size_t                   get_max_bought_tickets();
	size_t                   get_bought_tickets();
	
	bool valid() const;
	std::string get_error() const;

	bool purchase_n_tickets(unsigned short n_tickets);
private:
	bool check_coherence();
	bool set_error(std::string error_str);
	/**Returns true if the parsing went ok and the data is coherent
	Else returns false. Use the get_error method to see what happened
	*/
	bool parse(std::ifstream & stream);

	bool parse_destinations(std::istream & stream);
	void print_destinations(std::ostream & stream) const;

	static std::vector<Travelpack*> travelpacks;

	bool available;
	size_t id;
	std::vector<std::string> destinations;
	Date begginning;
	Date end;
	double price_per_person;
	size_t max_bought_tickets;
	size_t bought_tickets;

	bool is_valid;
	std::string error_message;
};

#endif