#pragma once
#ifndef _UTIL_H_
#define _UTIL_H_





#include <iostream>
#include <string>
#include "oqs_cpp.h"


std::string oqsBytesToString(oqs::bytes data_);

oqs::bytes stringToOqsBytes(std::string data_);

void Dump(oqs::bytes pData, size_t nSize);

void Dump(void* data, size_t nSize);








#endif // !_UTIL_H_