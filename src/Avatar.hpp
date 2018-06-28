//
//  MovingObject.hpp
//  moving object
//
//  Created by Flückiger Michael on 08.05.18.
//

#ifndef Avatar_hpp
#define Avatar_hpp
#include "ofMain.h"
#include <stdio.h>
#include "MovingObject.hpp"



class Avatar:public MovingObject {
    
    
    
    
    
    
public:
    Avatar();
    virtual ~Avatar();
    
    
    void setup();
    void update();
    void draw();
    
    int strokeWeight=6;
    
    
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
    
    
    void drawAvatar();
    
    ofVec2f headOffset;
    
    
private:
   
};


#endif /* Avatar_hpp */
