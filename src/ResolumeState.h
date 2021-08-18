#pragma once
#pragma once
#include "ofMain.h"
#include "ofxOsc.h"

class ResolumeState
{
public:
	void updateInputMessages(ofxOscReceiver& recv, std::vector<std::string>& terminalEntries);
	void sendOutputMessages(ofxOscSender& send);
	void updateTempoSpeedPhase(const float bpm, const float _phase);
	const size_t getCurSet();
	const size_t getCurCue();
	const size_t getCurCol();
	void setOffset(const int _offset);

private:
	const bool killContent();
private:
	size_t currentSet = 1;
	size_t currentCue = 1;
	size_t currentCol = 1;

	size_t nextSet = 1;
	size_t nextCue = 1;
					 
	size_t offset = 7;
	size_t nCues = 5;

	int numContentLayers = 4;
	float oldTempo = 128.0f;
	float tempo = 128.0f;
	float phase = 0.0f;
	float speed = 0.25f;
	float toSendValue = 0.0f;
	float floatBPM = 0.0f;
	float contentTargetBPM = 120.0f;
	bool killContentLayers = false;
	std::deque<ofxOscMessage> messageQ;

};

