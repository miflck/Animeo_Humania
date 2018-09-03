#pragma once

#include "ofMain.h"
#include "GuiApp.h"
#include "ofxJsonSettings.h"
#include "ofxOsc.h"




class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
    shared_ptr<GuiApp> gui;
    
    
    void audioIn(float * input, int bufferSize, int nChannels);

    vector <float> left;
    vector <float> right;
    vector <float> volHistory;
    int     bufferCounter;
    int     drawCounter;
    float smoothedVol;
    float scaledVol;
    
    ofSoundStream soundStream;

    ofColor maskcolor;

    bool bGreenScreen=true;
    
    
    void onMessageReceived(ofxOscMessage &msg);

    
    ofVec2f *savedKinectPosition;
    ofVec2f *savedBeamerPosition;
    float  *kinectScaleFact;
    float  *beamerFOV;


    void loadSettings();
    void saveSettings();


    
    ofVec2f *leftpos;
    ofVec2f *rightpos;


    
};
