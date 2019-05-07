#include "Client.h"



Client::Client()
{
}


Client::~Client()
{
}

bool Client::set_error(std::string error_str)
{
	is_valid = false;
	error_message = error_str;
	return is_valid;
}
