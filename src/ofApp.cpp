#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	//Setup Ableton Link
	state.link.enable(true);
	std::cout << "State setup" << std::endl;
	oscSend.setup("127.0.0.1", 8000);
	oscRec.setup(7000);
}

//--------------------------------------------------------------
void ofApp::update(){
	//update Ableton Link
	AL_time = state.link.clock().micros();

	while (oscRec.hasWaitingMessages())
	{
		//Get the next message;
		ofxOscMessage m;
		oscRec.getNextMessage(&m);
		std::string s = m.getAddress();

		if (s.substr(0, 4) == "/cue")
		{
			std::stringstream ss;
			ss << "/cue/" << m.getArgAsInt(0) << " : detected." << std::endl;
			std::cout << ss.str();
			launchers.emplace_back(m.getArgAsInt(0) - 1, resClipOffset);
		}
	}

	const auto phase = state.link.captureAppSessionState().phaseAtTime(AL_time, quantum);

	while (launchers.size() != 0 && phase < 0.06)
	{
		std::stringstream ss;
		ss << "/composition/columns/" << launchers.back().getColumnToLaunch() << "/connect";
		currColumn = launchers.back().getColumnToLaunch();
		ofxOscMessage m;
		m.setAddress(ss.str());
		m.addFloatArg(1.0f);
		oscSend.sendMessage(m);
		launchers.pop_back();
	}
	
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
	float floatBPM = ofMap(tempo, 20.0f, 500.0f, 0.0f, 1.0f);
	float toSendValue = std::pow(convertMe, 1.0f / 1.6609640474437f);
	
	
	ss  << "CONNECTIONS : " << state.link.numPeers()
		<< std::endl << "BPM  : " << tempo
		<< std::endl << "BEATS : " << beats
		<< std::endl << "PHASE : " << i_phase
		<< std::endl << "LOCAL SPEED VALUE : " << convertMe
		<< std::endl << "Time TO SEND VIA OSC : " << toSendValue
		<< std::endl << "Current Column : " << currColumn;	



	if (oldSendValue != toSendValue)
	{
		//send speed
		ofxOscMessage m;
		m.setAddress("/composition/speed");
		m.addFloatArg(toSendValue);
		oscSend.sendMessage(m);
		oldSendValue = toSendValue;
		//send tempo
		ofxOscMessage n;
		n.setAddress("/composition/tempocontroller/tempo");
		n.addFloatArg(floatBPM);
		oscSend.sendMessage(n);
	}
	
	
	ofSetColor(255, 56, 0);
	
	//display rectangles
	ofDrawRectangle({ 100 + (30.0f * i_phase) , 200.0f }, 20.0f, 20.0f);
	ofSetColor(0, 255, 128);
	//draw info
	ofDrawBitmapString( ss.str(), 100.0f, 100.0f);
}

void ofApp::keyPressed(int key)
{
}

resolumeClipLaunch::resolumeClipLaunch(int colToLaunch, const int startOffset)
	:
	columnToLaunch(colToLaunch),
	startOffset(startOffset)
{
}

const int resolumeClipLaunch::getColumnToLaunch() const
{
	return (columnToLaunch * startOffset) + 1;
}
