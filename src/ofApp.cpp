#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

	//OF stuff
	ofSetWindowTitle("A&B Bridge");
	
	//get data from file
	if (!data.LoadData("defaults.txt"))
		terminalEntries.emplace_back("FAILED TO LOAD DEFAULTS.TXT\nLOADED DEFAULT VALUES INSTEAD");


	//gui
	gui.setup();
	ofSetFrameRate(60);
	//Setup Ableton Link
	state.link.enable(true);
	
	//Use loaded or default data to launch all of the things
	oscSendRes.setup(data.getData().IPRes.c_str(), data.getData().oscSendResPort);
	oscRecRes.setup(data.getData().oscRecResPort);
	oscSendTouch.setup(data.getData().IPtouch.c_str(), data.getData().oscSendTouchPort);
	oscRecTouch.setup(data.getData().oscRecTouchPort);
	resState.setTargetBPM(data.getData().contentTargetBPM);

	resState.AddKillLayer(1);
	resState.AddKillLayer(3);
	resState.AddKillLayer(4);
	
	
	//Print that to user.
	std::stringstream ss;
	ss <<    "Resolume IP   : " << data.getData().IPRes.c_str() <<  "\nOSC Pad IP    : " << data.getData().IPtouch
		<< "\nTo Res port   : " << data.getData().oscSendResPort << "\nFrom Res port : " << data.getData().oscRecResPort
		<< "\nTo Pad port   : " << data.getData().oscSendTouchPort << "\nFrom Pad port : " << data.getData().oscRecTouchPort
		<< "\nContent BPM   : " << data.getData().contentTargetBPM << std::endl;
	terminalEntries.emplace_back(ss.str());
}

//--------------------------------------------------------------
void ofApp::update(){
	//update Ableton Link
	AL_time = state.link.clock().micros();
	phase = state.link.captureAppSessionState().phaseAtTime(AL_time, quantum);
	tempo = state.link.captureAppSessionState().tempo();

	//update res state
	resState.updateTempoSpeedPhase(tempo, phase);
	resState.updateInputMessages(oscRecTouch, terminalEntries);
	resState.sendOutputMessages(oscSendRes);
	//update touch state
	touchState.updateTempoSpeedPhase(tempo, phase);
	touchState.updateInputMessages(oscRecRes);
	touchState.sendOutputMessages(oscSendTouch, resState);

	//clear terminal entries to prevent memory leak
	if (terminalEntries.size() > 1024)
		terminalEntries.clear();

}

//--------------------------------------------------------------
void ofApp::draw() {
	std::stringstream bridgeString;
	std::stringstream cuesString;
	int i_phase = phase + 1.0f;



	bridgeString << "ABLETON BRIDGE CONNECTIONS : " << state.link.numPeers()
		<< std::endl << "CURRENT BPM  : " << tempo
		<< std::endl << "CURRENT BEAT : " << i_phase;

	cuesString << "Current Col : " << resState.getCurCol()
		<< std::endl << "Current Set : " << resState.getCurSet()
		<< std::endl << "Current Cue : " << resState.getCurCue();

	gui.begin();
	if(ImGui::Begin("Bridge", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize))
	{
	ImGui::SetWindowPos({ 0, 0 });
	ImGui::SetWindowSize({ float(ofGetWindowWidth()), float(ofGetWindowHeight()) });
	
	ImGui::Text(bridgeString.str().c_str());	
	ImGui::RadioButton("One  ", i_phase == 1);
	ImGui::SameLine();
	ImGui::RadioButton("Two  ", i_phase == 2);
	ImGui::RadioButton("Four ", i_phase == 4);
	ImGui::SameLine();
	ImGui::RadioButton("Three", i_phase == 3);
	
	ImGui::NewLine();
	ImGui::Text(cuesString.str().c_str());
	
	ImGui::NewLine();
	ImGui::Text("Commands");
	ImGui::ListBoxHeader("", {390, 195});
		for(auto& item : terminalEntries)
		{
			ImGui::TextColored({1.0f, 0.6f, 0.0f, 1.0f}, item.c_str());
			ImGui::SetScrollHere();
		}
	ImGui::ListBoxFooter();
	
	}
	ImGui::End();
	gui.end();

}

void ofApp::keyPressed(int key)
{
}
