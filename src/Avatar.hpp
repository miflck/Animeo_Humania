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
    
    
    virtual void setup();
    virtual void update();
    
    virtual void updateSkeletton();
    virtual void updateSkelettonMovers();
    virtual void updateBonesPositions();

    virtual void draw();
    virtual void drawAvatar();
    
    MappedPoints getBonesPositions();
    virtual void setSiblingBones(MappedPoints _bones);

    
    
    void setBoneMoverSpeed(float _speed);
    
    //void setSibling(Avatar *a);


    // draw stuff
    int avatarLineWidth=10;
    float scaler=-200;

    // Skeletton choose
    int skelettonId=0;
    void setSkelettonId(int id);
    int cycleSkelettonId();

    // RECORDING
    bool bRecord=false;
    bool bPlay=false;
    
    
    
    void startImitate();
    void stopImitate();
    void imitate();
    bool bIsImitating=false;
    
    void startRecording();
    void stopRecording();
    
    void startPlayback();
    void stopPlayback();
    void play();
    
    vector<MappedPoints> recordedBonesPositions;
    
    vector<MappedPoints> bonesHistory;
    float historyLength=500;
    bool bRecordHistory=false;
    
    vector<ofVec2f> recordedAvatarPositions;
    int playhead=0;

    
    
    bool bIsBound=false;
    void bindSkeletton(bool _b);
    
    
protected:
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
    
    ofVec2f headP;
    ofVec2f neckP;
    ofVec2f spineBaseP;
    ofVec2f leftEllbowP;
    ofVec2f leftHandP;
    ofVec2f rightEllbowP;
    ofVec2f rightHandP;
    ofVec2f rightKneeP;
    ofVec2f leftKneeP;
    ofVec2f rightFootP;
    ofVec2f leftFootP;
    
    ofVec2f headOffset;
    
    
    //MovingObject absolutePosition;

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
    
    MappedPoints actualBonesPositions;
    MappedPoints siblingBonesPositions;
    
    


   
};


#endif /* Avatar_hpp */
