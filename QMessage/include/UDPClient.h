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














unsigned WINAPI thread_start_client(std::string hostname, int port)
{
	using namespace std;
	//cout << "UDP Client" << endl;

	SOCKET sockCli = socket(AF_INET, SOCK_DGRAM, 0);
	if (INVALID_SOCKET == sockCli)
	{
		printf("socket errorNo = %d\n", GetLastError());
		return -1;
	}

	SOCKADDR_IN addrSrv;
	InetPton(AF_INET, hostname.c_str(), &addrSrv.sin_addr.s_addr);
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port = htons(port);



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
	sendto(sockCli, sendBuf, iv.size(), 0, (SOCKADDR*)&addrSrv, len);



	std::cout << "start chatting" << std::endl;
	CChat m_chat(key, iv, sockCli, addrSrv);
	m_chat.startSending();



	system("pause");
	closesocket(sockCli);
	WSACleanup();
	return 0;
}