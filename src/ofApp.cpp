#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	//Setup Ableton Link
	state.link.enable(true);
	std::cout << "State setup" << std::endl;
	oscSendRes.setup("127.0.0.1", 8000);
	oscRecRes.setup(8001);
	oscSendTouch.setup("127.0.0.1", 7001);
	oscRecTouch.setup(7000);
}

//--------------------------------------------------------------
void ofApp::update(){
	//update Ableton Link
	AL_time = state.link.clock().micros();
	phase = state.link.captureAppSessionState().phaseAtTime(AL_time, quantum);
	tempo = state.link.captureAppSessionState().tempo();

	//update res state
	resState.updateTempoSpeedPhase(tempo, phase);
	resState.updateInputMessages(oscRecTouch);
	resState.sendOutputMessages(oscSendRes);
	//update touch state
	touchState.updateTempoSpeedPhase(tempo, phase);
	touchState.updateInputMessages(oscRecRes);
	touchState.sendOutputMessages(oscSendTouch);

}

//--------------------------------------------------------------
void ofApp::draw(){
	
	std::stringstream ss; 
	int i_phase = phase + 1.0f;
	
	
	ss << "CONNECTIONS : " << state.link.numPeers()
		<< std::endl << "BPM  : " << tempo
		<< std::endl << "PHASE : " << i_phase
		<< std::endl << "Current Column : " << resState.getCurCol()
		<< std::endl << "Current Set : " << resState.getCurSet()
		<< std::endl << "Current Cue : " << resState.getCurCue();
	

	ofSetColor(255, 56, 0);
	//display rectangles
	ofDrawRectangle({ 100 + (30.0f * i_phase) , 250.0f }, 20.0f, 20.0f);
	ofSetColor(0, 255, 128);
	//draw info
	ofDrawBitmapString( ss.str(), 100.0f, 100.0f);
}

void ofApp::keyPressed(int key)
{
}
