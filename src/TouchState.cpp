#include "TouchState.h"

void TouchState::updateInputMessages(ofxOscReceiver& recv)
{
	while (recv.hasWaitingMessages())
	{
		//Get the next message;
		ofxOscMessage m;
		recv.getNextMessage(&m);
		std::string s = m.getAddress();

		if (s == "/composition/layers/1/clips/1/name")
		{
			std::stringstream ss;
			ss << "clip name" << m.getArgAsString(0) << " : detected.... " << std::endl;
			std::cout << ss.str();
			label = m.getArgAsString(0);
		}
	}


}

void TouchState::sendOutputMessages(ofxOscSender& send, ResolumeState& rs)
{
	//flipflop
	if (oldTempo != tempo)
	{
		oldTempo = tempo;
		//send tempo
		std::stringstream ss;
		ss << "BPM : " << std::setprecision(5) << tempo;
		ofxOscMessage t;
		t.setAddress("/tempo");
		t.addStringArg(ss.str());
		send.sendMessage(t);
	}
	if (oldPhase != phase)
	{
		oldPhase = phase;
		int i_phase = phase + 1.0f;
		std::stringstream ss;
		ss << "BEAT : " << i_phase;
		ofxOscMessage t;
		t.setAddress("/phase");
		t.addStringArg(ss.str());
		send.sendMessage(t);
	}

	if (rs.waitingForGo())
	{
		ofxOscMessage t;
		t.setAddress("/setcue");
		t.addFloatArg(1.0f);
		send.sendMessage(t);

	}
	else
	{
		//TODO fix this so it doesn't send an instruction each frame.
		ofxOscMessage t;
		t.setAddress("/setcue");
		t.addFloatArg(0.0f);
		send.sendMessage(t);
	}

	if (rs.getCurCol() != OldResClip)
	{
		OldResClip = rs.getCurCol();
		ofxOscMessage t;
		t.setAddress("/setcue");
		t.addFloatArg(0.0f);
		send.sendMessage(t);
	}
}

void TouchState::updateTempoSpeedPhase(const float bpm, const float _phase)
{
	tempo = bpm;
	phase = _phase;
}
