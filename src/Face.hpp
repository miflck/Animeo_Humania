//
//  Physics.hpp
//  appswitcher
//
//  Created by Flückiger Michael on 11.06.18.
//

#ifndef Face_hpp
#define Face_hpp

#include <stdio.h>
#include "ofMain.h"
#include "ApplicationBase.h"
#include "ofxBox2d.h"
#include "MovingObject.hpp"



class Face:public ApplicationBase{
public:
   Face();
    virtual ~Face();
    
    

    
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
    
    void toggleMouseActive();
    ofFbo screen;
    
private:
    ofxBox2d                                  box2d;   // the box2d world
    ofxBox2dCircle                            anchor;  // fixed anchor
    vector      <shared_ptr<ofxBox2dCircle> > circles; // default box2d circles
    vector      <shared_ptr<ofxBox2dJoint> >  joints;  // joints
    vector    <shared_ptr<ofxBox2dRect> >   boxes;           // default box2d rects
    ofxBox2dRect box;
    

    ofPolyline line;
    
    ofxBox2dJoint mousejoint;
    bool bIsMouseActive=false;

    //FACE
    ofVec2f facePosition;
    ofVec2f eyePosition;
    ofVec2f eyeOffset;
    ofVec2f leftMouth;
    ofVec2f rightMouth;
    ofVec2f mouthCenterPosition;

    ofVec2f leftMouthOffset;
    ofVec2f rightMouthOffset;
    
    ofVec2f leftMouthCornerOffset;
    ofVec2f rightMouthCornerOffset;
    
    ofPath mouth;



    // Skeletton
    ofVec2f headPosition;
    ofVec2f leftHandPosition;
    ofVec2f rightHandPosition;
    ofVec2f spineBasePosition;
    ofVec2f spineMidPosition;
    ofVec2f leftShoulderPosition;
    ofVec2f rightShoulderPosition;
    ofVec2f leftEllbowPositon;
    ofVec2f rightEllbowPositon;


    bool bHasEyes=false;
    bool bEyeReactive=false;
    
    bool bHasFace=false;
    bool bHasMouth=false;

    
};


#endif /* Physics_hpp */
