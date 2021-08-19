#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main( ){
	ofGLFWWindowSettings settings;
	settings.setSize(420, 380);
	settings.resizable = false;
	ofCreateWindow(settings);
	
	return ofRunApp(new ofApp());

}
