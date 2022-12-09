#include "util.h"



std::string oqsBytesToString(oqs::bytes data_)
{
	std::string str;
	str.assign(data_.begin(), data_.end());
	return str;
}


oqs::bytes stringToOqsBytes(std::string data_)
{
	oqs::bytes data(data_.begin(), data_.end());
	return data;
}


void Dump(oqs::bytes pData, size_t nSize)
{
    std::string strOut;
    for (size_t i = 0; i < nSize; i++)
    {
        char buf[8] = "";
        if (i > 0 && (i % 16 == 0))
            strOut += "\n";
        snprintf(buf, sizeof(buf), "%02X ", pData[i] & 0xFF);
        strOut += buf;
    }
    strOut += "\n";
    std::cout << (strOut.c_str());
}