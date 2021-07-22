#pragma once
#include "ofxOsc.h"

class TouchState
{
public:
	void updateInputMessages(ofxOscReceiver& recv);
	void sendOutputMessages(ofxOscSender& send);
private:
	std::string label = "";

};

