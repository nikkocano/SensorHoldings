#pragma once

#include <functional>
#include <string>
#include <WS2tcpip.h>

#include "opencv2/opencv.hpp"

class Server
{
public:
	Server();
	~Server();

	std::string start();
	std::string stop();
	std::string save();

private:
	void establishConnectionToClient();
	void startConnection();
	void initializeConnection();
	std::string sendMsgToClient(const char* action);
	void cleanUp();

	std::string mOutputFileName = "out.avi";
	bool mIsRecordingOn = false;
	SOCKET mServerSocket;
	SOCKET mClientSocket;
	cv::VideoWriter mVideoWriter;
};