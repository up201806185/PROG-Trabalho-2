#include <iostream>
#include "utils.h"

int main()
{
	int a;
	while (true)
	{
		std::cout << "ASD:> ";
		if (utils::read_num(std::cin, a))
			std::cout << "True" << std::endl;
		else
			std::cout << "False" << std::endl;

		std::cout << a << std::endl;
	}
}
