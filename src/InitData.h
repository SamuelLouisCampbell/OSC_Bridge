#pragma once
#include <string>
#include <vector>

struct InitData
{
	std::string IPRes;
	std::string IPtouch;
	int oscSendResPort;
	int oscRecResPort;
	int oscSendTouchPort;
	int oscRecTouchPort;
	float contentTargetBPM;
	int clipOffset;
	std::vector<size_t> killCommands;
};