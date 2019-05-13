#ifndef TRAVELPACK_H
#define TRAVELPACK_H

#include <string>
#include <vector>
#include "Date.h"
#include <map>
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

	static bool id_exists(size_t id);
	static Travelpack * get_pointer_from_id(size_t id);

	static void print_all();
	void print(std::ostream & stream) const;
	void pprint();

	bool							is_between_dates(Date start, Date end) const;
	static void						new_from_console();
	void							edit();
	static std::vector<Travelpack*>	select_pack_vector();
	static std::vector<Travelpack*> fetch_by_date(const Date start, const Date end);
	static std::vector<Travelpack*> fetch_by_date(const Date start, const Date end, const std::vector<Travelpack*> & packs);
	static std::vector<Travelpack*> fetch_by_destination(std::string dest);
	static std::vector<Travelpack*> fetch_by_date_and_destination(const Date start, const Date end, std::string dest);
	static std::vector<Travelpack*> fetch_all();
	void							mark_as_unavailable();

	size_t                   get_id() const;
	bool					 get_available() const;
	std::vector<std::string> get_destinations() const;
	Date                     get_begginning() const;
	Date                     get_end() const;
	double                   get_price_per_person() const;
	size_t                   get_max_bought_tickets() const;
	size_t                   get_bought_tickets() const;
	
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

	bool granular_edit(const bool keep_info[], bool edit_mode);

	void load_state(const Travelpack & donor);

	static std::map<size_t, Travelpack*> travelpacks;

	static std::multimap<std::string, Travelpack*> destination_to_travelpack_map;
	void remove_destinations_from_map();
	void add_destinations_to_map();
	std::vector< std::multimap<std::string, Travelpack*>::iterator> map_iterators;

	bool available;
	size_t id;
	std::vector<std::string> destinations;//Duplicates can exist, I am not entirely sure how this could affect the the program
	Date begginning;
	Date end;
	double price_per_person;
	size_t max_bought_tickets;
	size_t bought_tickets;

	bool is_valid;
	std::string error_message;
};

#endif