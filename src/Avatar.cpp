//
//  MovingObject.cpp
//  moving object
//
//  Created by Flückiger Michael on 08.05.18.
//

#include "Avatar.hpp"
#include "ofxEasing.h"
#include "KinectV2Manager.hpp"
#include "ApplicationController.h"
#include "ofxPoly.h"


Avatar::Avatar(){
    
}

Avatar::~Avatar(){
    
}

void Avatar::setup(){
    bSeekTarget=true;
    setTarget(ofVec2f(ofGetWidth()/2,ofGetHeight()/2));
    setSeekForce(50);
    
   // movers.push_back(&absolutePosition);
    movers.push_back(&leftHandMover);
    movers.push_back(&leftEllbowMover);
    movers.push_back(&neckMover);
    movers.push_back(&rightHandMover);
    movers.push_back(&rightEllbowMover);
    movers.push_back(&spineBaseMover);
    movers.push_back(&headMover);
    movers.push_back(&leftKneeMover);
    movers.push_back(&rightKneeMover);
    movers.push_back(&leftFootMover);
    movers.push_back(&rightFootMover);
    
    movers.push_back(&leftArmCP1Mover);
    movers.push_back(&leftArmCP2Mover);
    movers.push_back(&rightArmCP1Mover);
    movers.push_back(&rightArmCP2Mover);
    movers.push_back(&rightLegCP1Mover);
    movers.push_back(&rightLegCP2Mover);
    movers.push_back(&leftLegCP1Mover);
    movers.push_back(&leftLegCP2Mover);
    movers.push_back(&spineCP1Mover);
    movers.push_back(&spineCP2Mover);


    for(int i=0;i<movers.size();i++){
        movers[i]->bSeekTarget=true;
        movers[i]->setSeekForce(20);
        movers[i]->setMaxSpeed(150);
            //movers[i]->setSlowDown(false);
    }
    
    spineCP1Mover.setMaxSpeed(60);
    spineCP2Mover.setMaxSpeed(60);
    
    leftArmCP1Mover.setMaxSpeed(60);
    leftArmCP2Mover.setMaxSpeed(60);
    
    rightArmCP1Mover.setMaxSpeed(60);
    rightArmCP2Mover.setMaxSpeed(60);
    
    rightLegCP1Mover.setMaxSpeed(60);
    rightLegCP2Mover.setMaxSpeed(60);
    
    leftLegCP1Mover.setMaxSpeed(60);
    leftLegCP2Mover.setMaxSpeed(60);
    
    
    
    ofVec2f headCenter=head+headOffset;
    leftEyePosition.set(headCenter.x-20,headCenter.y-10);
    rightEyePosition.set(headCenter.x-20,headCenter.y-10);
    
    
    
}

void Avatar::setBoneMoverSpeed(float _speed){
    
    for(int i=0;i<movers.size();i++){
        movers[i]->bSeekTarget=true;
        movers[i]->setSeekForce(20);
        movers[i]->setMaxSpeed(_speed);
        //movers[i]->setSlowDown(false);
    }
    
    spineCP1Mover.setMaxSpeed(_speed/2);
    spineCP2Mover.setMaxSpeed(_speed/2);
    
    leftArmCP1Mover.setMaxSpeed(_speed/2);
    leftArmCP2Mover.setMaxSpeed(_speed/2);
    
    rightArmCP1Mover.setMaxSpeed(_speed/2);
    rightArmCP2Mover.setMaxSpeed(_speed/2);
    
    rightLegCP1Mover.setMaxSpeed(_speed/2);
    rightLegCP2Mover.setMaxSpeed(_speed/2);
    
    leftLegCP1Mover.setMaxSpeed(_speed/2);
    leftLegCP2Mover.setMaxSpeed(_speed/2);
    
}

void Avatar::update(){
    
    for(int i=0;i<movers.size();i++){
        movers[i]->move();
    }
    updateSkeletton();
    play();
    imitate();
    updateSkelettonMovers();
    updateBonesPositions();
    move();
   // auto endTime = easingInitTime + scaleDuration;
   // auto now = ofGetElapsedTimef();
   // actualRadius = ofxeasing::map_clamp(now, easingInitTime, endTime, actualRadius, radiusTarget, &ofxeasing::linear::easeIn);
}

void Avatar::draw(){
    
    ofPushMatrix();
    ofPushStyle();
    if(bPlay){
        ofTranslate(recordedAvatarPositions[playhead]);
    }else{
        ofTranslate(getPosition());
    }
    drawAvatar();
    ofPopStyle();
    ofPopMatrix();
}













void Avatar::setSkelettonId(int id){
    skelettonId= id;
}

int Avatar::cycleSkelettonId(){
    vector<MappedPoints> mskel=KINECTMANAGER->getMappedSkelettons();

    int actId=skelettonId;
    int tempId=0;
    if(mskel.size()>actId+1){
        tempId=actId+1;
    }
    setSkelettonId(tempId);
}

void Avatar::startRecording(){
    stopPlayback();
    recordedBonesPositions.clear();
    recordedAvatarPositions.clear();
    bRecord=true;
}
void Avatar::stopRecording(){
    bRecord=false;

}
void Avatar::startPlayback(){
    if(recordedBonesPositions.size()>0){
        bPlay=true;
        playhead=0;
        
    }
}
void Avatar::stopPlayback(){
    bPlay=false;
}


void Avatar::startImitate(){
    bIsImitating=true;
}
void Avatar::stopImitate(){
    bIsImitating=false;
}

void Avatar::updateSkeletton(){
    vector<MappedPoints> mskel=KINECTMANAGER->getMappedSkelettons();
    
    if(mskel.size()>0&&bRecord){
        recordedBonesPositions.push_back(mskel[skelettonId]);
        recordedAvatarPositions.push_back(getPosition());
    }
    
    if(mskel.size()>0 && bRecordHistory){
        bonesHistory.push_back(mskel[skelettonId]);
        if( bonesHistory.size() > historyLength){
         bonesHistory.erase(bonesHistory.begin(), bonesHistory.begin()+1);
        }
    }
    
    headP=ofVec2f(0,0);
//    neckP=head+ofVec2f(0,80);
    neckP=head+ofVec2f(0,0);

    spineBaseP=neck+ofVec2f(0,120);
    leftEllbowP=neck+ofVec2f(60,60);
    leftHandP=leftEllbow+ofVec2f(60,-60);
    rightEllbowP=neck+ofVec2f(-60,60);
    rightHandP=rightEllbow+ofVec2f(0,60);
    rightKneeP=spineBase+ofVec2f(-30,60);
    leftKneeP=spineBase+ofVec2f(30,60);
    rightFootP=rightKnee+ofVec2f(10,60);
    leftFootP=leftKnee+ofVec2f(-10,60);
    
    if(mskel.size()>0 && bIsBound){
        headP=ofVec2f(mskel[skelettonId].neckLocal*scaler);
        neckP=mskel[skelettonId].neckLocal*scaler;
        spineBaseP=mskel[skelettonId].spineBaseLocal*scaler;
        leftEllbowP=mskel[skelettonId].leftEllbowLocal*1.5*scaler;
        leftHandP=mskel[skelettonId].leftHandLocal*1.5*scaler;
        rightEllbowP=mskel[skelettonId].rightEllbowLocal*1.5*scaler;
        rightHandP=mskel[skelettonId].rightHandLocal*1.5*scaler;
        rightKneeP=mskel[skelettonId].rightKneeLocal*1.5*scaler;
        leftKneeP=mskel[skelettonId].leftKneeLocal*1.5*scaler;
        rightFootP=mskel[skelettonId].rightFootLocal*1.5*scaler;
        leftFootP=mskel[skelettonId].leftFootLocal*1.5*scaler;
    }
}


void Avatar::play(){
    if(bPlay){
        headP=ofVec2f(recordedBonesPositions[playhead].neckLocal*scaler);
        neckP=recordedBonesPositions[playhead].neckLocal*scaler;
        spineBaseP=recordedBonesPositions[playhead].spineBaseLocal*scaler;
        leftEllbowP=recordedBonesPositions[playhead].leftEllbowLocal*1.5*scaler;
        leftHandP=recordedBonesPositions[playhead].leftHandLocal*1.5*scaler;
        rightEllbowP=recordedBonesPositions[playhead].rightEllbowLocal*1.5*scaler;
        rightHandP=recordedBonesPositions[playhead].rightHandLocal*1.5*scaler;
        rightKneeP=recordedBonesPositions[playhead].rightKneeLocal*1.5*scaler;
        leftKneeP=recordedBonesPositions[playhead].leftKneeLocal*1.5*scaler;
        rightFootP=recordedBonesPositions[playhead].rightFootLocal*1.5*scaler;
        leftFootP=recordedBonesPositions[playhead].leftFootLocal*1.5*scaler;
        
        // update Playhead
        playhead=(playhead+1) % (recordedBonesPositions.size()-1);
    }
    
}


void Avatar::imitate(){
    if(bIsImitating){
        headP=ofVec2f(siblingBonesPositions.neckLocal);
        neckP=siblingBonesPositions.neckLocal;
        spineBaseP=siblingBonesPositions.spineBaseLocal;
        leftEllbowP=siblingBonesPositions.leftEllbowLocal;
        leftHandP=siblingBonesPositions.leftHandLocal;
        rightEllbowP=siblingBonesPositions.rightEllbowLocal;
        rightHandP=siblingBonesPositions.rightHandLocal;
        rightKneeP=siblingBonesPositions.rightKneeLocal;
        leftKneeP=siblingBonesPositions.leftKneeLocal;
        rightFootP=siblingBonesPositions.rightFootLocal;
        leftFootP=siblingBonesPositions.leftFootLocal;
    }
    
}

void Avatar::updateBonesPositions(){
    actualBonesPositions.neckLocal=neck;
    actualBonesPositions.spineBaseLocal=spineBase;
    actualBonesPositions.leftEllbowLocal=leftEllbow;
    actualBonesPositions.leftHandLocal=leftHand;
    actualBonesPositions.rightEllbowLocal=rightEllbow;
    actualBonesPositions.rightHandLocal=rightHand;
    actualBonesPositions.rightKneeLocal=rightKnee;
    actualBonesPositions.leftKneeLocal=leftKnee;
    actualBonesPositions.rightFootLocal=rightFoot;
    actualBonesPositions.leftFootLocal=leftFoot;
}

MappedPoints Avatar::getBonesPositions(){
    return actualBonesPositions;
}

void Avatar::setSiblingBones(MappedPoints _bones){
    siblingBonesPositions=_bones;
}

void Avatar::updateSkelettonMovers(){
    headMover.setTarget(headP);
    head=headMover.getPosition();
    neckMover.setTarget(neckP);
    neck=neckMover.getPosition();
    spineBaseMover.setTarget(spineBaseP);
    spineBase=spineBaseMover.getPosition();
    
    leftEllbowMover.setTarget(leftEllbowP);
    leftEllbow=leftEllbowMover.getPosition();
    
    leftHandMover.setTarget(leftHandP);
    leftHand=leftHandMover.getPosition();
    
    rightEllbowMover.setTarget(rightEllbowP);
    rightEllbow=rightEllbowMover.getPosition();
    
    rightHandMover.setTarget(rightHandP);
    rightHand=rightHandMover.getPosition();
    
    rightKneeMover.setTarget(rightKneeP);
    rightKnee=rightKneeMover.getPosition();
    
    leftKneeMover.setTarget(leftKneeP);
    leftKnee=leftKneeMover.getPosition();
    
    rightFootMover.setTarget(rightFootP);
    rightFoot=rightFootMover.getPosition();
    
    leftFootMover.setTarget(leftFootP);
    leftFoot=leftFootMover.getPosition();
    
    
    
    
    
    
    
    //ARM Mover
    ofVec2f mid=leftEllbow-neck;
    ofVec2f cp1=mid;//.getRotated(-40);
    cp1*=0.7;
    cp1+=neck;
    leftArmCP1Mover.setTarget(cp1);
    
    mid=leftEllbow-leftHand;
    ofVec2f cp2=mid;//.getRotated(40);
    cp2*=0.7;
    cp2+=leftHand;
    leftArmCP2Mover.setTarget(cp2);
    
    
    mid=rightEllbow-neck;
    cp1=mid;//.getRotated(-40);
    cp1*=0.7;
    cp1+=neck;
    rightArmCP1Mover.setTarget(cp1);
    
    mid=rightEllbow-rightHand;
    cp2=mid;//.getRotated(40);
    cp2*=0.57;
    cp2+=rightHand;
    rightArmCP2Mover.setTarget(cp2);
    
    
    // Leg Bezier
    mid=rightKnee-spineBase;
    cp1=mid;//.getRotated(-40);
    cp1*=0.75;
    cp1+=spineBase;
    rightLegCP1Mover.setTarget(cp1);
    
    mid=rightKnee-rightFoot;
    cp2=mid;//.getRotated(40);
    cp2*=0.75;
    cp2+=rightFoot;
    rightLegCP2Mover.setTarget(cp2);
    
    
    mid=leftKnee-spineBase;
    cp1=mid;//.getRotated(-40);
    cp1*=0.75;
    cp1+=spineBase;
    leftLegCP1Mover.setTarget(cp1);
    
    mid=leftKnee-leftFoot;
    cp2=mid;//.getRotated(40);
    cp2*=0.75;
    cp2+=leftFoot;
    leftLegCP2Mover.setTarget(cp1);
    
    
    float rotation=40;
    if(head.x-spineBase.x>0){
        rotation=-40;
    }else{
        rotation=40;
    }
    
    //SPine
    mid=spineBase-neck;
    cp1=mid.getRotated(rotation);
    cp1/=3;
    cp1+=neck;
    spineCP1Mover.setTarget(cp1);
    
    
    cp2=mid.getRotated(-rotation);
    cp2/=-3;
    cp2+=spineBase;
    spineCP2Mover.setTarget(cp2);
    
    
}


void Avatar::bindSkeletton(bool _b){
    bIsBound=_b;
}

void Avatar::drawAvatar(){
    //headOffset=ofVec2f(0,-100);
    headOffset=ofVec2f(0,-80);

    vector<MappedPoints> mskel=KINECTMANAGER->getMappedSkelettons();
    
    
    ofPushMatrix();
    ofPushStyle();
    
    
    ofVec2f headCenter=head+headOffset;

    ofSetColor(255);
    ofDrawCircle(headCenter,80);
    ofSetColor(0);
    
    // EYES
   // ofDrawCircle(headCenter.x-20,headCenter.y-10,10);
   // ofDrawCircle(headCenter.x+20,headCenter.y-10,10);
    ofPushMatrix();
    ofTranslate(headCenter);
    ofDrawCircle(leftEyePosition,10);
    ofDrawCircle(rightEyePosition,10);
    ofPopMatrix();

    //Cheeks
    ofSetColor(255,0,0);
    ofDrawCircle(headCenter.x-50,headCenter.y+30,20);
    ofDrawCircle(headCenter.x+50,headCenter.y+30,20);
    ofSetColor(0);
    ofDrawTriangle(headCenter.x, headCenter.y, headCenter.x+20, headCenter.y+40,headCenter.x-20, headCenter.y+40);
    // Hair
    ofSetColor(0);
    ofNoFill();
    ofSetLineWidth(3);
    ofDrawBezier(headCenter.x,headCenter.y-80,headCenter.x,headCenter.y-40,headCenter.x-40,headCenter.y-20,headCenter.x-80,headCenter.y-20);
    ofDrawBezier(headCenter.x,headCenter.y-80,headCenter.x,headCenter.y-40,headCenter.x+40,headCenter.y-20,headCenter.x+80,headCenter.y-20);
    
    ofSetColor(255);
    ofFill();
    
    
    
    ofPushMatrix();
    ofTranslate(headCenter);
    ofVec2f dist=(leftMouthPosition-mouthCenterPosition);
    ofVec2f cp1=ofVec2f(leftMouthPosition.x,leftMouthPosition.y-dist.y/2);
    ofVec2f cp2=ofVec2f(mouthCenterPosition.x+(dist.x/2),mouthCenterPosition.y);
    ofSetColor(0);
    
    ofPolyline rough;
    ofMesh smooth;
    
    rough.clear();
    smooth.clear();
    rough.addVertex(leftMouthPosition);
    rough.bezierTo(cp1,cp2, mouthCenterPosition);
    ofxPolyToMesh(smooth, rough, 2);
    smooth.draw();
    
    
    dist=(rightMouthPosition-mouthCenterPosition);
    cp1=ofVec2f(rightMouthPosition.x,rightMouthPosition.y-dist.y/2);
    cp2=ofVec2f(mouthCenterPosition.x+(dist.x/2),mouthCenterPosition.y);
    
    rough.clear();
    smooth.clear();
    rough.addVertex(rightMouthPosition);
    rough.bezierTo(cp1,cp2, mouthCenterPosition);
    ofxPolyToMesh(smooth, rough, 2);
    smooth.draw();
    ofPopMatrix();
    
    
    
    
    
    
    
     dist=ofVec2f(80,0);
    dist.rotate(-20);
    for(int i=0;i<6;i++){
        ofVec2f pos=headCenter;
        pos+=dist;
        ofSetColor(0);
        ofDrawCircle(pos,20);
        ofSetColor(255);
        ofDrawCircle(pos,17);
        dist.rotate(-180/6);
    }
    
    
    
    ofSetLineWidth(avatarLineWidth);
    
    float rotation=40;
    if(head.x-spineBase.x>0){
        rotation=-40;
    }else{
        rotation=40;
    }
    
    ofVec2f mid=spineBase-neck;
     cp1=mid.getRotated(rotation);
    cp1/=3;
    cp1+=neck;
    
     cp2=mid.getRotated(-rotation);
    cp2/=-3;
    cp2+=spineBase;
    
    
    
    ofNoFill();
    
     rough;
     smooth;
    
    rough.clear();
    smooth.clear();
    rough.addVertex(neck);
    rough.bezierTo(spineCP1Mover.getPosition(), spineCP2Mover.getPosition(), spineBase);
    ofxPolyToMesh(smooth, rough, 4);
    smooth.draw();
    
    
    if(APPC->debug){
        ofSetColor(255,0,0);
        ofFill();
        ofDrawCircle( spineCP1Mover.getPosition(),5);
        ofSetColor(255);
        ofSetColor(255,0,255);
        ofDrawCircle( spineCP2Mover.getPosition(),5);
        ofSetColor(255);
    }
    
    

    
    
    
    
    
    mid=leftKnee-spineBase;
    cp1=mid;//.getRotated(-40);
    cp1*=0.75;
    cp1+=spineBase;
    
    mid=leftKnee-leftFoot;
    cp2=mid;//.getRotated(40);
    cp2*=0.75;
    cp2+=leftFoot;
    
    ofNoFill();
    ofSetLineWidth(avatarLineWidth);
    
    
    rough.clear();
    smooth.clear();
    rough.addVertex(spineBase);
    rough.bezierTo(leftLegCP1Mover.getPosition(), leftLegCP2Mover.getPosition(), leftFoot);
    ofxPolyToMesh(smooth, rough, 4);
    smooth.draw();
    
    
    if(APPC->debug){
        ofFill();
        ofSetColor(255,0,0);
        ofSetLineWidth(1);
        ofDrawLine(cp1,spineBase);
        ofDrawCircle(cp1,5);
        ofSetColor(255);
        ofSetColor(255,0,255);
        ofDrawCircle(cp2,5);
        ofDrawLine(cp2,leftFoot);
        ofSetColor(0,0,255);
        ofDrawCircle(leftKnee,5);
        ofSetColor(255);
    }
    
    ofNoFill();
    ofSetLineWidth(avatarLineWidth);
    
    mid=rightKnee-spineBase;
    cp1=mid;//.getRotated(-40);
    cp1*=0.75;
    cp1+=spineBase;
    
    mid=rightKnee-rightFoot;
    cp2=mid;//.getRotated(40);
    cp2*=0.75;
    cp2+=rightFoot;
    
    rough.clear();
    smooth.clear();
    rough.addVertex(spineBase);
    rough.bezierTo(rightLegCP1Mover.getPosition(), rightLegCP2Mover.getPosition(), rightFoot);
    ofxPolyToMesh(smooth, rough, 4);
    smooth.draw();
    
    if(APPC->debug){
        ofFill();
        ofSetColor(255,0,0);
        ofSetLineWidth(1);
        ofDrawLine(cp1,spineBase);
        ofDrawCircle(cp1,5);
        ofSetColor(255);
        ofSetColor(255,0,255);
        ofDrawCircle(cp2,5);
        ofDrawLine(cp2,rightFoot);
        ofSetColor(0,0,255);
        ofDrawCircle(rightKnee,5);
        ofSetColor(255);
    }
    
    mid=leftEllbow-neck;
    cp1=mid;//.getRotated(-40);
    cp1*=0.7;
    cp1+=neck;
    
    mid=leftEllbow-leftHand;
    cp2=mid;//.getRotated(40);
    cp2*=0.7;
    cp2+=leftHand;
    
    if(APPC->debug){
        ofFill();
        ofSetColor(255,0,0);
        ofSetLineWidth(1);
        ofDrawLine(cp1,neck);
        ofDrawCircle(cp1,5);
        ofSetColor(255);
        ofSetColor(255,0,255);
        ofDrawCircle(cp2,5);
        ofDrawLine(cp2,leftHand);
        ofSetColor(0,0,255);
        ofDrawCircle(leftEllbow,5);
        ofSetColor(255);
    }
    
    
    ofSetLineWidth(avatarLineWidth);
    ofNoFill();
    ofSetColor(255);
   
    rough.clear();
    smooth.clear();
    rough.addVertex(neck);
    rough.bezierTo(leftArmCP1Mover.getPosition(), leftArmCP2Mover.getPosition(), leftHand);
    ofxPolyToMesh(smooth, rough, 4);
    smooth.draw();
    
    ofFill();
    ofDrawCircle(leftHand,15);
    
    mid=rightEllbow-neck;
    cp1=mid;//.getRotated(-40);
    cp1*=0.5;
    cp1+=neck;
    
    mid=rightEllbow-rightHand;
    cp2=mid;//.getRotated(40);
    cp2*=0.5;
    cp2+=rightHand;
    
    if(APPC->debug){
        ofFill();
        ofSetColor(255,0,0);
        ofSetLineWidth(1);
        ofDrawLine(cp1,neck);
        ofDrawCircle(cp1,5);
        ofSetColor(255);
        ofSetColor(255,0,255);
        ofDrawCircle(cp2,5);
        ofDrawLine(cp2,rightHand);
        ofSetColor(0,0,255);
        ofDrawCircle(rightEllbow,5);
        ofSetColor(255);
    }
    ofSetLineWidth(avatarLineWidth);
    
    ofNoFill();
    
    rough.clear();
    smooth.clear();
    rough.addVertex(neck);
    rough.bezierTo(rightArmCP1Mover.getPosition(), rightArmCP2Mover.getPosition(), rightHand);
    ofxPolyToMesh(smooth, rough, 4);
    smooth.draw();
    
    ofFill();
    ofDrawCircle(rightHand,15);
    
    ofPath leftfootpath;
    leftfootpath.moveTo(leftFoot.x+10,leftFoot.y);
    leftfootpath.arc(leftFoot.x+10,leftFoot.y, 20, 20, 180, 0);
    leftfootpath.setFilled(true);
    leftfootpath.setStrokeWidth(1);
    leftfootpath.close();
    leftfootpath.draw();
    
    ofPath rightfootpath;
    rightfootpath.moveTo(rightFoot.x-10,rightFoot.y);
    rightfootpath.arc(rightFoot.x-10,rightFoot.y, 20, 20, 180, 0);
    rightfootpath.setFilled(true);
    rightfootpath.setStrokeWidth(1);
    rightfootpath.close();
    rightfootpath.draw();
    
    ofPopStyle();
    ofPopMatrix();
    
}

void Avatar::setLeftEyePosition(ofVec2f p){
    leftEyePosition=p;
}

void Avatar::setRightEyePosition(ofVec2f p){
    rightEyePosition=p;
}

void Avatar::setLeftMouthPosition(ofVec2f p){
    leftMouthPosition=p;
}

void Avatar::setRightMouthPosition(ofVec2f p){
    rightMouthPosition=p;
}

void Avatar::setMouthCenterPosition(ofVec2f p){
    mouthCenterPosition=p;
}
