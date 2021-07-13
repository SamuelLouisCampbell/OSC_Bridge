#pragma once
#define LINK_PLATFORM_WINDOWS
#include "ofMain.h"
#include <Link.hpp>
#include "ofxOsc.h"

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
	float speedVal = 0.1f;
	float toSendValue = 0.0f;
	float oldSendValue = toSendValue;
	State state;
	std::chrono::microseconds AL_time;
	const double quantum = 4.0f;
	ofxOscSender oscSend;

};
