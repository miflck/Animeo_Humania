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


Avatar::Avatar(){
    
}

Avatar::~Avatar(){
    
}

void Avatar::setup(){
    bSeekTarget=true;
    setTarget(ofVec2f(ofGetWidth()/2,ofGetHeight()/2));
    setSeekForce(5);
    
    movers.push_back(&leftHandMover);
    movers.push_back(&leftEllbowMover);
    movers.push_back(&neckMover);
    
    movers.push_back(&rightHandMover);
    movers.push_back(&rightEllbowMover);
    movers.push_back(&spineBaseMover);
    
    movers.push_back(&leftKneeMover);
    movers.push_back(&rightKneeMover);
    movers.push_back(&leftFootMover);
    movers.push_back(&rightFootMover);
    
    for(int i=0;i<movers.size();i++){
        movers[i]->bSeekTarget=true;
        movers[i]->setSeekForce(10);
    }
    
}

void Avatar::update(){
    for(int i=0;i<movers.size();i++){
        movers[i]->move();
    }
    
   // head=ofVec2f(0,0);
    
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
        
        
    }else{
        
        
   
        
        ofVec2f neckP=head+headOffset+ofVec2f(0,80);
        neck=neckP;
        
        neckMover.setTarget(neckP);
        neck=neckMover.getPosition();

        
        ofVec2f spineBaseP=neck+ofVec2f(0,120);
        spineBase=spineBaseP;
        
        ofVec2f leftEllbowP=neck+ofVec2f(60,60);
       // leftEllbow=leftEllbowP;
        leftEllbowMover.setTarget(leftEllbowP);
        leftEllbow=leftEllbowMover.getPosition();

        ofVec2f leftHandP=leftEllbow+ofVec2f(60,-60);
       // leftHand=leftHandP;
        leftHandMover.setTarget(leftHandP);
        leftHand=leftHandMover.getPosition();
        
        ofVec2f rightEllbowP=neck+ofVec2f(-60,60);
        rightEllbow=rightEllbowP;
        rightEllbowMover.setTarget(rightEllbowP);
        rightEllbow=rightEllbowMover.getPosition();
        
        ofVec2f rightHandP=rightEllbow+ofVec2f(0,60);
       rightHand=rightHandP;
        rightHandMover.setTarget(rightHandP);
        rightHand=rightHandMover.getPosition();
        
        ofVec2f rightKneeP=spineBase+ofVec2f(-30,60);
        rightKnee=rightKneeP;
        rightKneeMover.setTarget(rightKneeP);
        rightKnee=rightKneeMover.getPosition();
        
        ofVec2f leftKneeP=spineBase+ofVec2f(30,60);
        leftKnee=leftKneeP;
        leftKneeMover.setTarget(leftKneeP);
       leftKnee=leftKneeMover.getPosition();
        
        ofVec2f rightFootP=rightKnee+ofVec2f(10,60);
        rightFoot=rightFootP;
        rightFootMover.setTarget(rightFootP);
        rightFoot=rightFootMover.getPosition();
        
        ofVec2f leftFootP=leftKnee+ofVec2f(-10,60);
        leftFoot=leftFootP;
        leftFootMover.setTarget(leftFootP);
        leftFoot=leftFootMover.getPosition();

    
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
    ofDrawCircle(head.x-20,head.y-40,10);
    ofDrawCircle(head.x+20,head.y-40,10);
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
  
    }else{
        ofSetLineWidth(10);


        float rotation=40;
        if(head.x-spineBase.x>0){
            rotation=-40;
        }else{
            rotation=40;
        }
        
        ofVec2f mid=spineBase-neck;
        ofVec2f cp1=mid.getRotated(rotation);
        cp1/=3;
        cp1+=neck;
        
        ofVec2f cp2=mid.getRotated(-rotation);
        cp2/=-3;
        cp2+=spineBase;
        

        ofNoFill();
        ofDrawBezier(neck.x,neck.y, cp1.x,cp1.y,cp2.x,cp2.y, spineBase.x,spineBase.y);
        if(APPC->debug){
            ofSetColor(255,0,0);
            ofFill();
            ofDrawCircle(cp1,5);
            ofSetColor(255);
            ofSetColor(255,0,255);
            ofDrawCircle(cp2,5);
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
        ofSetLineWidth(10);
        ofDrawBezier(spineBase.x,spineBase.y, cp1.x,cp1.y,cp2.x,cp2.y, leftFoot.x,leftFoot.y);
        
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
        ofSetLineWidth(10);
        
        mid=rightKnee-spineBase;
        cp1=mid;//.getRotated(-40);
        cp1*=0.75;
        cp1+=spineBase;
        
        mid=rightKnee-rightFoot;
        cp2=mid;//.getRotated(40);
        cp2*=0.75;
        cp2+=rightFoot;
        ofDrawBezier(spineBase.x,spineBase.y, cp1.x,cp1.y,cp2.x,cp2.y, rightFoot.x,rightFoot.y);
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
        
        
        ofSetLineWidth(10);
        ofNoFill();
        ofDrawBezier(neck.x,neck.y, cp1.x,cp1.y,cp2.x,cp2.y, leftHand.x,leftHand.y);
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
        ofSetLineWidth(10);

        ofNoFill();
        ofDrawBezier(neck.x,neck.y,cp1.x,cp1.y,cp2.x,cp2.y, rightHand.x,rightHand.y);
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
        rightfootpath.moveTo(rightFoot.x-10,leftFoot.y);
        rightfootpath.arc(rightFoot.x-10,leftFoot.y, 20, 20, 180, 0);
        rightfootpath.setFilled(true);
        rightfootpath.setStrokeWidth(1);
        rightfootpath.close();
        rightfootpath.draw();

        
    }
    ofPopStyle();
    ofPopMatrix();
    
}


void Avatar::draw(){
    
    ofPushMatrix();
    ofPushStyle();
    ofTranslate(getPosition());
    drawAvatar();
    ofPopStyle();
    ofPopMatrix();
}


