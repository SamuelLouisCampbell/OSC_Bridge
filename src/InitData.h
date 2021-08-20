#pragma once
#include <string>

struct InitData
{
	std::string IPRes;
	std::string IPtouch;
	int oscSendResPort;
	int oscRecResPort;
	int oscSendTouchPort;
	int oscRecTouchPort;
	float contentTargetBPM;
};