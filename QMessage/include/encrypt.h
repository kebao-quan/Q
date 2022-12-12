#pragma once
#ifndef _ENCRYPT_H_
#define _ENCRYPT_H_




#include "message.h"


class CServer
{
public:
	oqs::KeyEncapsulation* server;
	CServer(std::string kem_name) {
		server = new oqs::KeyEncapsulation{ kem_name };
	}

	oqs::bytes ciphertext;

	void encap(oqs::bytes client_public_key)
	{
		std::tie(ciphertext, shared_secret_server) = server->encap_secret(client_public_key);
	}
	//send back ciphertext

	oqs::bytes sendCipher()
	{
		return ciphertext;
	}

	oqs::bytes getSecret()
	{
		if (!shared_secret_server.empty())
		{
			return shared_secret_server;
		}
		std::cout << "No serect key available" << std::endl;
	}



private:
	oqs::bytes shared_secret_server{};
};

class CClient
{
public:
	oqs::KeyEncapsulation* client;
	oqs::bytes client_public_key;
	CClient(std::string kem_name)
	{
		client = new oqs::KeyEncapsulation{ kem_name };
		client_public_key = client->generate_keypair();
	}

	//TODO: Send Public Key
	oqs::bytes sendPub()
	{
		return client_public_key;
	}


	void decap(oqs::bytes ciphertext)
	{
		shared_secret_client = client->decap_secret(ciphertext);
	}

	void encrypt(CMessage message)
	{
		if (shared_secret_client.empty())
		{
			std::cout << "Please complete the KEM" << std::endl;
			return;
		}
		std::string data = message.data();
		oqs::bytes data_(data.begin(), data.end());
		std::cout << data << std::endl;
		std::cout << oqs::hex_chop(data_) << "\n";


		//oqs::bytes ciphertext;
		//oqs::C::OQS_AES128_ECB_enc(&data_[0], 64, &shared_secret_client[0], &ciphertext[0]);
		//std::cout << "ciphertext: " << ciphertext << std::endl;
		//std::cout << oqs::hex_chop(ciphertext) << "\n";

		std::string str;
		str.assign(data_.begin(), data_.end());
		std::cout << str << std::endl;
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



	oqs::bytes getSecret()
	{
		if (!shared_secret_client.empty())
		{
			return shared_secret_client;
		}
		std::cout << "No serect key available" << std::endl;
	}


private:
	oqs::bytes shared_secret_client{};
};










#endif // !_ENCRYPT_H_