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
#include "helper.hpp"



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
    MovingObject headMover;

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
    
    
    MovingObject leftArmCP1Mover;
    MovingObject leftArmCP2Mover;
    
    MovingObject rightArmCP1Mover;
    MovingObject rightArmCP2Mover;
    
    MovingObject spineCP1Mover;
    MovingObject spineCP2Mover;
    
    MovingObject leftLegCP1Mover;
    MovingObject leftLegCP2Mover;
    
    MovingObject rightLegCP1Mover;
    MovingObject rightLegCP2Mover;
    
    vector<MovingObject*>movers;
    

    
    MovingObject absolutePosition;
    
    
    float scaler=-200;

    // Skeletton choose
    int skelettonId=0;
    void setSkelettonId(int id);
    int cycleSkelettonId();
    
    
    
    bool bRecord=false;
    bool bPlay=false;
    
    void startRecording();
    void stopRecording();
    
    
    void startPlayback();
    void stopPlayback();
    
    vector<MappedPoints> recordedBonesPositions;
    vector<ofVec2f> recordedAvatarPositions;
    int playhead=0;

    
    
private:
   
};


#endif /* Avatar_hpp */
