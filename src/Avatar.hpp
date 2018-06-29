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
    
    
    ofVec2f leftHand;
    ofVec2f leftShoulder;
    ofVec2f leftEllbow;
    ofVec2f leftWrist;
    ofVec2f leftHip;
    ofVec2f leftKnee;
    ofVec2f leftAnkle;
    ofVec2f leftFoot;
    
    ofVec2f rightHand;
    ofVec2f rightShoulder;
    ofVec2f rightEllbow;
    ofVec2f rightWrist;
    ofVec2f rightHip;
    ofVec2f rightKnee;
    ofVec2f rightAnkle;
    ofVec2f rightFoot;
    
    ofVec2f head;
    ofVec2f neck;
    ofVec2f spineBase;
    ofVec2f spineMid;
    
    
    void drawAvatar();
    
    ofVec2f headOffset;
    
    MovingObject leftHandMover;
    MovingObject leftEllbowMover;
    MovingObject neckMover;
    
    MovingObject rightHandMover;
    MovingObject rightEllbowMover;

    MovingObject spineBaseMover;
    MovingObject leftKneeMover;
     MovingObject leftFootMover;
    
    MovingObject rightKneeMover;
    MovingObject rightFootMover;
    
    vector<MovingObject*>movers;
    
    
    
    
private:
   
};


#endif /* Avatar_hpp */
