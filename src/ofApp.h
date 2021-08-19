#pragma once
#define LINK_PLATFORM_WINDOWS
#include "ofMain.h"
#include <Link.hpp>
#include "ofxOsc.h"
#include "ResolumeState.h"
#include "TouchState.h"
#include "ofxImGui.h"


class ofApp : public ofBaseApp{
private:
	struct State
	{
		std::atomic<bool> running;
		ableton::Link link;

		State()
			: running(true)
			, link(120.0f)
		{
		}
	};
public:
	void setup();
	void update();
	void draw();
	void keyPressed(int key);


private:

//Gui stuff
	ofxImGui::Gui gui;
	char buffer_a[16] = { 0 };
	std::vector<std::string> terminalEntries;

//ableton stuff
	float phase;
	float tempo;

//res stuff
	ResolumeState resState;

//touch OSC stuff
	TouchState touchState;

//Timing stuff
	float speedVal = 0.1f;
	float toSendValue = 0.0f;
	float oldSendValue = toSendValue;

	State state;
	//OSC
	std::chrono::microseconds AL_time;
	const double quantum = 4.0f;
	ofxOscSender oscSendRes;
	ofxOscSender oscSendTouch;
	ofxOscReceiver oscRecTouch;
	ofxOscReceiver oscRecRes;
	std::string IPRes = "127.0.0.1";
	std::string IPtouch = "127.0.0.1";
	int oscSendResPort = 8000;
	int oscRecResPort = 8001;
	int oscSendTouchPort = 7001;
	int oscRecTouchPort = 7000;

};
