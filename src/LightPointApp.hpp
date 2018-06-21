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
    
    
    //structure
    void init();
    void update();
    void draw();
    void exit();

    
    //events
    bool bAddedListeners;
    bool bIsRunning;
    
    bool bSendOSCPosition=false;
    int m8layer=1;
    
    BreathPoint mover;
    
    int skelettId=0;

    int skelettonNodeId=8;
    
    ofFbo screen;

    bool bRecord=false;
    bool bRepusion=false;
    bool bSlowDown=true;
    void toggleRepulsion();
    
    ofVec2f homeposition;
    
   void onMessageReceived(ofxOscMessage &msg);
    
    

    
};


#endif /* LightPointApp_hpp */
