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

void TouchState::sendOutputMessages(ofxOscSender& send)
{
}
