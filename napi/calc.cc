#include <iostream>
#include <string.h>
#include "calc.h"

std::string rmspace(std::string str);
float rec(std::string str);

float calc(std::string strin)
{
	float ans;

	std::getline(std::cin, strin);
	// cout << strin << endl;
	strin = rmspace(strin);
	// cout << "rmspace: " << strin << endl;

	//ans = rec(strin);
	//std::cout<<ans<<std::endl;
	return 123;
}

std::string rmspace(std::string str)
{
	std::string str1;
	unsigned int i;

	for (i = 0; i < str.length(); i++) {
		// cout << "\"" << str[i] << "\"" << endl;
		if (str[i] != ' ') {
			str1.push_back(str[i]);
		}
	}
	return str1;
}

float rec(std::string str)
{
	int i;
	int isnum = 1;
	int brace_count = 0;
	
	for (i = str.length() - 1; i >= 0; i--) {
		if (brace_count >= 1) {
			if (str[i] == '(') {
				brace_count -= 1;
			}
		} else {
			switch (str[i]) {
				case ')': {
					isnum = 0;
					brace_count += 1;
					break;
				}
				case '+': {
					isnum = 0;
					// cout << "detect +" << endl;
					// cout << i << endl;
					return rec(str.substr(0, i)) +
						rec(str.substr(i + 1, str.length() - i - 1));
					break;
				}
				case '-': {
					isnum = 0;
					// cout << "detect -" << endl;
					return rec(str.substr(0, i)) -
						rec(str.substr(i + 1, str.length() - i - 1));
					break;
				}
			}
		}
	}
	for (i = str.length() - 1; i >= 0; i--) {
		if (brace_count >= 1) {
			if (str[i] == '(') {
				brace_count -= 1;
			}
		} else {
			switch (str[i]) {
				case ')': {
					isnum = 0;
					brace_count += 1;
					break;
				}
				case '*': {
					isnum = 0;
					// cout << "detect *" << endl;
					return rec(str.substr(0, i)) *
						rec(str.substr(i + 1, str.length() - i - 1));
					break;
				}
				case '/': {
					isnum = 0;
					// cout << "detect /" << endl;
					return rec(str.substr(0, i)) /
						rec(str.substr(i + 1, str.length() - i - 1));
					break;
				}
			}
		}
	}
	if (isnum == 1) {
		// cout << stof(str) << " ";
		return std::stof(str);
	}
	if (str[0] == '(' && str[str.length() - 1] == ')') {
		// cout << "only brace" << endl;
		return rec(str.substr(1, str.length() - 2));
	}
	return 0;
}
