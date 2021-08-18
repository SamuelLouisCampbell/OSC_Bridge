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
	resState.updateInputMessages(oscRecTouch, terminalEntries);
	resState.sendOutputMessages(oscSendRes);
	//update touch state
	touchState.updateTempoSpeedPhase(tempo, phase);
	touchState.updateInputMessages(oscRecRes);
	touchState.sendOutputMessages(oscSendTouch);

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
	//if (ImGui::BeginChild("Bridge", {0,0}, false, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar))
	//{
		
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
		ImGui::ListBoxHeader("");
			for(auto& item : terminalEntries)
			{
				ImGui::TextColored({1.0f, 0.6f, 0.0f, 1.0f}, item.c_str());
				ImGui::SetScrollHere();
			}
		ImGui::ListBoxFooter();
		//ImGui::EndChild();
	}
	ImGui::End();
	gui.end();

}

void ofApp::keyPressed(int key)
{
}
