//
//  Humania.cpp
//  appswitcher
//
//  Created by Flückiger Michael on 17.07.18.
//

#include "Humania.hpp"
#include "ofxEasing.h"
#include "KinectV2Manager.hpp"
#include "ApplicationController.h"
#include "ofxPoly.h"

Humania::Humania(){
    
}

Humania::~Humania(){
    
}
void Humania::setup(){
    cout<<"setup humania"<<startposition<<endl;

    Avatar::setup();
    state=IDLE;
    headRadiusTarget=0;
    headEasingDuration=20;
    actualHeadRadius=0;
    setPosition(ofGetWidth()/3,ofGetHeight()/2);
    setTarget(ofVec2f(2*ofGetWidth()/3,ofGetHeight()/2));
    startposition=&Settings::getVec2("humania/startposition");
   // startposition=&Settings::getVec2("LightPointApp/homeposition");

    
    // FACE
    
    mouthCenterPosition=ofVec2f(0,150);
    leftMouth=ofVec2f(200,0);
    rightMouth=ofVec2f(-200,0);
    
    eyeRadiusTarget=0;
    eyeEasingDuration=5;
    actualEyeRadius=0;
    
    eyeDiameterTarget=0;
    actualEyeDiameter=0;
    
    
     bHasEyes=false;
     bHasMouth=false;
     bHasCheecks=false;
     bHasHair=false;
    
    
}





void Humania::update(){
    auto headEasingEndTime = headEasingInitTime + headEasingDuration;
    auto now = ofGetElapsedTimef();
    actualHeadRadius = ofxeasing::map_clamp(now, headEasingInitTime, headEasingEndTime, actualHeadRadius, headRadiusTarget, &ofxeasing::linear::easeIn);
    
    auto eyeEasingEndTime = eyeEasingInitTime + eyeEasingDuration;
    
    
    actualEyeRadius = ofxeasing::map_clamp(now, eyeEasingInitTime, eyeEasingEndTime, actualEyeRadius, eyeRadiusTarget, &ofxeasing::linear::easeIn);
    actualEyeDiameter= ofxeasing::map_clamp(now, eyeEasingInitTime, eyeEasingEndTime, actualEyeDiameter, eyeDiameterTarget, &ofxeasing::linear::easeIn);

    if(!bHasEyes && (actualEyeRadius==bigEyeRadius)){
        bHasEyes=true;
    }
    
    switch (state) {
            
        case IDLE:
           // Avatar::update();
            
            break;
            
            
        case FACE:
            Avatar::update();
            updateFace();
            break;
            
        case AVATAR:
            Avatar::update();
            move();
            //updateFace();

            break;
            
        default:
            break;
    }
}
void Humania::draw(){
    
    switch (state) {
            
        case IDLE:
            break;
            
        case FACE:
            
            ofPushMatrix();
            ofPushStyle();
            if(bPlay){
                ofTranslate(recordedAvatarPositions[playhead]);
            }else{
                ofTranslate(getPosition());
            }
            drawFaceAvatar();
            ofPopStyle();
            ofPopMatrix();
            
            
            break;
            
        case AVATAR:
            Avatar::draw();
            
            /*
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
            */
            break;
            
        default:
            break;
    }
    
}




void Humania::updateFace(){
    
    relativeHeadPosition=ofVec2f(0,0);
    if(bHeadIsBound){
        relativeHeadPosition=head;
    }
    
    if(bEyesAreBound) {
        float yoff=ofMap(head.y,0,-170,50,-50,true);
        eyeOffset=ofVec2f(head.x-spineBase.x,yoff);
    }else{
        eyeOffset=ofVec2f(0,0);
    }
    
    if(bMouthIsBound){
    leftMouthOffset=leftHand-leftEllbow;
    rightMouthOffset=rightHand-rightEllbow;
    leftMouthCornerOffset=leftEllbow-spineBase;
    rightMouthCornerOffset=rightHand-spineBase;
    }else{
        leftMouthOffset=ofVec2f(0,-30);
        rightMouthOffset=ofVec2f(0,-30);
        leftMouthCornerOffset=ofVec2f(-100,0);
        rightMouthCornerOffset=ofVec2f(100,0);
    }
    leftMouth=ofVec2f(mouthCenterPosition.x+leftMouthCornerOffset.x,leftMouthOffset.y+mouthCenterPosition.y);
    rightMouth=ofVec2f(mouthCenterPosition.x+rightMouthCornerOffset.x,rightMouthOffset.y+mouthCenterPosition.y);
    
   
}


void Humania::setState(int _state){
    cout<<"SET STATE"<<_state<<endl;
    stateBefore=state;
    state=_state;
    switch (state) {
            
        case IDLE:
            break;
            
        case FACE:
            resetToStart();
            headEasingInitTime= ofGetElapsedTimef();
            headRadiusTarget=300;
           
            break;
        case AVATAR:
            headRadiusTarget=80;
            setSmallEyes();
            break;
        
      
            
        default:
            break;
    }
}

int Humania::getState(){
    return state;
}

void Humania::setSmallEyes(){
    eyeEasingInitTime=ofGetElapsedTimef();
    eyeRadiusTarget=smallEyeRadius;
    eyeDiameterTarget=smallEyeRadius;
    //actualEyeDiameter=50;

}

void Humania::setBigEyes(){
    eyeRadiusTarget=bigEyeRadius;
    eyeEasingInitTime=ofGetElapsedTimef();
    eyeDiameterTarget=bigEyeRadius;
    actualEyeDiameter=bigEyeRadius;
}

void Humania::saveStartposition(ofVec2f _p){
    
    startposition->set(_p.x,_p.y);
    Settings::get().save("data.json");
    
}


void Humania::resetToStart(){
    cout<<"reset to start"<<startposition->x<<endl;
    setPosition(startposition->x, startposition->y);
    setTarget(ofVec2f(startposition->x, startposition->y));
}


void Humania::drawAvatar(){
    
    headOffset=ofVec2f(0,-100);
   // vector<MappedPoints> mskel=KINECTMANAGER->getMappedSkelettons();
    
    ofPushMatrix();
    ofPushStyle();
    ofVec2f headCenter=head+headOffset;
    ofSetColor(255);
    //ofDrawCircle(headCenter,80);
    ofDrawCircle(headCenter,actualHeadRadius);
    ofSetColor(0);
    
    // EYES
   
    //NOW with the animated Eyes
    // ofDrawCircle(headCenter.x-20,headCenter.y-10,10);
  //  ofDrawCircle(headCenter.x+20,headCenter.y-10,10);
    
   // ofDrawCircle(headCenter.x-2*actualEyeRadius,headCenter.y-actualEyeRadius,actualEyeRadius);
    //ofDrawCircle(headCenter.x+2*actualEyeRadius,headCenter.y-actualEyeRadius,actualEyeRadius);
    
    ofDrawEllipse(headCenter.x-2*eyeRadiusTarget,headCenter.y-eyeRadiusTarget,actualEyeDiameter*2,actualEyeRadius*2);
    ofDrawEllipse(headCenter.x+2*eyeRadiusTarget,headCenter.y-eyeRadiusTarget,actualEyeDiameter*2,actualEyeRadius*2);
    
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
    
    
    
    ofSetLineWidth(avatarLineWidth);
    
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
    
    ofPopStyle();
    ofPopMatrix();
    
    

    
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


void Humania::drawFaceAvatar(){
    
    headOffset=ofVec2f(0,0);
    ofPushMatrix();
    ofPushStyle();
    ofVec2f headCenter=relativeHeadPosition+headOffset;
    ofSetColor(255);
    ofDrawCircle(headCenter,actualHeadRadius);
    ofSetColor(0);
    
    // EYES
    ofPushMatrix();
    ofTranslate(eyeOffset.x,eyeOffset.y);
    
 //   ofDrawCircle(headCenter.x-20,headCenter.y-10,actualEyeRadius);
 //   ofDrawCircle(headCenter.x+20,headCenter.y-10,actualEyeRadius);
  //  ofDrawEllipse(headCenter.x-2*eyeRadiusTarget,headCenter.y-eyeRadiusTarget,actualEyeDiameter*2,actualEyeRadius*2);
  //  ofDrawEllipse(headCenter.x+2*eyeRadiusTarget,headCenter.y-eyeRadiusTarget,actualEyeDiameter*2,actualEyeRadius*2);
    
    if(bHasEyes){
    ofDrawEllipse(headCenter.x-2*actualEyeRadius,headCenter.y-actualEyeRadius,actualEyeDiameter*2,actualEyeRadius*2);
    ofDrawEllipse(headCenter.x+2*actualEyeRadius,headCenter.y-actualEyeRadius,actualEyeDiameter*2,actualEyeRadius*2);
    }else{
          ofDrawEllipse(headCenter.x-2*eyeRadiusTarget,headCenter.y-eyeRadiusTarget,actualEyeDiameter*2,actualEyeRadius*2);
          ofDrawEllipse(headCenter.x+2*eyeRadiusTarget,headCenter.y-eyeRadiusTarget,actualEyeDiameter*2,actualEyeRadius*2);
    }
    
    ofPopMatrix();

    
   /* ofPushMatrix();
    ofSetColor(0);
    
    ofTranslate(eyeOffset.x,-80);
    ofDrawCircle(100, 0, 50);
    ofDrawCircle(-100, 0, 50);
    ofPopMatrix();
    */
    
    
    //Cheeks
  /*  ofSetColor(255,0,0);
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
      */
    
    if(bHasMouth){
        mouth.clear();
        mouth.moveTo(leftMouth);
        mouth.bezierTo(ofVec2f(leftMouth.x,leftMouth.y+(mouthCenterPosition.y-leftMouth.y)),ofVec2f(mouthCenterPosition.x+50,mouthCenterPosition.y), mouthCenterPosition);
        mouth.bezierTo(ofVec2f(mouthCenterPosition.x-50,mouthCenterPosition.y),ofVec2f(rightMouth.x,rightMouth.y+(mouthCenterPosition.y-rightMouth.y)), rightMouth);
    
        mouth.setStrokeColor(ofColor(0,0,0));
        mouth.setFilled(false);
        mouth.setStrokeWidth(5);
        mouth.draw();
    }
    
    //ofSetLineWidth(avatarLineWidth);
    
   
    
    ofPopStyle();
    ofPopMatrix();
    
    
    
}
