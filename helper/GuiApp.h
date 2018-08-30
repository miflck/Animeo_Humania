#pragma once

#include "ofMain.h"
#include "ofxGui.h"

class GuiApp: public ofBaseApp {
public:
	void setup();
	void update();
	void draw();

	ofParameterGroup parameters;
    
	ofParameter<float> radius;
    
    ofParameter<ofVec3f>kinectPosition;
    ofParameter<ofVec3f>beamerPosition;
    ofParameter<float>beamerFov;
    
    ofParameter<ofVec3f>testPosition;
    ofParameter<float>kinectscalefact;
    ofParameter<ofVec3f>rayPosition;

    ofParameter<bool> kinectManagerDebug;
    

    
    ofParameterGroup box2dbasics;

    ofParameter<float>emotionsgravity;
    ofParameter<float>drag;

    
    
    

    
	ofParameter<ofColor> color;
	ofxPanel gui;
    
    
    ofTrueTypeFont    font;


    

    
};

