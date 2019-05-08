#include <iostream>
#include "utils.h"
#include "Date.h"
#include "Address.h"
#include "Travelpack.h"
#include "Client.h"

using utils::print;
int main()
{
	Travelpack().load("tp_test.txt");
	Client().load("client_test.txt");
	Client().save("client_test2.txt");
}
