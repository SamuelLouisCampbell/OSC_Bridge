#pragma once
#define LINK_PLATFORM_WINDOWS
#include "ofMain.h"
#include <Link.hpp>
#include "ofxOsc.h"

class resolumeClipLaunch
{
public:
	resolumeClipLaunch(int colToLaunch, const int startOffset);
	const int getColumnToLaunch() const;
private:
	int columnToLaunch;
	int startOffset;
};

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
	std::vector<resolumeClipLaunch> launchers;
	size_t resClipOffset = 8;
	float speedVal = 0.1f;
	float toSendValue = 0.0f;
	float oldSendValue = toSendValue;
	State state;
	std::chrono::microseconds AL_time;
	const double quantum = 4.0f;
	ofxOscSender oscSend;
	ofxOscReceiver oscRec;

};
