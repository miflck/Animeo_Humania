//
//  LightPointApp.hpp
//  appswitcher
//
//  Created by Flückiger Michael on 11.06.18.
//

#ifndef LightPointApp_hpp
#define LightPointApp_hpp

#include <stdio.h>
#include "ofMain.h"
#include "ApplicationBase.h"
#include "BreathPoint.hpp"
#include "ofxOsc.h"

#include "ofxGui.h"


#include "ofxJsonSettings.h"


#define  BOUNCE 1
#define ENTER 2
#define LEAVE 3


class LightPointApp:public ApplicationBase{
public:
    LightPointApp();
    virtual ~LightPointApp();
    
    
    void keyPressed(ofKeyEventArgs &e);
    void mouseMoved(ofMouseEventArgs &a );
    void mouseDragged(ofMouseEventArgs &a);
    void mousePressed(ofMouseEventArgs &a);
    void mouseReleased(ofMouseEventArgs &a);
    void mouseScrolled(ofMouseEventArgs &a);
    void mouseEntered(ofMouseEventArgs &a);
    void mouseExited(ofMouseEventArgs &a);

    
    void turnOn();
    void turnOff();
    void onSettingsLoaded();

    
    
    //structure
    void init();
    void update();
    void draw();
    void exit();

    
    
    void setMoverToStartPosition();
    void goHome();
    
    
    // states
    int state=0;
    int stateBefore=0;
    void switchState(int _newstate);
    
    //events
    bool bAddedListeners;
    bool bIsRunning;
    
    bool bSendOSCPosition=false;
    int m8layer=1;
    
    BreathPoint mover;
    
    int skelettId=0;

    int skelettonNodeId=2;
    
    int seekTargetId=0;
    
    ofFbo screen;

    bool bRecord=false;
    bool bRepusion=false;
    bool bSlowDown=true;
    void toggleRepulsion();
    
    
    ofVec2f *homeposition;
    ofVec2f *startposition;

    
    
    void bounceFromWalls();
    
    bool bounceFromCabin();
    bool bBounceFromCabin=true;
    ofVec2f *cabinposition;
    ofVec2f *cabindimension;
    ofVec2f lastPosition;
    ofRectangle cabinRect;
    
    bool goInside=false;


    

    void onOSCSetup(ofxOscMessage &msg);

    
    
   void onMessageReceived(ofxOscMessage &msg);
    
    

    
};


#endif /* LightPointApp_hpp */
