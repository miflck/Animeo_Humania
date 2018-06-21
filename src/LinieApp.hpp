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
    
    void toggleMouseActive();
    
    bool repulse=true;
    
    
    ofVec2f anchorPositionTop;
    ofVec2f anchorPositionBottom;


private:
    ofxBox2d                                  box2d;   // the box2d world
    ofxBox2dCircle                            anchor;  // fixed anchor
    vector      <shared_ptr<ofxBox2dCircle> > circles; // default box2d circles
    vector      <shared_ptr<ofxBox2dJoint> >  joints;  // joints
    ofxBox2dCircle                            anchor2;  // fixed anchor
    vector      <ofVec2f>   positions;  // joints

    ofPolyline line;
    
    ofxBox2dJoint mousejoint;
    bool bIsMouseActive=false;
    
};


#endif /* LinieApp_hpp */
