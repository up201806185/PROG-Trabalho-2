#include <iostream>
#include "utils.h"
#include "Date.h"
#include "Address.h"

using utils::print;
int main()
{
	while (true)
	{
		Address d(std::cin);
		print(d.valid());
		print(d.get_error());
		print(d);
	}
}
