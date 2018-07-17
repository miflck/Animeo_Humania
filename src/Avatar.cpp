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
    
    
    leftArmCP1Mover.bSeekTarget=true;
    leftArmCP2Mover.bSeekTarget=true;
    
    rightArmCP1Mover.bSeekTarget=true;
    rightArmCP2Mover.bSeekTarget=true;
    
    rightLegCP1Mover.bSeekTarget=true;
    rightLegCP2Mover.bSeekTarget=true;
    
    leftLegCP1Mover.bSeekTarget=true;
    leftLegCP2Mover.bSeekTarget=true;
    
    spineCP1Mover.bSeekTarget=true;
    spineCP2Mover.bSeekTarget=true;
    
    spineCP1Mover.setMaxSpeed(60);
    spineCP2Mover.setMaxSpeed(60);
    
    spineCP1Mover.setSeekForce(10);
    spineCP2Mover.setSeekForce(10);

    movers.push_back(&absolutePosition);

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
    
    for(int i=0;i<movers.size();i++){
        movers[i]->bSeekTarget=true;
        movers[i]->setSeekForce(20);
        movers[i]->setMaxSpeed(150);
            //movers[i]->setSlowDown(false);
    }
    
}

void Avatar::update(){
    for(int i=0;i<movers.size();i++){
        movers[i]->move();
    }
    
    leftArmCP1Mover.move();
    leftArmCP2Mover.move();
    rightArmCP1Mover.move();
    rightArmCP2Mover.move();
    
    rightLegCP1Mover.move();
    rightLegCP2Mover.move();
    leftLegCP1Mover.move();
    leftLegCP2Mover.move();
    
    spineCP1Mover.move();
    spineCP2Mover.move();
    
   ofVec2f headP=ofVec2f(0,0);
    
    
    
    ofVec2f neckP=head+headOffset+ofVec2f(0,80);
    ofVec2f spineBaseP=neck+ofVec2f(0,120);
    ofVec2f leftEllbowP=neck+ofVec2f(60,60);
    ofVec2f leftHandP=leftEllbow+ofVec2f(60,-60);
    ofVec2f rightEllbowP=neck+ofVec2f(-60,60);
    ofVec2f rightHandP=rightEllbow+ofVec2f(0,60);
    ofVec2f rightKneeP=spineBase+ofVec2f(-30,60);
    ofVec2f leftKneeP=spineBase+ofVec2f(30,60);
    ofVec2f rightFootP=rightKnee+ofVec2f(10,60);
    ofVec2f leftFootP=leftKnee+ofVec2f(-10,60);
    
    
    // r=50.f*sin(ofGetElapsedTimef()*5);
    // ofVec2f leftEllbowP=neck+ofVec2f(60,60+r);
    
    vector<MappedPoints> mskel=KINECTMANAGER->getMappedSkelettons();
    
    if(mskel.size()>0&&bRecord){
        recordedBonesPositions.push_back(mskel[skelettonId]);
        recordedAvatarPositions.push_back(getPosition());
    }

    
    
    if(mskel.size()>0){
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
        
        playhead=(playhead+1) % (recordedBonesPositions.size()-1);
        cout<<playhead<<" "<<recordedBonesPositions.size()<<endl;
        
        
        
    }
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

    


   // }
    
   // auto endTime = easingInitTime + scaleDuration;
   // auto now = ofGetElapsedTimef();
    move();
   // actualRadius = ofxeasing::map_clamp(now, easingInitTime, endTime, actualRadius, radiusTarget, &ofxeasing::linear::easeIn);
   // move();
    //position+=speed;
    //skeletons=KINECTMANAGER->getSkelettons();
    
    
}


void Avatar::drawAvatar(){
    headOffset=ofVec2f(0,-100);
    vector<MappedPoints> mskel=KINECTMANAGER->getMappedSkelettons();

    
    ofPushMatrix();
    ofPushStyle();

    
    ofVec2f headCenter=head+headOffset;
    
    ofSetColor(255);
    ofDrawCircle(headCenter,80);
    ofSetColor(0);
    
    // EYES
    ofDrawCircle(headCenter.x-20,headCenter.y-10,10);
    ofDrawCircle(headCenter.x+20,headCenter.y-10,10);
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
    
    ofPolyline rough;
    ofMesh smooth;
    
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
        ofSetLineWidth(10);
       // ofDrawBezier(spineBase.x,spineBase.y, cp1.x,cp1.y,cp2.x,cp2.y, leftFoot.x,leftFoot.y);
    
    
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
        ofSetLineWidth(10);
        
        mid=rightKnee-spineBase;
        cp1=mid;//.getRotated(-40);
        cp1*=0.75;
        cp1+=spineBase;
        
        mid=rightKnee-rightFoot;
        cp2=mid;//.getRotated(40);
        cp2*=0.75;
        cp2+=rightFoot;
        //ofDrawBezier(spineBase.x,spineBase.y, cp1.x,cp1.y,cp2.x,cp2.y, rightFoot.x,rightFoot.y);
    
    
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
        
        
        ofSetLineWidth(10);
        ofNoFill();
        //ofDrawBezier(neck.x,neck.y, cp1.x,cp1.y,cp2.x,cp2.y, leftHand.x,leftHand.y);
    
        ofSetColor(255);
      //  ofDrawBezier(neck.x,neck.y, leftArmCP1Mover.getPosition().x,leftArmCP1Mover.getPosition().y,leftArmCP2Mover.getPosition().x,leftArmCP2Mover.getPosition().y, leftHand.x,leftHand.y);
   
    
        rough.clear();
        smooth.clear();
        rough.addVertex(neck);
        rough.bezierTo(leftArmCP1Mover.getPosition(), leftArmCP2Mover.getPosition(), leftHand);
        ofxPolyToMesh(smooth, rough, 4);
        smooth.draw();

    
        ofFill();
        ofDrawCircle(leftHand,15);

        
     /*   ofPath leftArmpath;
        leftArmpath.moveTo(neck);
        leftArmpath.bezierTo(cp1.x,cp1.y,cp2.x,cp2.y, leftHand.x,leftHand.y);
        leftArmpath.setFilled(false);
        leftArmpath.setColor(ofColor(255,0,0));
        leftArmpath.setStrokeWidth(5);
        leftArmpath.draw();
        */
        
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
        //ofDrawBezier(neck.x,neck.y,cp1.x,cp1.y,cp2.x,cp2.y, rightHand.x,rightHand.y);
    
        rough.clear();
        smooth.clear();
        rough.addVertex(neck);
        rough.bezierTo(rightArmCP1Mover.getPosition(), rightArmCP2Mover.getPosition(), rightHand);
        ofxPolyToMesh(smooth, rough, 4);
        smooth.draw();
    
        ofFill();
        ofDrawCircle(rightHand,15);
        
        
        
        

        
  //  ofDrawCircle(leftFoot, 20);
  //  ofDrawCircle(rightFoot, 20);

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

        
  //  }
    ofPopStyle();
    ofPopMatrix();
    
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

