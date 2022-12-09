#pragma once
#include <iostream>
#include "oqs_cpp.h"

class CMessage 
{
private:
	size_t stdStringLen{};
	size_t stringLen{};

	



public:
	CMessage() {};


	CMessage(std::string m) : message(m)
	{
		stringLen = m.length();
		stdStringLen = sizeof(m);
	}

	std::string message{};

	std::string printMessage();


	void test()
	{
		std::cout << stringLen << std::endl;
		std::cout << stdStringLen << std::endl;
	}


	std::string get()
	{
		return message.c_str();
	}

	std::string data()
	{
		return message.data();
	}

};