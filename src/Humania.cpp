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
  
    setPosition(ofGetWidth()/3,ofGetHeight()/2);
    setTarget(ofVec2f(2*ofGetWidth()/3,ofGetHeight()/2));
    startposition=&Settings::getVec2("humania/startposition");
    setPosition(*startposition);
    setTarget(*startposition);
    
    
    headRadiusBig=300;
    headRadiusNormal=80;
    
    headRadiusTarget=0;
    headEasingDuration=20;
    actualHeadRadius=0;
    
    // FACE
    
    mouthCenterPosition=ofVec2f(0,60);
    leftMouth=ofVec2f(30,0);
    rightMouth=ofVec2f(-30,0);
    
    eyeRadiusTarget=0;
    eyeEasingDuration=5;
    actualEyeRadius=0;
    
    eyeDiameterTarget=0;
    actualEyeDiameter=0;
    
    
    //cheeks
    actualCheeksAlpha=0;
    cheeksEasingDuration=10;
    cheeksAlphaTarget=0;
    
    //nose
    actualNoseAlpha=0;
    noseEasingDuration=10;

    //nose
    hairInitTime=0;
    actualHairAlpha=0;
    hairEasingDuration=10;
    hairAlphaTarget=0;
    hairRotationTarget=0;
    actualHairRotation=0;
    
     bHasEyes=false;
     bHasMouth=false;
     bHasCheecks=false;
     bHasHair=false;

    
    bBindPosition=true;
    
    
    eyemovers.push_back(&leftEyeMover);
    eyemovers.push_back(&rightEyeMover);

    
    
    float s=headRadiusBig/headRadiusNormal;

    
    leftEyeTarget=ofVec2f(-80,-40)/s;
    rightEyeTarget=ofVec2f(150,40)/s;
    
    leftEyeMover.setTarget(leftEyeTarget);
    rightEyeMover.setTarget(rightEyeTarget);
    
 
    
    
    for(int i=0;i<eyemovers.size();i++){
        eyemovers[i]->bSeekTarget=true;
        eyemovers[i]->setSeekForce(20);
        eyemovers[i]->setMaxSpeed(80);
        //movers[i]->setSlowDown(false);
    }

    
    mouthmovers.push_back(&leftMouthMover);
    mouthmovers.push_back(&rightMouthMover);
    
    leftMouthTarget=ofVec2f(-30,-10)+mouthCenterPosition;
    rightMouthTarget=ofVec2f(30,-10)+mouthCenterPosition;

    leftMouthMover.setTarget(leftMouthTarget);
    rightMouthMover.setTarget(rightMouthTarget);
    
    
    
    for(int i=0;i<mouthmovers.size();i++){
        mouthmovers[i]->bSeekTarget=true;
        mouthmovers[i]->setSeekForce(20);
        mouthmovers[i]->setMaxSpeed(80);
        //movers[i]->setSlowDown(false);
    }
    
    
    
     noseSound.load("Sounds/Nase.wav");
     cheekSound.load("Sounds/Backen.wav");
     mouthSound.load("Sounds/Mund.wav");

    bodySound.load("Sounds/Körper.wav");
    hairSound.load("Sounds/Haare.wav");

    

    
}





void Humania::update(){
    
    for(int i=0;i<eyemovers.size();i++){
        eyemovers[i]->move();
    }
    
    for(int i=0;i<mouthmovers.size();i++){
        mouthmovers[i]->move();
    }
    
    
    auto headEasingEndTime = headEasingInitTime + headEasingDuration;
    auto now = ofGetElapsedTimef();
   
    actualHeadRadius = ofxeasing::map_clamp(now, headEasingInitTime, headEasingEndTime, actualHeadRadius, headRadiusTarget, &ofxeasing::linear::easeInOut);
    scaleFactor=actualHeadRadius/headRadiusNormal;
    
    
    
    auto eyeEasingEndTime = eyeEasingInitTime + eyeEasingDuration;
    
    
    actualEyeRadius = ofxeasing::map_clamp(now, eyeEasingInitTime, eyeEasingEndTime, actualEyeRadius, eyeRadiusTarget, &ofxeasing::linear::easeInOut);
    actualEyeDiameter= ofxeasing::map_clamp(now, eyeEasingInitTime, eyeEasingEndTime, actualEyeDiameter, eyeDiameterTarget, &ofxeasing::linear::easeInOut);
    
    
    auto cheeksEasingEndTime = cheeksInitTime + cheeksEasingDuration;
    actualCheeksAlpha= ofxeasing::map_clamp(now, cheeksInitTime, cheeksEasingEndTime, actualCheeksAlpha, cheeksAlphaTarget, &ofxeasing::linear::easeInOut);
//    cout<<actualCheeksAlpha<<" "<<cheeksAlphaTarget<<" cheeksEasingEndTime "<<cheeksEasingEndTime<<endl;
    
    auto noseEasingEndTime = noseInitTime + noseEasingDuration;
    actualNoseAlpha= ofxeasing::map_clamp(now, noseInitTime, cheeksEasingEndTime, actualNoseAlpha, noseAlphaTarget, &ofxeasing::linear::easeInOut);
    
    
    auto hairEasingEndTime = hairInitTime +hairEasingDuration;
    actualHairAlpha= ofxeasing::map_clamp(now,hairInitTime, hairEasingEndTime, actualHairAlpha, hairAlphaTarget, &ofxeasing::linear::easeInOut);
        actualHairRotation= ofxeasing::map_clamp(now,hairInitTime, hairEasingEndTime, actualHairRotation, hairRotationTarget, &ofxeasing::linear::easeInOut);

    if(!bHasEyes && (actualEyeRadius==bigEyeRadius)){
        bHasEyes=true;
    }
    
    
   // updateSkeletton();
    
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
    
    
    ofVec2f headRadiusShift=neck-spineBase;
    headRadiusShift.y-=actualHeadRadius;

    switch (state) {
            
        case IDLE:
            break;
            
        case FACE:
            ofPushMatrix();
            ofPushStyle();
            if(bPlay && bBindPosition){
                ofTranslate(recordedAvatarPositions[playhead]);
            
            }else{
                ofTranslate(getPosition());
            }
            
            ofPushMatrix();
            if(bHasBody){
            ofTranslate(headRadiusShift);
            }
            drawFaceAvatar();
            ofPopMatrix();
            ofPopStyle();
            
            if(bHasBody){
                ofPushMatrix();
                ofTranslate(-spineBase);
                drawBody();
                ofPopMatrix();
            }
            
            ofPopMatrix();
            
            
            break;
            
        case AVATAR:
            ofPushMatrix();
            ofPushStyle();
            //ofTranslate(0,-80); // weird head translation

            if(bPlay && bBindPosition){
                ofTranslate(recordedAvatarPositions[playhead]);
                
            }else{
                ofTranslate(getPosition());
            }
            ofPushMatrix();
            ofTranslate(headRadiusShift);
            drawFaceAvatar();
            ofPopMatrix();
            ofPopStyle();
            if(bHasBody){
                ofPushMatrix();
                ofTranslate(-spineBase);
                drawBody();
                ofPopMatrix();
            }
            ofPopMatrix();
            
            
            
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
        
        ofVec2f left=leftHand-neck;
        float length=ofMap(left.length(),50,180,0,50,true);
        leftEyeMover.setTarget((leftHand-neck).getScaled(length));
        
        ofVec2f right=rightHand-neck;
        length=ofMap(right.length(),50,180,0,50,true);
        rightEyeMover.setTarget((rightHand-neck).getScaled(length));

        
        
    }else{
        
       // leftEyeMover.setTarget(ofVec2f(-20,0));
       // rightEyeMover.setTarget(ofVec2f(+20,0));
        
    }
    
    if(bMouthIsBound){
    leftMouthOffset=leftHand-leftEllbow;
    rightMouthOffset=rightHand-rightEllbow;
    leftMouthCornerOffset=leftEllbow-spineBase;
    rightMouthCornerOffset=rightHand-spineBase;
        
        
    float length=ofMap(leftMouthOffset.length(),50,180,0,50,true);
    leftMouthOffset=((leftMouthOffset).getScaled(length));
    length=ofMap(rightMouthOffset.length(),50,180,0,50,true);
    rightMouthOffset=((rightMouthOffset).getScaled(length));
    
    length=ofMap(leftMouthCornerOffset.length(),50,180,0,50,true);
    leftMouthCornerOffset=((leftMouthCornerOffset).getScaled(length));
        
    length=ofMap(rightMouthCornerOffset.length(),50,180,0,50,true);
    rightMouthCornerOffset=((rightMouthCornerOffset).getScaled(length));
        
       // leftMouthCornerOffset=ofVec2f(-100,0);
       // rightMouthCornerOffset=ofVec2f(100,0);
        
        leftMouthTarget=ofVec2f(mouthCenterPosition.x+leftMouthCornerOffset.x,leftMouthOffset.y+mouthCenterPosition.y);
        rightMouthTarget=ofVec2f(mouthCenterPosition.x+rightMouthCornerOffset.x,rightMouthOffset.y+mouthCenterPosition.y);
        
        leftMouthMover.setTarget(leftMouthTarget);
        rightMouthMover.setTarget(rightMouthTarget);
        
    }else{
        
       
        
        
       /* leftMouthOffset=ofVec2f(0,-10);
        rightMouthOffset=ofVec2f(0,-10);
        leftMouthCornerOffset=ofVec2f(-30,0);
        rightMouthCornerOffset=ofVec2f(30,0);*/
    }
    
    // without movers
   // leftMouth=ofVec2f(mouthCenterPosition.x+leftMouthCornerOffset.x,leftMouthOffset.y+mouthCenterPosition.y);
   // rightMouth=ofVec2f(mouthCenterPosition.x+rightMouthCornerOffset.x,rightMouthOffset.y+mouthCenterPosition.y);
    leftMouth=leftMouthMover.getPosition();
    rightMouth=rightMouthMover.getPosition();
   
}


ofVec2f Humania::getLeftEyePosition(){
    return leftEyeMover.getPosition();
}

ofVec2f Humania::getRightEyePosition(){
    return rightEyeMover.getPosition();
}


ofVec2f Humania::getLeftMouthPosition(){
    return leftMouth;
}

ofVec2f Humania::getRightMouthPosition(){
    return rightMouth;
}

ofVec2f Humania::getMouthCenterPosition(){
    return mouthCenterPosition;
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
            headRadiusTarget=headRadiusBig;
            headEasingDuration=20;
            actualHeadRadius=headRadiusBig;

            break;
        case AVATAR:
            headEasingInitTime= ofGetElapsedTimef();
            headRadiusTarget=headRadiusNormal;
            headEasingDuration=5;
           // setSmallEyes();
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


void Humania::openEyes(){
    actualEyeRadius=0;
    eyeRadiusTarget=smallEyeRadius;
    eyeEasingInitTime=ofGetElapsedTimef();
    eyeDiameterTarget=smallEyeRadius;
    actualEyeDiameter=smallEyeRadius;
}


void Humania::closeEyes(){
    eyeRadiusTarget=0;
    eyeEasingInitTime=ofGetElapsedTimef();
    eyeDiameterTarget=smallEyeRadius;
    actualEyeDiameter=smallEyeRadius;
}


void Humania::toggleCheeks(){
    if(bHasCheecks){
        bHasCheecks=false;
        cheeksInitTime=ofGetElapsedTimef();
        cheeksAlphaTarget=0;
        cheeksEasingDuration=10;

    }else{
        bHasCheecks=true;
        cheeksInitTime=ofGetElapsedTimef();
        cheeksAlphaTarget=255;
        cheeksEasingDuration=10;
    }
}

void Humania::showCheeks(bool _b){
    if(_b){
        bHasCheecks=true;
        cheeksInitTime=ofGetElapsedTimef();
        cheeksAlphaTarget=255;
        cheeksEasingDuration=10;
        actualCheeksAlpha=0;
        cheekSound.play();
   
        
    }else{
        bHasCheecks=false;
        cheeksInitTime=ofGetElapsedTimef();
        cheeksAlphaTarget=0;
        cheeksEasingDuration=10;
    }
}



void Humania::toggleNose(){
    if(bHasNose){
        bHasNose=false;
        noseInitTime=ofGetElapsedTimef();
        noseAlphaTarget=0;
        noseEasingDuration=10;
    }else{
        bHasNose=true;
        noseInitTime=ofGetElapsedTimef();
        noseAlphaTarget=255;
        noseEasingDuration=10;
    }
}

void Humania::showNose(bool _b){
    if(_b){
        bHasNose=true;
        noseInitTime=ofGetElapsedTimef();
        actualNoseAlpha=0;
        noseAlphaTarget=255;
        noseEasingDuration=10;
        noseSound.play();
    }else{
        bHasNose=false;
        noseInitTime=ofGetElapsedTimef();
        noseAlphaTarget=0;
        noseEasingDuration=10;
    }
}

void Humania::showHair(bool _b){
    if(_b){
        bHasHair=true;
        hairInitTime=ofGetElapsedTimef();
        hairAlphaTarget=255;
        hairEasingDuration=10;
        hairRotationTarget=-180;
        hairSound.play();

    }else{
        bHasHair=false;
        hairInitTime=ofGetElapsedTimef();
        hairAlphaTarget=0;
        hairEasingDuration=10;
        hairRotationTarget=0;

    }
}


void Humania::toggleBody(){
    if(bHasBody){
        bHasBody=false;
    }else{
        for(int i=0;i<movers.size();i++){
            movers[i]->setPosition(0, 0);
            movers[i]->setMaxSpeed(80);
        }
        bHasBody=true;
    }
}

void Humania::showBody(bool _b){
    if(_b){
        bHasBody=true;

        for(int i=0;i<movers.size();i++){
            movers[i]->setPosition(0, 0);
            movers[i]->setMaxSpeed(80);
        }
        
        // translate to spineBase
        ofVec2f headRadiusShift=neck-spineBase;
        headRadiusShift.y-=actualHeadRadius;
        ofVec2f p=getPosition();
        p-=headRadiusShift;
        setPosition(p);
        bodySound.play();

    }else{
        bHasBody=false;
    }
}


void Humania::bindPosition(bool _b){
    bBindPosition=_b;
}

void Humania::saveStartposition(ofVec2f _p){
    startposition->set(_p.x,_p.y);
    Settings::get().save("data.json");
}


void Humania::resetToStart(){
    cout<<"reset to start"<<startposition->x<<endl;
    setPosition(startposition->x, startposition->y);
    setTarget(ofVec2f(startposition->x, startposition->y));
    
    leftEyeMover.setTarget(leftEyeTarget);
    rightEyeMover.setTarget(rightEyeTarget);
    
    showCheeks(false);
    showNose(false);
    bHasMouth=false;
    bMouthIsBound=false;
    bEyesAreBound=false;
    showHair(false);
    bHasBody=false;
    
   // setState(IDLE);
    
}



void Humania::drawBody(){
    ofSetColor(255);

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
}



void Humania::drawFaceAvatar(){
    
    headOffset=ofVec2f(0,0);
    ofPushMatrix();
    ofPushStyle();
    ofVec2f headCenter=relativeHeadPosition+headOffset;
    ofSetColor(255);
    ofDrawCircle(headCenter,actualHeadRadius);
    ofSetColor(0);

    
    ofPushMatrix();
    ofSetColor(0);
    //ofTranslate(headOffset);
    ofScale(scaleFactor, scaleFactor);

    ofTranslate(leftEyeMover.getPosition().x,leftEyeMover.getPosition().y);
    ofDrawEllipse(0,0,actualEyeDiameter*2,actualEyeRadius*2);
    ofPopMatrix();
    
    
    ofPushMatrix();
    ofSetColor(0);
   // ofTranslate(headOffset);
    ofScale(scaleFactor, scaleFactor);
    ofTranslate(rightEyeMover.getPosition().x,rightEyeMover.getPosition().y);
    ofDrawEllipse(0,0,actualEyeDiameter*2,actualEyeRadius*2);
    ofPopMatrix();
 
    
    

    
    ofPushMatrix();
    ofScale(scaleFactor, scaleFactor);
    //Cheeks
    ofSetColor(255,0,0,actualCheeksAlpha);
    ofDrawCircle(headCenter.x-50,headCenter.y+30,20);
    ofDrawCircle(headCenter.x+50,headCenter.y+30,20);
   
    
    ofSetColor(0,actualNoseAlpha);
    ofDrawTriangle(headCenter.x, headCenter.y, headCenter.x+20, headCenter.y+40,headCenter.x-20, headCenter.y+40);
    ofPopMatrix();
    
    
    
    
    
   // if(bHasHair){
        // Hair
    
    ofPushMatrix();

    ofScale(scaleFactor, scaleFactor);
        ofSetColor(0,actualHairAlpha);
        ofNoFill();
        ofSetLineWidth(3*scaleFactor);
        ofDrawBezier(headCenter.x,headCenter.y-80,headCenter.x,headCenter.y-40,headCenter.x-40,headCenter.y-20,headCenter.x-80,headCenter.y-20);
        ofDrawBezier(headCenter.x,headCenter.y-80,headCenter.x,headCenter.y-40,headCenter.x+40,headCenter.y-20,headCenter.x+80,headCenter.y-20);
      
        ofSetColor(255);
        ofFill();
        
        
        ofVec2f dist=ofVec2f(headRadiusNormal,0);
        dist.rotate(-15);
        for(int i=0;i<6;i++){
            ofVec2f pos=headCenter;
            pos+=dist;
            ofSetColor(0,actualHairAlpha);
            ofDrawCircle(pos,20);
            ofSetColor(255,actualHairAlpha);
            ofDrawCircle(pos,17);
            dist.rotate(actualHairRotation/6);
        }
    ofPopMatrix();
  //  }
    
    if(bHasMouth){
        ofPushMatrix();

        ofScale(scaleFactor, scaleFactor);
        mouth.clear();
        mouth.moveTo(leftMouth);
        
        ofVec2f dist=(leftMouth-mouthCenterPosition);
        ofVec2f cp1=ofVec2f(leftMouth.x,leftMouth.y-dist.y/2);
        ofVec2f cp2=ofVec2f(mouthCenterPosition.x+(dist.x/2),mouthCenterPosition.y);

       // mouth.bezierTo(cp1,cp2, mouthCenterPosition);
        
        ofSetColor(0);

        ofPolyline rough;
        ofMesh smooth;
        rough.clear();
        smooth.clear();
        rough.addVertex(leftMouth);
        rough.bezierTo(cp1,cp2, mouthCenterPosition);
        ofxPolyToMesh(smooth, rough, 2);
        smooth.draw();
        
        
        
         dist=(rightMouth-mouthCenterPosition);
         cp1=ofVec2f(rightMouth.x,rightMouth.y-dist.y/2);
         cp2=ofVec2f(mouthCenterPosition.x+(dist.x/2),mouthCenterPosition.y);
        
        rough.clear();
        smooth.clear();
        rough.addVertex(rightMouth);
        rough.bezierTo(cp1,cp2, mouthCenterPosition);
        ofxPolyToMesh(smooth, rough, 2);
        smooth.draw();
        
       // mouth.bezierTo(ofVec2f(leftMouth.x,leftMouth.y+(mouthCenterPosition.y-leftMouth.y)),ofVec2f(mouthCenterPosition.x-20,mouthCenterPosition.y), mouthCenterPosition);
      //  mouth.bezierTo(ofVec2f(mouthCenterPosition.x+20,mouthCenterPosition.y),ofVec2f(rightMouth.x,rightMouth.y+(mouthCenterPosition.y-rightMouth.y)), rightMouth);
        
        //mouth.bezierTo(cp2,cp1, rightMouth);
    
        //mouth.setStrokeColor(ofColor(0,0,0));
        //mouth.setFilled(false);
        //mouth.setStrokeWidth(3*scaleFactor);
       // mouth.draw();
        
        
     
        
        ofPopMatrix();
    }
    


    ofPopStyle();
    ofPopMatrix();
    
    
    
}
