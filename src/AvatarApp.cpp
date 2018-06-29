//
//  AvatarApp.cpp
//  appswitcher
//
//  Created by Flückiger Michael on 11.06.18.
//

#include "AvatarApp.hpp"
#include "ApplicationController.h"

#include "KinectV2Manager.hpp"


AvatarApp::AvatarApp(){
    init();
}


AvatarApp::~AvatarApp(){
    
}

void AvatarApp::init(){
    cout<<"init AvatarApp"<<endl;
    bAddedListeners = false;
    
    box2d.init();
    box2d.setGravity(0, 10);
    box2d.createGround();
    box2d.setFPS(60.0);
    box2d.registerGrabbing();
    
    
   
    screen.allocate(1920,1080, GL_RGB);
    screen.begin();
    ofClear(0,0,0,0);
    screen.end();
    
    
    facePosition=ofVec2f(ofGetWidth()/3,ofGetHeight()/2);
    
    mouthCenterPosition=ofVec2f(0,150);
    leftMouth=ofVec2f(200,0);
    rightMouth=ofVec2f(-200,0);
    
    leftHandMover.bSeekTarget=true;
    leftHandMover.setSeekForce(2);
    
    
    avatar.setup();
    avatar.bSeekTarget=true;
    
}

void AvatarApp::update(){
    avatar.setTarget(ofVec2f(ofGetMouseX(),ofGetMouseY()));
    
    
    vector<MappedPoints> mskel=KINECTMANAGER->getMappedSkelettons();
    if(mskel.size()>0){
        avatar.setTarget(mskel[0].spineBase);

        
    }
    
    
    avatar.update();
    box2d.update();
  
    ofVec2f offset(250,0);
    if(avatars.size()>0){
        avatars[0]->setTarget(avatar.getPosition());
        avatars[0]->update();

        for(int i=1;i<avatars.size();i++){
            avatars[i]->setTarget(avatars[i-1]->getPosition()-offset);
            avatars[i]->update();
        }
    }
  
 

    
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
        
        
        leftHandMover.setTarget(leftHand);
        
     
    }
    

    // remove shapes offscreen
    ofRemove(boxes, ofxBox2dBaseShape::shouldRemoveOffScreen);
    ofRemove(circles, ofxBox2dBaseShape::shouldRemoveOffScreen);
    
  
}


void AvatarApp::draw(){
    
    for(int i=avatars.size()-1;i>=0;i--){
        avatars[i]->draw();
    }

    avatar.draw();
    
    /*for(int i=0;i<avatars.size();i++){
        avatars[i]->draw();
    }*/
    
  
   /* ofPushMatrix();
    ofTranslate(-300,0);
    ofSetLineWidth(10);
    ofDrawCircle(leftHandMover.getPosition(),10);
    ofDrawLine(leftHand,leftEllbow);
    ofDrawCircle(leftEllbow,5);

    ofDrawLine(leftEllbow,leftShoulder);
    
    
    ofDrawLine(rightShoulder,leftShoulder);

    
    ofDrawCircle(leftShoulder,5);

    ofSetColor(255);
    ofVec2f headCenter=ofVec2f(head.x,head.y-40);

    ofDrawCircle(headCenter,80);
    ofSetColor(0);

    

    ofDrawCircle(head.x-20,head.y-60,10);
    ofDrawCircle(head.x+20,head.y-60,10);
    
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
    ofSetColor(255,0,0);

    ofDrawCircle(headCenter.x-40,headCenter.y+10,20);
    ofDrawCircle(headCenter.x+40,headCenter.y+10,20);

    
    ofSetColor(255);

    ofDrawLine(leftShoulder,neck);
    ofDrawCircle(neck,5);

    
    ofDrawCircle(rightHand,10);
    ofDrawLine(rightHand,rightEllbow);
    ofDrawLine(rightEllbow,rightShoulder);
    ofDrawCircle(rightShoulder,5);
    ofDrawCircle(rightEllbow,5);

    
    ofDrawLine(rightShoulder,neck);
    ofDrawLine(neck,spineMid);
    
    ofDrawLine(spineMid,spineBase);

    
    //ofDrawTriangle(neck,leftHip,rightHip);
    
    ofDrawLine(spineBase,leftHip);
    ofDrawLine(spineBase,rightHip);
    
    ofDrawLine(rightHip,rightKnee);
    ofDrawLine(rightKnee,rightFoot);
    
    ofDrawLine(leftHip,leftKnee);
    ofDrawLine(leftKnee,leftFoot);
    ofPopMatrix();
*/
    if(APPC->debug){
      
    }
    
    
    vector<MappedPoints> mskel=KINECTMANAGER->getMappedSkelettons();
    if(mskel.size()>0){
        ofPushMatrix();
        ofTranslate(ofGetWidth(), ofGetHeight());
        ofDrawCircle(mskel[0].localZero,10);
        ofDrawCircle(mskel[0].leftHandLocal,10);
        ofPopMatrix();
        
        ofDrawCircle(mskel[0].head,10);
        ofDrawCircle(mskel[0].leftHand,10);
        
    }

   
    

    

    
    
}

void AvatarApp::exit(){
    cout<<"exit AvatarApp"<<endl;
}



//KEY LISTENER
//--------------------------------------------------------------
void AvatarApp::keyPressed(ofKeyEventArgs &e){
    if(e.key == 'e') {
        bHasEyes=!bHasEyes;
    }
    
    
    if(e.key == 'f') {
        bHasFace=!bHasFace;

    }
    
    if(e.key == 'g') {
        bHasMouth=!bHasMouth;

    }
    
    if(e.key=='a'){
        Avatar * a =new Avatar();
        a->setup();
        a->setTarget(ofVec2f(ofRandom(ofGetWidth()),ofRandom(ofGetHeight())));
        a->bSeekTarget=true;
        
        a->setSeekForce(50);
        avatars.push_back(a);
    }
    
}


//--------------------------------------------------------------
void AvatarApp::mouseMoved(ofMouseEventArgs &a){

}

//--------------------------------------------------------------
void AvatarApp::mouseDragged(ofMouseEventArgs &a){
    
}

//--------------------------------------------------------------
void AvatarApp::mousePressed(ofMouseEventArgs &a){
   

}

//--------------------------------------------------------------
void AvatarApp::mouseReleased(ofMouseEventArgs &a){
    
}

void AvatarApp::mouseScrolled(ofMouseEventArgs &a){
    
}

//--------------------------------------------------------------
void AvatarApp::mouseEntered(ofMouseEventArgs &a){
    
}

//--------------------------------------------------------------
void AvatarApp::mouseExited(ofMouseEventArgs &a){
    
}


void AvatarApp::toggleMouseActive(){
    bIsMouseActive=!bIsMouseActive;
}


void AvatarApp::turnOn(){
    if(!bAddedListeners){
        addListeners();
    }
    bAddedListeners=true;
}

void AvatarApp::turnOff(){
    if(bAddedListeners){
        removeListeners();
    }
    bAddedListeners=false;
}
