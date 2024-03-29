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
	void AddKillLayer(const size_t layerNo);
	const size_t getCurSet();
	const size_t getCurCue();
	const size_t getCurCol();
	void setOffset(const int _offset);
	const bool waitingForGo();
	void setTargetBPM(const float target);
	void setPreviewLayer(const size_t lay);
	

private:
	const bool killContent();
private:
	size_t currentSet = 0;
	size_t currentCue = 0;
	size_t currentCol = 0;

	size_t nextSet = 1;
	size_t nextCue = 1;
					 
	size_t offset = 10;
	size_t nCues = 5;

	size_t previewLayer = 3;
	size_t oldPreview = 0;
	size_t currentPreview = 0;

	float oldTempo = 128.0f;
	float tempo = 128.0f;
	float phase = 0.0f;
	float speed = 0.25f;
	float toSendValue = 0.0f;
	float floatBPM = 0.0f;
	float contentTargetBPM = 128.0f;
	bool killContentLayers = false;
	std::vector<size_t> killLayers;
	std::deque<ofxOscMessage> messageQ;
	bool waitForGo = false;
	size_t setCueCount = 0;

};

