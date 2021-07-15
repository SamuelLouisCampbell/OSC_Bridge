#include "ResolumeState.h"

void ResolumeState::updateInputMessages(ofxOscReceiver& recv)
{
	while (recv.hasWaitingMessages())
	{
		//Get the next message;
		ofxOscMessage m;
		recv.getNextMessage(&m);
		std::string s = m.getAddress();

		if (s == "/cue")
		{
			std::stringstream ss;
			ss << "/cue/" << m.getArgAsInt(0) << " : detected." << std::endl;
			std::cout << ss.str();
			nextCue = m.getArgAsInt(0);
		}
		else if (s == "/set")
		{
			std::stringstream ss;
			ss << "/set/" << m.getArgAsInt(0) << " : detected." << std::endl;
			std::cout << ss.str();
			nextSet = m.getArgAsInt(0);
		}
	}
}

void ResolumeState::sendOutputMessages(ofxOscSender& send)
{
	//decide which column to launch based on current data.
	if (currentSet != nextSet)
	{
		//Clip Set Changed. 
		//reset cues to 1 :: prebreak
		nextCue = 1; 
		currentCue = 1;

		//update current set
		currentSet = nextSet;

		//update actual column index
		currentCol = (currentSet * offset + (currentCue - 1)) - (offset - 1);

		//send that to RES
		std::stringstream ss;
		ss << "/composition/columns/" << currentCol << "/connect";
		ofxOscMessage m;
		m.setAddress(ss.str());
		m.addFloatArg(1.0f);
		send.sendMessage(m);
		std::cout << ss.str() << std::endl;
	}
	if (currentCue != nextCue)
	{
		//cue updated
		currentCue = nextCue;
		//update actual column index
		currentCol = (currentSet * offset + (currentCue - 1)) - (offset - 1);

		//send that to RES
		std::stringstream ss;
		ss << "/composition/columns/" << currentCol << "/connect";
		ofxOscMessage m;
		m.setAddress(ss.str());
		m.addFloatArg(1.0f);
		send.sendMessage(m);
		std::cout << ss.str() << std::endl;

	}

	if (oldTempo != tempo)
	{
		//flipflop
		oldTempo = tempo;
		//send speed
		ofxOscMessage s;
		s.setAddress("/composition/speed");
		s.addFloatArg(toSendValue);
		send.sendMessage(s);
		
		//send tempo
		ofxOscMessage t;
		t.setAddress("/composition/tempocontroller/tempo");
		t.addFloatArg(floatBPM);
		send.sendMessage(t);
	}
	/*std::stringstream ss;
	ss << "/composition/columns/" << launchers.back().getColumnToLaunch() << "/connect";
	currentColumn = launchers.back().getColumnToLaunch();
	ofxOscMessage m;
	m.setAddress(ss.str());
	m.addFloatArg(1.0f);
	send.sendMessage(m);*/
}

void ResolumeState::updateTempoSpeedPhase(const float bpm, const float _phase)
{
	tempo = bpm;
	phase = _phase;
	//convert bpm to "SPEED" 
	float convertMe = ofMap(tempo, contentTargetBPM / 2.0f, contentTargetBPM * 2.0f, 0.05f, 0.2f);
	floatBPM = ofMap(tempo, 20.0f, 500.0f, 0.0f, 1.0f);
	toSendValue = std::pow(convertMe, 1.0f / 1.6609640474437f);
	
	
}

const size_t ResolumeState::getCurSet()
{
	return currentSet;
}

const size_t ResolumeState::getCurCue()
{
	return currentCue;
}

const size_t ResolumeState::getCurCol()
{
	return currentCol;
}

void ResolumeState::setOffset(const int _offset)
{
	offset = _offset;
}
