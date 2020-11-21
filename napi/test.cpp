#include <string.h>
#include <iostream>
#include "calc.h"

int main()
{
	std::string str = "1+3-2*5";
	std::cout << calc(str) << std::endl;
}
