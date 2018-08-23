//
//  LinieApp.hpp
//  appswitcher
//
//  Created by Flückiger Michael on 11.06.18.
//

#ifndef LinieApp_hpp
#define LinieApp_hpp

#include <stdio.h>
#include "ofMain.h"
#include "ApplicationBase.h"
#include "ofxBox2d.h"
#include "ofxJsonSettings.h"
#include "ofxOsc.h"
#include "MovingObject.hpp"



class LinieApp:public ApplicationBase{
public:
    LinieApp();
    virtual ~LinieApp();
    
    

    
    void turnOn();
    void turnOff();
    
    
    //structure
    void init();
    void update();
    void draw();
    void exit();
    void keyPressed(ofKeyEventArgs &e);
    
    void mouseMoved(ofMouseEventArgs &a );

     void mouseDragged(ofMouseEventArgs &a);
     void mousePressed(ofMouseEventArgs &a);
     void mouseReleased(ofMouseEventArgs &a);
     void mouseScrolled(ofMouseEventArgs &a);
     void mouseEntered(ofMouseEventArgs &a);
     void mouseExited(ofMouseEventArgs &a);

    
    
    
    //events
    bool bAddedListeners;
    bool bIsRunning;
    bool bUseHand=false;
    
    void onMessageReceived(ofxOscMessage &msg);

    
    void toggleMouseActive();
    
    bool repulse=true;
    
    
    ofVec2f anchorPositionTop;
    ofVec2f anchorPositionBottom;
    
    ofVec2f *anchorStartPositionTop;
    ofVec2f *anchorStartPositionBottom;
    
    
    ofVec2f *finalCirclePosition;

    
    
    bool bMakeWave=false;
    bool bHasStartWave=false;
    
    float waveInittime;
    float waveSpeed;
    ofVec2f waveInitPosition;
    float waveAmplitude;
    float howmany;
    int waveActuatorIndex=1;
    void wave();
    void startWave(float _speed,float _amplitude, float _howmany);
    void endWave();
    
    float waveMaxVolTime=PI;
    float maxVol;
    float maxWaveVol(float v);

    
    float OSCwaveSpeed;
    float OSCwaveAmplitude;
    float OSChowmany;
    
    float generalDamping=0.99;
    
    bool bOrganic=false;
    
    bool bMakeCircle=false;
    void makeCircle(bool _b);
    int circleBoundindex=0;
    
    int circlePositionBoundIndex=0;
    
    MovingObject mover1;
    ofVec2f moverCircleRadius;
    ofVec2f moverCircleCenter;

    float moverCircleAngle=0;
    float moverCircleSpeed=1;
    
    float damping;
    
    int numCircles=40;
    float circleAttractionForce=3;
    
    
    

    
    bool bEndCircle=false;
    float endCircleRadius;
    // circle easing
    float initAlpha;
    float actualAlpha;
    float alphaTarget;
    float alphaEasingInitTime;
    float alphaEasingDuration;
    
    void startFadeEndCircle();
    bool bStartFade=false;
    
    
private:
    ofxBox2d                                  box2d;   // the box2d world
    ofxBox2dCircle                            anchor;  // fixed anchor
    vector      <shared_ptr<ofxBox2dCircle> > circles; // default box2d circles
    vector      <shared_ptr<ofxBox2dJoint> >  joints;  // joints
    
    vector      <shared_ptr<ofxBox2dJoint> >  circlejoints;  // joints

    
    ofxBox2dCircle                            anchor2;  // fixed anchor
    vector      <ofVec2f>   positions;  // joints

    ofPolyline line;
    
    ofxBox2dJoint mousejoint;
    bool bIsMouseActive=false;
    
};


#endif /* LinieApp_hpp */
