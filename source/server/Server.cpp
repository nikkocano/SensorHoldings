#include "Server.hpp"

#include <iostream>
#include <thread>
#include <vector>

#pragma comment (lib, "ws2_32.lib")

Server::Server()
{
	initializeConnection();
	establishConnectionToClient();
}

Server::~Server()
{
	cleanUp();
}

void Server::cleanUp()
{
	// Gracefully close down everything
	closesocket(mServerSocket);
	closesocket(mClientSocket);
	WSACleanup();
}

void Server::initializeConnection()
{
	// Initialize winsock
	WSADATA wsData;
	WORD ver = MAKEWORD(2, 2);

	int wsResult = WSAStartup(ver, &wsData);
	if (wsResult != 0)
	{
		std::cerr << "Can't start Winsock, Err #" << wsResult << std::endl;
		return;
	}
	
	// Create a socket
	mServerSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (mServerSocket == INVALID_SOCKET)
	{
		std::cerr << "Can't create socket, Err #" << WSAGetLastError() << std::endl;
		WSACleanup();
		return;
	}

	// Bind the ip address and port to a socket
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(54000);
	hint.sin_addr.S_un.S_addr = INADDR_ANY; // Could also use inet_pton ...

	bind(mServerSocket, (sockaddr*)&hint, sizeof(hint));

	// Tell winsock the socket is for listening
	listen(mServerSocket, SOMAXCONN);
}

void Server::establishConnectionToClient()
{
	std::thread t1(&Server::startConnection, this);
	t1.detach();
}

void Server::startConnection()
{
	// Wait for a connection
	sockaddr_in client;
	int clientSize = sizeof(client);

	mClientSocket = accept(mServerSocket, (sockaddr*)&client, &clientSize);
	if (mClientSocket == INVALID_SOCKET)
	{
		std::cerr << "Can't create socket, Err #" << WSAGetLastError() << std::endl;
		WSACleanup();
		return;
	}

	char host[NI_MAXHOST]; // Client's remote name
	char service[NI_MAXHOST]; // Service (i.e. port) the client is connect on
	ZeroMemory(host, NI_MAXHOST); // same as memset(host, 0, NI_MAXHOST);
	ZeroMemory(service, NI_MAXHOST);

	if (getnameinfo((sockaddr*)&client, sizeof(client), host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0)
	{
		std::cout << host << " connected on port " << service << std::endl;
	}
	else
	{
		inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
		std::cout << host << " connected on port " << ntohs(client.sin_port) << std::endl;;
	}

	// while loop; accept data from client
	// ### OPEN CV CODE
	cv::Mat img;
	img = cv::Mat::zeros(480, 640, CV_8UC3);
	int imgSize = img.total() * img.elemSize();
	uchar* iptr = img.data;
	int bytes = 0;
	char key = 'a';

	if (!img.isContinuous()) {
		img = img.clone();
	}

	std::cout << "Image Size:" << imgSize << std::endl;
	cv::namedWindow("CV Video Server", 1);

	while (key != 'q') {
		// probably better to send encoded
		if ((bytes = recv(mClientSocket, reinterpret_cast<char*>(img.data), imgSize, MSG_WAITALL)) == -1)
		{
			std::cerr << "recv failed, received bytes = " << bytes << std::endl;
			std::cerr << "error: " << WSAGetLastError() << std::endl;
		}

		cv::imshow("CV Video Server", img);

		if (mIsRecordingOn)
		{
			mVideoWriter.write(cv::Mat(480, 640, CV_8UC3, iptr));
		}

		if (key = cv::waitKey(10) >= 0)
		{
			break;
		}
	}
	// ### OPEN CV CODE END

	if (mVideoWriter.isOpened())
	{
		mVideoWriter.release();
		mIsRecordingOn = false;
	}
	cleanUp();
}

std::string Server::start()
{
	return sendMsgToClient("start");
}

std::string Server::stop()
{
	return sendMsgToClient("stop");
}

/*
 * Improvement: Maybe create 1 recv function that will parse the buffer
 *              then process as intended (e.g. image stream/client responses).
 *              Main thread will now only 'send' messages to client.
 */
std::string Server::sendMsgToClient(const char* action)
{
	const int maxSendMsgSize = 10;
	if (int bytes = send(mClientSocket, action, maxSendMsgSize, 0) < 0)
	{
		std::cerr << "Message " << action << " failed. bytes = " << bytes << std::endl;
		return "Sending " + std::string(action) + " failed!";
	}

	const int maxRecvMsgSize = 50;
	char buf[maxRecvMsgSize];
	ZeroMemory(buf, maxRecvMsgSize);
	if (int bytes = recv(mClientSocket, buf, maxRecvMsgSize, MSG_WAITALL) == -1) // This is buggy
	{
		std::cerr << "recv failed, received bytes = " << bytes 
			<< ", error: " << WSAGetLastError() << std::endl;
		return "Client response for " + std::string(action) + " failed!";
	}

	return std::string(buf);
}

std::string Server::save()
{
	mVideoWriter.open(mOutputFileName, cv::VideoWriter::fourcc('M', 'J', 'P', 'G'),
		20.0, cv::Size(640, 480), true);
	if (mVideoWriter.isOpened())
	{
		mIsRecordingOn = true;
		return "Saving to file out.avi...";

	}
	return "File save error...";
}
