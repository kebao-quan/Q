
#include <WinSock2.h>
#include <iostream>
#include <ws2tcpip.h>
#include <tchar.h>
#include "oqs_cpp.h"
#include "cryptlib.h"
#include "rijndael.h"
#include "secblock.h"
#include "osrng.h"
#include "files.h"
#include "hex.h"
#include "filters.h"
#include "modes.h"



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



int main()
{
	using namespace std;
	cout << "UDP Client" << endl;

	WORD wVersionRequested;
	WSADATA wsaData;
	int err;


	wVersionRequested = MAKEWORD(2, 2);

	err = WSAStartup(wVersionRequested, &wsaData);
	if (err != 0)
	{
		printf("WSAStartup errorNum = %d\n", GetLastError());
		return err;
	}
	if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2)
	{
		printf("LOBYTE errorNum = %d\n", GetLastError());
		WSACleanup();
		return -1;
	}


	SOCKET sockCli = socket(AF_INET, SOCK_DGRAM, 0);
	if (INVALID_SOCKET == sockCli)
	{
		printf("socket errorNo = %d\n", GetLastError());
		return -1;
	}

	SOCKADDR_IN addrSrv;
	InetPton(AF_INET, _T("127.0.0.1"), &addrSrv.sin_addr.s_addr);
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port = htons(6001);



	std::string kem_name = "Kyber512";
	oqs::KeyEncapsulation client{ kem_name };
	//std::cout << "\n\nKEM details:\n" << client.get_details() << std::endl;

	oqs::bytes client_public_key = client.generate_keypair();


	int len = sizeof(SOCKADDR_IN);
	std::string pKey = oqsBytesToString(client_public_key);
	const char* sendBuf = pKey.data();


	cout << client_public_key.size() << endl;
	cout << "length: " << pKey.length() << endl;
	//Dump(client_public_key, client_public_key.size());

	sendto(sockCli, sendBuf, pKey.length(), 0, (SOCKADDR*)&addrSrv, len);



	char recvBuf[1000] = { 0 };
	recvfrom(sockCli, recvBuf, 768, 0, (SOCKADDR*)&addrSrv, &len);
	std::string ciphertext_ = std::string(recvBuf, 768);
	//std::cout << "length: " << pKey.length() << std::endl;
	oqs::bytes ciphertext = stringToOqsBytes(ciphertext_);
	oqs::bytes shared_secret_client = client.decap_secret(ciphertext);
	cout << "Share key: " << endl;
	cout << oqs::hex_chop(shared_secret_client) << endl;

	std::cout << "server secret key established" << std::endl;


	using namespace CryptoPP;
	std::string sKey = oqsBytesToString(shared_secret_client);
	SecByteBlock key((const unsigned char*)(sKey.data()), sKey.size());


	//generate iv
	AutoSeededRandomPool prng;
	SecByteBlock iv(AES::BLOCKSIZE);
	prng.GenerateBlock(iv, iv.size());



	sendBuf = (const char*)iv.data();
	std::string ivtest = std::string(sendBuf, iv.size());
	std::cout << "iv: " << oqs::hex_chop(stringToOqsBytes(ivtest)) << std::endl;



	//sprintf_s(sendBuf, 100, "Ack: %s", recvBuf);
	sendto(sockCli, sendBuf, iv.size(), 0, (SOCKADDR*)&addrSrv, len);



	std::string cipher, recovered;
	int jlen = recvfrom(sockCli, recvBuf, 1000, 0, (SOCKADDR*)&addrSrv, &len);
	cipher = std::string(recvBuf, jlen);


	try
	{
		CBC_Mode< AES >::Decryption d;
		d.SetKeyWithIV(key, key.size(), iv);

		CryptoPP::StringSource s(cipher, true,
			new StreamTransformationFilter(d,
				new StringSink(recovered)
			) // StreamTransformationFilter
		); // StringSource

		std::cout << "recovered text: " << recovered << std::endl;
	}
	catch (const Exception& e)
	{
		std::cerr << e.what() << std::endl;
		exit(1);
	}










	system("pause");
	closesocket(sockCli);
	WSACleanup();
	return 0;
}