//Entry point of the application


#include "QMessage.h"





int main()
{
	using std::cout;
    using std::endl;
    using std::cin;
    initSockEnv();

    cout << "Are you a server or a client?" << endl;
    std::string mode;
    cin >> mode;
    if (mode == "server")
    {
        void* serverParam;
        if (thread_start_server(serverParam) == 0)
        {
            cout << "server thread error, terminated" << endl;
        }
    }
    else if (mode == "client")
    {
        std::string hostname;
        int port;
        cout << "Enter peer hostname: " << endl;
        cin >> hostname;
        cout << "Enter peer port" << endl;
        cin >> port;

        cout << "Connecting to " << hostname << " on port " << port << "......." << endl << endl;

        thread_start_client(hostname, port);
    }

    //unsigned int serverThreadId;
    //void* serverParam;
    //_beginthreadex(NULL, 0, &thread_start_server, serverParam, 0, &serverThreadId);

    //unsigned int returncode;
    ////_endthreadex(returncode);



    //std::string hostname, port;
    //cout << "Enter peer hostname: " << endl;
    //cin >> hostname;
    //cout << "Enter peer port" << endl;
    //cin >> port;

    //cout << "Connecting to " << hostname << "on port " << port << "......." << endl;
    //unsigned int clientThreadId;
    //void* clientParam;
    //_beginthreadex(NULL, 0, &thread_start_client, clientParam, 0, &clientThreadId);
    //int a;
    //cin >> a;
    // 
    // 
    // 
    // 
    // 
	//string PeerReceiverName = "";
	//string PeerReceiverPort = "";
	//cout << "Enter the peer hostname \n";
	//cin >> PeerReceiverName;
	//cout << "Enter the peer receiver port \n";
	//cin >> PeerReceiverPort;

  //  CServerSocket* pserver = CServerSocket::getInstance();
  //  int count = 0;
  //  if (pserver->InitSocket() == false)
  //  {
  //      cout << "Network Initialization failed, check network, network initialization failed" << endl;
  //      exit(0);
  //  }
  //  while (CServerSocket::getInstance() != NULL)
  //  {
		//if (pserver->AcceptClient() == false)
		//{
		//	if (count >= 3)
		//	{
		//		cout << "Cannot connect to the user, please try again, Connection failed" << endl;
		//		exit(0);
		//	}
		//	cout << "Cannot connect to the user, please try again, Connection failed" << endl;
		//	count++;
		//}
		//int ret = pserver->DealCommand();
  //  }





	//std::string kem_name = "Kyber512";
	////CEncrypt mEncrypt = CEncrypt(kem_name);
	//CServer* mServer = new CServer(kem_name);
	//CClient* mClient = new CClient(kem_name);

	//CMessage message = std::string("hi, my name is kevin");

	//oqs::bytes publicKey = mClient->sendPub();


	//mServer->encap(publicKey);
	//oqs::bytes ciphertext = mServer->sendCipher();

 //   cout << "DUMP" << endl;
 //   //Dump(ciphertext, ciphertext.size());
 //   cout << oqs::hex_chop(ciphertext) << endl;


	//mClient->decap(ciphertext);

	//mClient->encrypt(message);
	////cout << message.get();
	////cout << mEncrypt.mClient.client_public_key << endl;




    



    return 0;
}
