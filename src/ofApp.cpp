#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	//gui
	gui.setup();
	ofSetFrameRate(60);
	//Setup Ableton Link
	state.link.enable(true);
	std::cout << "State setup" << std::endl;
	oscSendRes.setup(IPRes.c_str(), oscSendResPort);
	oscRecRes.setup(oscRecResPort);
	oscSendTouch.setup(IPtouch.c_str(), oscSendTouchPort);
	oscRecTouch.setup(oscRecTouchPort);
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
	std::stringstream bridgeString;
	std::stringstream cuesString;
	int i_phase = phase + 1.0f;

	

	bridgeString << "ABLETON BRIDGE CONNECTIONS : " << state.link.numPeers()
		<< std::endl << "CURRENT BPM  : " << tempo
		<< std::endl << "CURRENT PHASE : " << i_phase;
	
	cuesString << "Current Column : " << resState.getCurCol()
		<< std::endl << "Current Set : " << resState.getCurSet()
		<< std::endl << "Current Cue : " << resState.getCurCue();

	gui.begin();
	ImGui::Begin("Bridge");
	if (ImGui::BeginChild("Bridge"))
	{

		ImGui::Text(bridgeString.str().c_str());	
		ImGui::RadioButton("One", i_phase == 1);
		ImGui::RadioButton("Two", i_phase == 2);
		ImGui::RadioButton("Three", i_phase == 3);
		ImGui::RadioButton("Four", i_phase == 4);
		ImGui::Text(cuesString.str().c_str());
		ImGui::EndChild();
	}
	ImGui::End();
	gui.end();

}

void ofApp::keyPressed(int key)
{
}
