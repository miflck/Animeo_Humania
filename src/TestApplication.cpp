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
    
    //ofBackground(255,0,0);
    vector<MappedPoints> mskel=KINECTMANAGER->getMappedSkelettons();
    
    switch (skelettonNodeId) {
        case 5:
            mover.setTarget(ofVec2f(ofGetMouseX(),ofGetMouseY()));
            break;
    }
    
    if(mskel.size()>skelettId){
    //for(int i=0;i<mskel.size();i++){
        switch (skelettonNodeId) {
            case 8:
                   mover.setTarget(mskel[skelettId].head);
                break;
                
            case 6:
                mover.setTarget(mskel[skelettId].leftHand);
                break;
                
            case 4:
                mover.setTarget(mskel[skelettId].rightHand);
                break;
                
       
                
            default:
                break;
        }
     
   // }
    }
    
    mover.update();
    
  if(bSendOSCPosition)APPC->oscmanager.sendPositionToLayer(m8layer,mover.getPosition().x-ofGetWidth()/2,-mover.getPosition().y+ofGetHeight()/2);

}


void TestApplication::draw(){
    ofPushStyle();
    mover.draw();
    vector<MappedPoints> mskel=KINECTMANAGER->getMappedSkelettons();
    ofSetLineWidth(8);
    ofPushMatrix();
 
   /* for(int i=0;i<mskel.size();i++){
        ofPushMatrix();
        mskel[i].drawSkeletton();
        ofPopMatrix();
    }*/
    ofPopMatrix();
    ofPopStyle();
}

void TestApplication::exit(){
    cout<<"exit TestApplication"<<endl;
}



//KEY LISTENER
//--------------------------------------------------------------
void TestApplication::keyPressed(ofKeyEventArgs &e){
    cout<<e.key<<endl;
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
    if(e.key=='z'){
        mover.bSeekMouse=!mover.bSeekMouse;
    }

    if(e.key=='o'){
        bSendOSCPosition=!bSendOSCPosition;
    }
    
    if(e.key=='8'){
        skelettonNodeId=8;
    }
 
    if(e.key=='6'){
        skelettonNodeId=6;
    }
    
    if(e.key=='4'){
        skelettonNodeId=4;
    }
    
    if(e.key=='5'){
        skelettonNodeId=5;
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
    
    
    if(e.key=='s'){
        mover.scaleTo(300,2.f);
    }
    
    if(e.key=='a'){
        mover.scaleTo(600,50.f);
    }
    
    if(e.key=='d'){
        mover.scaleTo(50,0.5f);
    }
    
    if(e.key=='q'){
        mover.setSeekForce(5);
    }
    if(e.key=='w'){
        mover.setSeekForce(0.5);
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
