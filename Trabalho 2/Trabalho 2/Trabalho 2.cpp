#include <iostream>
#include "utils.h"
#include "Date.h"
#include "Address.h"
#include "Travelpack.h"
#include "Client.h"
#include "pack_menu.h"
#include "client_menu.h"

using utils::print;
int main()
{
	Travelpack().load("tp_test.txt");
	Client().load("client_test.txt");
	
	Travelpack::travelpacks.at(1)->central_pprint();
}