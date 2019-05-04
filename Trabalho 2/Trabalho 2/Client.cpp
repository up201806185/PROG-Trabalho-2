#include "Client.h"



Client::Client()
{
}


Client::~Client()
{
}

Client::Client(std::string name, unsigned nif, unsigned short f_size, Address address) {
	this->name = name;
	this->nif = f_size;
	this->f_size = f_size;
	this->address = address;
}

Client::Client(std::string name, unsigned VATnumber, unsigned short familySize, Address address, vector<Packet> & packets, unsigned totalPurchased) {

	// REQUIRES IMPLEMENTATION 
}


std::string Client::get_name() {

	return name;
}

size_t Client::get_nif() {

	return nif;
}

unsigned short Client::get_f_size() {

	return f_size;
}

Address Client::get_address() {

	return address;
}

std::vector<Travelpack> Client::get_packs() {

	return packs;
}

unsigned Client::get_total_purchased() {

	return total_purchased;
}


void Client::set_name(std::string nome) {

	this->name = name;
}

void Client::set_nif(size_t nif) {

	this->nif = nif;
}

void Client::set_f_size(unsigned short f_size) {

	this->f_size = f_size;
}
void Client::set_address(Address address) {

	this->address = address;
}
void Client::set_packs(std::vector<Travelpack> &packs) {

	this->packs = packs;
}
void Client::set_total_purchased(size_t total_purchased) {

	this->total_purchased = total_purchased;
}


std::ostream& operator<<(std::ostream& out, const Client & client) {

	out << client.str();
	return out;

}
