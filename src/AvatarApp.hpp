//
//  Physics.hpp
//  appswitcher
//
//  Created by Flückiger Michael on 11.06.18.
//

#ifndef AvatarApp_hpp
#define AvatarApp_hpp

#include <stdio.h>
#include "ofMain.h"
#include "ApplicationBase.h"
#include "ofxBox2d.h"
#include "MovingObject.hpp"
#include "Avatar.hpp"



class AvatarApp:public ApplicationBase{
public:
   AvatarApp();
    virtual ~AvatarApp();
    
    

    
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

    //AvatarApp
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
    
    
    MovingObject leftHandMover;

    ofVec3f leftHand;
    ofVec3f leftShoulder;
    ofVec3f leftEllbow;
    ofVec3f leftWrist;
    ofVec3f leftHip;
    ofVec3f leftKnee;
    ofVec3f leftAnkle;
    ofVec3f leftFoot;
    
    ofVec3f rightHand;
    ofVec3f rightShoulder;
    ofVec3f rightEllbow;
    ofVec3f rightWrist;
    ofVec3f rightHip;
    ofVec3f rightKnee;
    ofVec3f rightAnkle;
    ofVec3f rightFoot;
    
    ofVec3f head;
    ofVec3f neck;
    ofVec3f spineBase;
    ofVec3f spineMid;
    
    
    
    
    
    
    
    
    
    ofPath mouth;



    // Skeletton
   /* ofVec2f headPosition;
    ofVec2f leftHandPosition;
    ofVec2f rightHandPosition;
    ofVec2f spineBasePosition;
    ofVec2f spineMidPosition;
    ofVec2f leftShoulderPosition;
    ofVec2f rightShoulderPosition;
    ofVec2f leftEllbowPositon;
    ofVec2f rightEllbowPositon;
    */


    bool bHasEyes=false;
    bool bEyeReactive=false;
    
    bool bHasFace=false;
    bool bHasMouth=false;
    
    Avatar avatar;
    
    vector<Avatar>avatars;

    
};


#endif /* Physics_hpp */
