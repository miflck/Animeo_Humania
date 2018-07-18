//
//  Face.cpp
//  appswitcher
//
//  Created by Flückiger Michael on 11.06.18.
//

#include "Face.hpp"
#include "ApplicationController.h"

#include "KinectV2Manager.hpp"


Face::Face(){
    init();
}


Face::~Face(){
    
}

void Face::init(){
    cout<<"init Face"<<endl;
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
}

void Face::update(){
    box2d.update();
  
    mouth.clear();
    
  
 

    
    vector<MappedPoints> mskel=KINECTMANAGER->getMappedSkelettons();
    if(mskel.size()>0){
        ofVec2f head;
        head=mskel[0].head;
        head=ofVec2f(head.x, head.y);
        
         headPosition=ofVec2f(mskel[0].head.x,mskel[0].head.y);
         leftHandPosition=ofVec2f(mskel[0].leftHand.x,mskel[0].leftHand.y);
         rightHandPosition=ofVec2f(mskel[0].rightHand.x,mskel[0].rightHand.y);
         spineBasePosition=ofVec2f(mskel[0].spineBase.x,mskel[0].spineBase.y);
        spineMidPosition=ofVec2f(mskel[0].spineMid.x,mskel[0].spineMid.y);
        leftShoulderPosition=ofVec2f(mskel[0].leftShoulder.x,mskel[0].leftShoulder.y);
        rightShoulderPosition=ofVec2f(mskel[0].rightShoulder.x,mskel[0].rightShoulder.y);
        leftEllbowPositon=ofVec2f(mskel[0].leftEllbow.x,mskel[0].leftEllbow.y);
        rightEllbowPositon=ofVec2f(mskel[0].rightEllbow.x,mskel[0].rightEllbow.y);
        
        
        eyeOffset=ofVec2f(headPosition.x-spineBasePosition.x,(headPosition.y-spineBasePosition.y)/10);
        leftMouthOffset=leftHandPosition-leftEllbowPositon;
        rightMouthOffset=rightHandPosition-rightEllbowPositon;
        
        leftMouthCornerOffset=leftHandPosition-spineBasePosition;
        rightMouthCornerOffset=rightHandPosition-spineBasePosition;
        
        
       // leftMouth=ofVec2f(leftMouth.x,leftMouthOffset.y+mouthCenterPosition.y);
       // rightMouth=ofVec2f(rightMouth.x,rightMouthOffset.y+mouthCenterPosition.y);
        
        leftMouth=ofVec2f(mouthCenterPosition.x+leftMouthCornerOffset.x,leftMouthOffset.y+mouthCenterPosition.y);
        rightMouth=ofVec2f(mouthCenterPosition.x+rightMouthCornerOffset.x,rightMouthOffset.y+mouthCenterPosition.y);
        
        mouth.moveTo(leftMouth);
        mouth.bezierTo(ofVec2f(leftMouth.x,leftMouth.y+(mouthCenterPosition.y-leftMouth.y)),ofVec2f(mouthCenterPosition.x+50,mouthCenterPosition.y), mouthCenterPosition);
        mouth.bezierTo(ofVec2f(mouthCenterPosition.x-50,mouthCenterPosition.y),ofVec2f(rightMouth.x,rightMouth.y+(mouthCenterPosition.y-rightMouth.y)), rightMouth);
       
     
    }
    

    // remove shapes offscreen
    ofRemove(boxes, ofxBox2dBaseShape::shouldRemoveOffScreen);
    ofRemove(circles, ofxBox2dBaseShape::shouldRemoveOffScreen);
    
  
}


void Face::draw(){
    ofPushStyle();
    ofPushMatrix();
    ofTranslate(facePosition.x, facePosition.y);
    ofSetColor(255);
    ofDrawCircle(0, 0, 300);
    ofSetColor(255,0,0);
    
    ofSetLineWidth(5);
   
    ofSetColor(0);
    
    mouth.setStrokeColor(ofColor(0,0,0));
    mouth.setFilled(false);
    mouth.setStrokeWidth(5);
    mouth.draw();
  
    ofPushMatrix();
    ofSetColor(0);

    ofTranslate(eyeOffset.x,-80);
    ofDrawCircle(100, 0, 50);
    ofDrawCircle(-100, 0, 50);
    ofPopMatrix();
    
    ofPopMatrix();
    ofPopStyle();

    if(APPC->debug){
        ofPushStyle();
        for(int i=0; i<circles.size(); i++) {
            ofFill();
            ofSetHexColor(0x01b1f2);
            circles[i].get()->draw();
        }
        ofPopStyle();
    }
    
    


   
    
   /*   screen.begin();

  ofPushStyle();
    ofSetColor(255,100);
    ofDrawCircle(ofGetMouseX(), ofGetMouseY(), 10);
    screen.end();
   
    screen.draw(0,0);
   */
    
    

    
    
}

void Face::exit(){
    cout<<"exit Face"<<endl;
}



//KEY LISTENER
//--------------------------------------------------------------
void Face::keyPressed(ofKeyEventArgs &e){
    if(e.key == 'e') {
        bHasEyes=!bHasEyes;
    }
    
    
    if(e.key == 'f') {
        bHasFace=!bHasFace;

    }
    
    if(e.key == 'g') {
        bHasMouth=!bHasMouth;

    }
    
}


//--------------------------------------------------------------
void Face::mouseMoved(ofMouseEventArgs &a){

}

//--------------------------------------------------------------
void Face::mouseDragged(ofMouseEventArgs &a){
    
}

//--------------------------------------------------------------
void Face::mousePressed(ofMouseEventArgs &a){
   

}

//--------------------------------------------------------------
void Face::mouseReleased(ofMouseEventArgs &a){
    
}

void Face::mouseScrolled(ofMouseEventArgs &a){
    
}

//--------------------------------------------------------------
void Face::mouseEntered(ofMouseEventArgs &a){
    
}

//--------------------------------------------------------------
void Face::mouseExited(ofMouseEventArgs &a){
    
}


void Face::toggleMouseActive(){
    bIsMouseActive=!bIsMouseActive;
}


void Face::turnOn(){
    if(!bAddedListeners){
        addListeners();
    }
    bAddedListeners=true;
}

void Face::turnOff(){
    if(bAddedListeners){
        removeListeners();
    }
    bAddedListeners=false;
}
