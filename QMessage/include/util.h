#pragma once

#include <iostream>
#include <string>
#include "oqs_cpp.h"


std::string oqsBytesToString(oqs::bytes data_);

oqs::bytes stringToOqsBytes(std::string data_);

void Dump(oqs::bytes pData, size_t nSize);