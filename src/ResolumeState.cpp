#include "ResolumeState.h"


void ResolumeState::updateInputMessages(ofxOscReceiver& recv, std::vector<std::string>& terminalEntries)
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
			nextCue = m.getArgAsInt(0);
			terminalEntries.push_back(ss.str());
		}
		else if (s == "/set")
		{
			std::stringstream ss;
			ss << "/set/" << m.getArgAsInt(0) << " : detected." << std::endl;
			nextSet = m.getArgAsInt(0);
			terminalEntries.push_back(ss.str());
		}
		else if (s == "/go")
		{
			waitForGo = false;
			std::stringstream ss;
			ss << "/go : detected." << std::endl;
			terminalEntries.push_back(ss.str());
			if (nextCue < nCues)
			{
				nextCue++;
			}
			else
			{
				nextCue = 1;
			}
		}
		else if (s == "/kill")
		{
			waitForGo = false;
			std::stringstream ss;
			ss << "/kill : detected." << std::endl;
			killContentLayers = true;
			terminalEntries.push_back(ss.str());
		}
		else if (s == "/setcue")
		{
			if (waitLatch == false)
			{
				waitForGo = true;
				waitLatch = true;
			}
			std::stringstream ss;
			ss << "/setcue : detected." << std::endl;
			terminalEntries.push_back(ss.str());
		}
		else
		{
			//Some messags are simply passed on
			//print out what they are for terminal
			std::stringstream ss;
			ss << "Passing through this message : " << m.getAddress() << std::endl;
			terminalEntries.push_back(ss.str());
			//add them to the msg q
			messageQ.push_back(m);
		}

	}
}

void ResolumeState::sendOutputMessages(ofxOscSender& send)
{
	//decide which column to launch based on current data.
	if (phase < 0.0625f && !waitForGo)
	{
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

	if (killContentLayers)
	{
		for(int i = 0; i < killLayers.size(); i++)
		{
			std::stringstream ss; 
			ss << "/composition/layers/" << killLayers[i] << "/clear";
			ofxOscMessage s;
			s.setAddress(ss.str());
			s.addIntArg(1);
			send.sendMessage(s);
		}
		for (int i = 0; i < killLayers.size(); i++)
		{
			std::stringstream ss;
			ss << "/composition/layers/" << killLayers[i] << "/clear";
			ofxOscMessage s;
			s.setAddress(ss.str());
			s.addIntArg(0);
			send.sendMessage(s);
		}
		killContent();
	}

	//send any pass through messages;
	while (!messageQ.empty())
	{
		send.sendMessage(messageQ.front());
		messageQ.pop_front();
	}
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

void ResolumeState::AddKillLayer(const size_t layerNo)
{
	killLayers.push_back(layerNo);
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

const bool ResolumeState::killContent()
{
	bool temp = killContentLayers;
	killContentLayers = false;
	return temp;
}

void ResolumeState::setOffset(const int _offset)
{
	offset = _offset;
}

const bool ResolumeState::waitingForGo()
{
	return waitForGo;
}

void ResolumeState::setTargetBPM(const float target)
{
	contentTargetBPM = target;
}
