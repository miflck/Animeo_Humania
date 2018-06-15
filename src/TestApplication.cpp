//
//  TestApplication.cpp
//  appswitcher
//
//  Created by Flückiger Michael on 11.06.18.
//

#include "TestApplication.hpp"
#include "ApplicationController.h"
#include "KinectV2Manager.hpp"


TestApplication::TestApplication(){
    init();
}


TestApplication::~TestApplication(){
    
}

void TestApplication::init(){
    cout<<"init TestApplication"<<endl;
    bAddedListeners = false;
    mover.setup();
    mover.setTarget(ofVec2f(ofGetWidth(),ofGetHeight()));
}

void TestApplication::update(){
    
    
    vector<MappedPoints> mskel=KINECTMANAGER->getMappedSkelettons();
    
    for(int i=0;i<mskel.size();i++){
        mskel[i].drawSkeletton();
    }
    
    if(mskel.size()>skelettId){
    //for(int i=0;i<mskel.size();i++){
        mover.setTarget(mskel[skelettId].leftHand);
   // }
    }
    
    mover.update();
    
  if(bSendOSCPosition)APPC->oscmanager.sendPositionToLayer(m8layer,mover.getPosition().x-ofGetWidth()/2,-mover.getPosition().y+ofGetHeight()/2);

}


void TestApplication::draw(){
    mover.draw();
}

void TestApplication::exit(){
    cout<<"exit TestApplication"<<endl;
}



//KEY LISTENER
//--------------------------------------------------------------
void TestApplication::keyPressed(ofKeyEventArgs &e){
    if(e.key==OF_KEY_LEFT){
        mover.applyForce(ofVec2f(-1,0));
    }
    if(e.key==OF_KEY_RIGHT){
        mover.applyForce(ofVec2f(1,0));
    }
    
    if(e.key==OF_KEY_UP){
        mover.applyForce(ofVec2f(0,-1));
    }
    if(e.key==OF_KEY_DOWN){
        mover.applyForce(ofVec2f(0,1));
    }
    if(e.key=='t'){
        mover.bSeekTarget=!mover.bSeekTarget;
    }
    if(e.key=='m'){
        mover.bSeekMouse=!mover.bSeekMouse;
    }

    if(e.key=='o'){
        bSendOSCPosition=!bSendOSCPosition;
    }
    
    
    if(e.key=='+'){
        m8layer++;
        
    }
    
    if(e.key=='-'){
        m8layer--;
        
    }
        
    if(e.key=='0'){
        skelettId=0;
    }
    if(e.key=='1'){
        skelettId=1;
    }
    
}



void TestApplication::turnOn(){
    if(!bAddedListeners){
        addListeners();
    }
    bAddedListeners=true;
}

void TestApplication::turnOff(){
    if(bAddedListeners){
        removeListeners();
    }
    bAddedListeners=false;
}


//--------------------------------------------------------------
void TestApplication::mouseMoved(ofMouseEventArgs &a){
    
}

//--------------------------------------------------------------
void TestApplication::mouseDragged(ofMouseEventArgs &a){
    
}

//--------------------------------------------------------------
void TestApplication::mousePressed(ofMouseEventArgs &a){
    mover.setTarget(ofVec2f(a.x,a.y));
}

//--------------------------------------------------------------
void TestApplication::mouseReleased(ofMouseEventArgs &a){
    
}

void TestApplication::mouseScrolled(ofMouseEventArgs &a){
    
}

//--------------------------------------------------------------
void TestApplication::mouseEntered(ofMouseEventArgs &a){
    
}

//--------------------------------------------------------------
void TestApplication::mouseExited(ofMouseEventArgs &a){
    
}
