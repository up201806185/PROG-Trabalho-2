#include <iostream>
#include "utils.h"
#include "Date.h"

int main()
{
	Date d;
	d.set_today();
	std::cout << d << std::endl;
}
