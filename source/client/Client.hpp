#pragma once

#include <string>
#include <thread>
#include <WS2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

class Client
{
public:
	Client();
	~Client();

private:
	void initializeConnection();
	void sendData();
	void clientReceive();
	void startSend();
	void stopSend();
	void cleanUp();
	int sendMsgToServer(const char* msg);

	std::string mServerIpAddress = "127.0.0.1"; // IP Address of the server
	int mServerPort = 54000; // Listening port # on the server
	bool mIsClientSendTerminated = true;
	std::thread mClientSendThread;
	SOCKET mClientSocket;
};

