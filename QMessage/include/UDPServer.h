#pragma once

#include <WinSock2.h>
#include <iostream>
#include "oqs_cpp.h"
#include "util.h"
#include "encrypt.h"
#include "cryptlib.h"
#include "rijndael.h"
#include "secblock.h"
#include "osrng.h"
#include "files.h"
#include "hex.h"
#include "filters.h"
#include "modes.h"



BOOL initSockEnv()
{
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
		return FALSE;
	}
}

typedef struct args
{
	SOCKET sockSrv;
	SOCKADDR_IN addrCli;
	CryptoPP::SecByteBlock key;
	CryptoPP::SecByteBlock iv;
}*Pargs;

unsigned WINAPI thread_recvfrom(void* arg)
{
	using namespace CryptoPP;
	Pargs args = (Pargs)arg;
	SOCKET sockSrv = args->sockSrv;
	SOCKADDR_IN addrCli = args->addrCli;
	CryptoPP::SecByteBlock key = args->key;
	CryptoPP::SecByteBlock iv = args->iv;
	char recvBuf[1000];
	int len = sizeof(SOCKADDR_IN);
	std::string recovered, cipher;
	while (1)
	{
		int jlen = recvfrom(sockSrv, recvBuf, 1000, 0, (SOCKADDR*)&addrCli, &len);
		cipher = std::string(recvBuf, jlen);
		CBC_Mode< AES >::Decryption d;
		try
		{

			d.SetKeyWithIV(key, key.size(), iv);

			CryptoPP::StringSource s(cipher, true, new StreamTransformationFilter(d, new StringSink(recovered))); 

			std::cout << recovered.c_str() << std::endl;
		}
		catch (const Exception& e)
		{
			std::cerr << e.what() << std::endl;
			std::cout << "ERROR e" << std::endl;
			exit(1);
		}
		recovered.clear();
		//std::cout << "empty string: " << recovered.c_str() << std::endl;
	}
}


class CChat
{
public:
	CryptoPP::SecByteBlock key;
	CryptoPP::SecByteBlock iv;
	SOCKET sockSrv;
	SOCKADDR_IN addrCli;
	std::string cipher, plain;
	args a;
	CChat(CryptoPP::SecByteBlock key, CryptoPP::SecByteBlock iv, SOCKET sockSrv, SOCKADDR_IN addrCli) : key(key), iv(iv), sockSrv(sockSrv), addrCli(addrCli)
	{
		a = {
			sockSrv,
			addrCli,
			key,
			iv
		};
	}
	//recvfrom(sockSrv, recvBuf, 800, 0, (SOCKADDR*)&addrCli, &len))
	args getArgs()
	{	

		Pargs arguments = (Pargs)&a;
		return a;
	}

	BOOL startSending()
	{
		//sending until the program stop
		using namespace CryptoPP;
		int len = sizeof(SOCKADDR_IN);
		std::string plain;




		unsigned int ThreadId;
		//args a = getArgs();
		_beginthreadex(NULL, 0, &thread_recvfrom, &a, 0, &ThreadId);

		while (1)
		{
			cipher.clear();
			std::getline(std::cin, plain);
			try
			{
				CBC_Mode< AES >::Encryption e;
				e.SetKeyWithIV(key, key.size(), iv);
				CryptoPP::StringSource s(plain, true, new StreamTransformationFilter(e, new StringSink(cipher)));
			}
			catch (const Exception& e)
			{
				std::cerr << e.what() << std::endl;
				return FALSE;
			}

			const char* sendBuf = cipher.data();
			sendto(sockSrv, sendBuf, cipher.length(), 0, (SOCKADDR*)&addrCli, len);

		}
		return FALSE;
	}
};





unsigned WINAPI thread_start_server(void* arg)
{
	int port = 6001;
	std::cout << "listening on port " << port << std::endl;

	SOCKET sockSrv = socket(AF_INET, SOCK_DGRAM, 0);
	if (INVALID_SOCKET == sockSrv)
	{
		printf("socket errorNo = %d\n", GetLastError());
		return 0;
	}

	SOCKADDR_IN addrSrv;
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	addrSrv.sin_port = htons(port);


	if (SOCKET_ERROR == ::bind(sockSrv, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR_IN)))
	{
		printf("bind errorNo = %d", GetLastError());
		return 0;
	}
	SOCKADDR_IN addrCli;
	int len = sizeof(SOCKADDR_IN);


	char recvBuf[800] = { 0 };
	//char* sendBuf = new char[800];

	std::string kem_name = "Kyber512";
	//CEncrypt mEncrypt = CEncrypt(kem_name);
	CServer* mServer = new CServer(kem_name);

	while (1)
	{
		int publen = recvfrom(sockSrv, recvBuf, 800, 0, (SOCKADDR*)&addrCli, &len);
		if (SOCKET_ERROR == publen)
		{
			std::cout << WSAGetLastError() << std::endl;
			return 0;
		}
		std::string pKey = std::string(recvBuf, 800);
		//std::cout << "length: " << pKey.length() << std::endl;
		oqs::bytes client_public_key = stringToOqsBytes(pKey);

		//Dump(stringToOqsBytes(pKey), stringToOqsBytes(pKey).size());
		//std::cout << oqs::hex_chop(stringToOqsBytes(pKey)) << std::endl;
		mServer->encap(client_public_key);
		oqs::bytes ciphertext_ = mServer->sendCipher();
		std::string ciphertext = oqsBytesToString(ciphertext_);
		const char* sendBuf = ciphertext.data();
		sendto(sockSrv, sendBuf, ciphertext.length(), 0, (SOCKADDR*)&addrCli, len);


		std::cout << "server secret key established" << std::endl;
		std::cout << "Shared key: " << oqs::hex_chop(mServer->getSecret()) << std::endl;
		  

		using namespace CryptoPP;
		//convert key to type of CryptoPP::SecByteBlock
		std::string sKey = oqsBytesToString(mServer->getSecret());
		SecByteBlock key((const unsigned char*)(sKey.data()), sKey.size());


		recvfrom(sockSrv, recvBuf, 16, 0, (SOCKADDR*)&addrCli, &len);
		std::string iv_ = std::string(recvBuf, 16);
		std::cout << "iv: " << oqs::hex_chop(stringToOqsBytes(iv_)) << std::endl;
		SecByteBlock iv((const unsigned char*)(iv_.data()), iv_.size());


		//std::string plain = "Hello this is a message";
		//std::string cipher, recovered;
		//std::cout << "plain text: " << plain << std::endl;






		std::cout << "start chat" << std::endl;
		CChat m_chat(key, iv, sockSrv, addrCli);
		m_chat.startSending();

	}




	closesocket(sockSrv);
	WSACleanup();
	return 1;
}
