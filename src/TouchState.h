#pragma once
#include "ofxOsc.h"
#include "ResolumeState.h"

class TouchState
{
public:
	void updateInputMessages(ofxOscReceiver& recv);
	void sendOutputMessages(ofxOscSender& send, ResolumeState& rs);
	void updateTempoSpeedPhase(const float bpm, const float _phase);
private:
	std::string label = "";
	float oldTempo = 128.0f;
	float tempo = 128.0f;
	float phase = 0.0f;
	float oldPhase = 0.0f;
	int OldResClip = 0;

};

