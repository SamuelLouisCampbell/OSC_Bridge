#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	//Setup Ableton Link
	state.link.enable(true);
	std::cout << "State setup" << std::endl;
	oscSend.setup("127.0.0.1", 8000);
}

//--------------------------------------------------------------
void ofApp::update(){
	//update Ableton Link
	AL_time = state.link.clock().micros();
}

//--------------------------------------------------------------
void ofApp::draw(){
	
	std::stringstream ss; 
	float tempo = state.link.captureAppSessionState().tempo();
	const auto beats = state.link.captureAppSessionState().beatAtTime(AL_time, quantum);
	const auto phase = state.link.captureAppSessionState().phaseAtTime(AL_time, quantum);
	int i_phase = phase + 1.0f;

	//convert bpm to "SPEED" 

	float convertMe = ofMap(tempo,60.0f,240.0f, 0.05f, 0.2f);
	
	float toSendValue = std::pow(convertMe, 1.0f / 1.6609640474437f);
	
	
	ss << std::endl << "BPM  : " << tempo
		<< std::endl << "BEATS : " << beats
		<< std::endl << "PHASE : " << i_phase
		<< std::endl << "LOCAL SPEED VALUE : " << convertMe
		<< std::endl << "Time TO SEND VIA OSC : " << toSendValue;



	if (oldSendValue != toSendValue)
	{
		ofxOscMessage m;
		m.setAddress("/composition/speed");
		m.addFloatArg(toSendValue);
		oscSend.sendMessage(m);
		oldSendValue = toSendValue;
	}
	
	
	ofSetColor(255, 56, 0);
	

	ofDrawRectangle({ 100 + (30.0f * i_phase) , 200.0f }, 20.0f, 20.0f);


	ofSetColor(0, 255, 128);

	ofDrawBitmapString( ss.str(), 100.0f, 100.0f);
}

void ofApp::keyPressed(int key)
{
	//if (key == 'a' || key == 'A')
	//{
	//	speedVal += 0.001f;
	//}
	//if (key == 's' || key == 'S')
	//{
	//	speedVal -= 0.001f;
	//}

}

