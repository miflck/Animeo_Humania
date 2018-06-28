//
//  MovingObject.cpp
//  moving object
//
//  Created by Flückiger Michael on 08.05.18.
//

#include "Avatar.hpp"
#include "ofxEasing.h"
#include "KinectV2Manager.hpp"


Avatar::Avatar(){
    
}

Avatar::~Avatar(){
    
}

void Avatar::setup(){
}

void Avatar::update(){
    
    head=ofVec2f(ofGetMouseX(),ofGetMouseY());
    
    vector<MappedPoints> mskel=KINECTMANAGER->getMappedSkelettons();
    if(mskel.size()>0){
        
        head=mskel[0].head;
        
        leftHand=ofVec2f(mskel[0].leftHand.x,mskel[0].leftHand.y);
        leftEllbow=ofVec2f(mskel[0].leftEllbow.x,mskel[0].leftEllbow.y);
        leftShoulder=ofVec2f(mskel[0].leftShoulder.x,mskel[0].leftShoulder.y);
        leftHip=mskel[0].leftHip;
        leftKnee=mskel[0].leftKnee;
        leftAnkle=mskel[0].leftAnkle;
        leftFoot=mskel[0].leftFoot;
        leftWrist=mskel[0].leftWrist;
        
        rightHand=ofVec2f(mskel[0].rightHand.x,mskel[0].rightHand.y);
        rightEllbow=ofVec2f(mskel[0].rightEllbow.x,mskel[0].rightEllbow.y);
        
        rightShoulder=ofVec2f(mskel[0].rightShoulder.x,mskel[0].rightShoulder.y);
        rightHip=mskel[0].rightHip;
        rightKnee=mskel[0].rightKnee;
        rightAnkle=mskel[0].rightAnkle;
        rightFoot=mskel[0].rightFoot;
        rightWrist=mskel[0].rightWrist;
        
        neck=mskel[0].neck;
        spineBase=ofVec2f(mskel[0].spineBase.x,mskel[0].spineBase.y);
        spineMid=ofVec2f(mskel[0].spineMid.x,mskel[0].spineMid.y);
    }
    
   // auto endTime = easingInitTime + scaleDuration;
   // auto now = ofGetElapsedTimef();
    move();
   // actualRadius = ofxeasing::map_clamp(now, easingInitTime, endTime, actualRadius, radiusTarget, &ofxeasing::linear::easeIn);
   // move();
    //position+=speed;
    //skeletons=KINECTMANAGER->getSkelettons();
    
    
}


void Avatar::drawAvatar(){
    
    headOffset=ofVec2f(0,-40);
    vector<MappedPoints> mskel=KINECTMANAGER->getMappedSkelettons();

    
    ofPushMatrix();
    ofPushStyle();
    
    ofVec2f headCenter=head+headOffset;
    
    ofSetColor(255);
    ofDrawCircle(headCenter,80);
    ofSetColor(0);
    
    // EYES
    ofDrawCircle(head.x-20,head.y-60,10);
    ofDrawCircle(head.x+20,head.y-60,10);
    //Cheeks
    ofSetColor(255,0,0);
    ofDrawCircle(headCenter.x-40,headCenter.y+10,20);
    ofDrawCircle(headCenter.x+40,headCenter.y+10,20);
    
    // Hair
    ofVec2f dist=ofVec2f(80,0);
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
    
    
    if(mskel.size()>0){
        
        ofDrawLine(neck,spineMid);
        ofDrawLine(spineMid,spineBase);
        
        ofDrawLine(spineBase,leftHip);
        ofDrawLine(spineBase,rightHip);
        
        ofDrawLine(rightHip,rightKnee);
        ofDrawLine(rightKnee,rightFoot);
        
        ofDrawLine(leftHip,leftKnee);
        ofDrawLine(leftKnee,leftFoot);
        
        ofSetColor(255);
        ofDrawCircle(neck,5);
        ofDrawLine(rightShoulder,leftShoulder);
        
        ofDrawLine(leftShoulder,neck);
        ofDrawCircle(leftShoulder,5);
        ofDrawLine(leftEllbow,leftShoulder);
        ofDrawCircle(leftEllbow,5);
        ofDrawLine(leftHand,leftEllbow);

        ofDrawLine(rightShoulder,neck);
        ofDrawCircle(rightShoulder,5);
        ofDrawLine(rightEllbow,rightShoulder);
        ofDrawCircle(rightEllbow,5);
        ofDrawLine(rightHand,rightEllbow);
        ofDrawCircle(rightHand,10);
  
    }
    ofPopStyle();
    ofPopMatrix();
    
}


void Avatar::draw(){
    
    ofPushMatrix();
    ofPushStyle();
    drawAvatar();
    ofPopStyle();
    ofPopMatrix();
}


