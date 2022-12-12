#include "UDPClient.h"


BOOL start_client(std::string hostname, int port, bool* running)
{
	unsigned int ThreadId;

	std::string hostnameCopy = hostname;
	int portCopy = port;
	bool runningCopy = *running;
	bool* p_runningCopy = &runningCopy;

	static client_args a;
	a.hostname = hostnameCopy;
	a.port = portCopy;
	a.running = p_runningCopy;
	std::cout << a.hostname << std::endl;
	std::cout << a.port << std::endl;

	if (!_beginthreadex(NULL, 0, &thread_start_client, &a, 0, &ThreadId))
	{
		std::cout << "thread_start_client error" << std::endl;
		return FALSE;
	}
	return TRUE;
}


unsigned WINAPI thread_start_client(void* arg)
{
	using namespace std;
	cout << "UDP Client" << endl;

	std::string hostname;


	client_args* a = (client_args*)arg;
	hostname = a->hostname;
	int port = a->port;



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
	cout << "connecting to port " << port << endl;


	std::string kem_name = "Kyber512";
	oqs::KeyEncapsulation client{ kem_name };
	//std::cout << "\n\nKEM details:\n" << client.get_details() << std::endl;

	oqs::bytes client_public_key = client.generate_keypair();

	int len = sizeof(SOCKADDR_IN);
	std::string pKey = oqsBytesToString(client_public_key);
	const char* sendBuf = pKey.data();


	//Dump(client_public_key, client_public_key.size());

	sendto(sockCli, sendBuf, pKey.length(), 0, (SOCKADDR*)&addrSrv, len);



	char recvBuf[1000] = { 0 };
	recvfrom(sockCli, recvBuf, 768, 0, (SOCKADDR*)&addrSrv, &len);
	std::string ciphertext_ = std::string(recvBuf, 768);
	//std::cout << "length: " << pKey.length() << std::endl;
	oqs::bytes ciphertext = stringToOqsBytes(ciphertext_);
	oqs::bytes shared_secret_client = client.decap_secret(ciphertext);
	std::cout << "client secret key established" << std::endl;
	cout << "Shared key: " << oqs::hex_chop(shared_secret_client) << endl;
	cout << 1 << endl;


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



	std::cout << "start chat" << std::endl;
	CChat m_chat(key, iv, sockCli, addrSrv);
	m_chat.startSending();



	system("pause");
	closesocket(sockCli);
	WSACleanup();
	return 0;
}