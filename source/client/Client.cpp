#include "Client.hpp"

#include <iostream>
#include <mutex>
#include <vector>

#include "opencv2/opencv.hpp"

Client::Client()
	: mClientSendThread()
{
	initializeConnection();
	clientReceive();
}

Client::~Client()
{
	cleanUp();
}

void Client::initializeConnection()
{
	// Initialize WinSock
	WSAData data;
	WORD ver = MAKEWORD(2, 2);
	int wsResult = WSAStartup(ver, &data);
	if (wsResult != 0)
	{
		std::cerr << "Can't start Winsock, Err #" << wsResult << std::endl;
		return;
	}

	// Create socket
	mClientSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (mClientSocket == INVALID_SOCKET)
	{
		std::cerr << "Can't create socket, Err #" << WSAGetLastError() << std::endl;
		WSACleanup();
		return;
	}

	// Fill in a hint structure
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(mServerPort);
	inet_pton(AF_INET, mServerIpAddress.c_str(), &hint.sin_addr);

	// Connect to server
	int connResult = connect(mClientSocket, (sockaddr*)&hint, sizeof(hint));
	if (connResult == SOCKET_ERROR)
	{
		std::cerr << "Can't connect to server, Err #" << WSAGetLastError() << std::endl;
		cleanUp();
		return;
	}
}

void Client::startSend()
{
	mIsClientSendTerminated = false;
	mClientSendThread = std::thread(&Client::sendData, this);
	mClientSendThread.detach();
}

void Client::stopSend()
{
	mIsClientSendTerminated = true;
}

void Client::cleanUp()
{
	// Gracefully close down everything
	closesocket(mClientSocket);
	WSACleanup();
}

void Client::sendData()
{
	// While loop to send data
	// ### OPEN CV CODE
	int capDev = 0;
	cv::VideoCapture cap(capDev); // open the default camera

	cv::Mat img;
	img = cv::Mat::zeros(480, 640, CV_8UC3);
	int imgSize = img.total() * img.elemSize();

	//make img continuos
	if (!img.isContinuous()) {
		img = img.clone();
	}

	//cv::namedWindow("CV Video Client", 1);

	while (true)
	{
		if (mIsClientSendTerminated)
		{
			break;
		}
		cap >> img;
		//cv::imshow("CV Video Client", img);

		if (int bytes = send(mClientSocket, reinterpret_cast<char*>(img.data), imgSize, 0) < 0) {
			std::cerr << "bytes = " << bytes << std::endl;
			break;
		}
		cv::waitKey(10);
	}

	cap.release();
	// ### OPEN CV CODE END
}

void Client::clientReceive()
{
	do
	{
		// Wait for response
		const int maxRecvMsgSize = 10;
		char buf[maxRecvMsgSize];
		ZeroMemory(buf, maxRecvMsgSize);
		if (int bytes = recv(mClientSocket, buf, maxRecvMsgSize, MSG_WAITALL) == -1)
		{
			std::cerr << "recv failed, received bytes = " << bytes
				<< ", error: " << WSAGetLastError() << std::endl;
			sendMsgToServer("Server request failed!");
		}

		std::mutex msgMutex;
		std::lock_guard<std::mutex> lock(msgMutex);
		if (strcmp(buf, "stop") == 0)
		{
			stopSend();
			sendMsgToServer("Streaming stopped...");
		}
		else if (strcmp(buf, "start") == 0)
		{
			if (!mIsClientSendTerminated)
			{
				sendMsgToServer("Stream is already started...");
			}
			else
			{
				if (int bytes = sendMsgToServer("Starting stream...") >= 0)
				{
					startSend();
				}
			}
		}
	} while (true);
}

int Client::sendMsgToServer(const char* msg)
{
	const int maxMsgSize = 50;
	std::cout << msg << std::endl;
	return send(mClientSocket, msg, maxMsgSize, 0);
}