//
//  PhysicsWorld.cpp
//  appswitcher
//
//  Created by Flückiger Michael on 11.06.18.
//

#include "PhysicsWorld.hpp"
#include "ApplicationController.h"

#include "KinectV2Manager.hpp"


PhysicsWorld::PhysicsWorld(){
    init();
}


PhysicsWorld::~PhysicsWorld(){
    
}

void PhysicsWorld::init(){
    cout<<"init PhysicsWorld"<<endl;
    bAddedListeners = false;
    
    box2d.init();
    box2d.setGravity(0, 10);
    box2d.createGround();
    box2d.setFPS(60.0);
    box2d.registerGrabbing();
    anchor.setPhysics(50, 0.5, 0.9);
    anchor.setup(box2d.getWorld(), 0, 0, 50);
    box.setup(box2d.getWorld(), ofGetWidth()/2, -20, ofGetWidth(), 20);
    
}

void PhysicsWorld::update(){
    box2d.update();
    
    vector<MappedPoints> mskel=KINECTMANAGER->getMappedSkelettons();
    if(mskel.size()>0){
        ofVec2f head;
        head=mskel[0].head;
        head=ofVec2f(head.x, head.y);

        anchor.setPosition(mskel[0].leftHand.x, mskel[0].leftHand.y);
        
        box2d.setGravity(0, (mskel[0].head.y-mskel[0].rightHand.y)/10);

    
    float r = ofRandom(10, 40);        // a random radius 4px - 20px
    circles.push_back(shared_ptr<ofxBox2dCircle>(new ofxBox2dCircle));
    circles.back().get()->setPhysics(3.0, 0.8, 0.1);
    circles.back().get()->setup(box2d.getWorld(), head.x,head.y, r);
    circles.back().get()->setVelocity(ofRandom(-5,5), ofRandom(-1,-5));
    }
    // remove shapes offscreen
    ofRemove(boxes, ofxBox2dBaseShape::shouldRemoveOffScreen);
    ofRemove(circles, ofxBox2dBaseShape::shouldRemoveOffScreen);
    
  
}


void PhysicsWorld::draw(){
   
    cout<<circles.size()<<endl;

    if(APPC->debug){
        ofPushStyle();
        for(int i=0; i<circles.size(); i++) {
            ofFill();
            ofSetHexColor(0x01b1f2);
            circles[i].get()->draw();
        }
        ofSetColor(255,0,0);
        anchor.draw();
        ofPopStyle();
    }
    
   
    
}

void PhysicsWorld::exit(){
    cout<<"exit PhysicsWorld"<<endl;
}



//KEY LISTENER
//--------------------------------------------------------------
void PhysicsWorld::keyPressed(ofKeyEventArgs &e){
    if(e.key == 'e') {
        
        vector<MappedPoints> mskel=KINECTMANAGER->getMappedSkelettons();
        ofVec2f head=ofVec2f(ofGetMouseX(), ofGetMouseY());
        if(mskel.size()>0){
            head=mskel[0].head;
            head=ofVec2f(head.x, head.y);
        }
        
        cout<<"Add Circles"<<endl;
        float r = ofRandom(10, 40);        // a random radius 4px - 20px
        circles.push_back(shared_ptr<ofxBox2dCircle>(new ofxBox2dCircle));
        circles.back().get()->setPhysics(3.0, 0.53, 0.1);
        circles.back().get()->setup(box2d.getWorld(), ofGetMouseX(), ofGetMouseY(), r);
        circles.back().get()->setVelocity(ofRandom(-5,5), ofRandom(-1,-5));

    }
}


//--------------------------------------------------------------
void PhysicsWorld::mouseMoved(ofMouseEventArgs &a){

}

//--------------------------------------------------------------
void PhysicsWorld::mouseDragged(ofMouseEventArgs &a){
    
}

//--------------------------------------------------------------
void PhysicsWorld::mousePressed(ofMouseEventArgs &a){
   

}

//--------------------------------------------------------------
void PhysicsWorld::mouseReleased(ofMouseEventArgs &a){
    
}

void PhysicsWorld::mouseScrolled(ofMouseEventArgs &a){
    
}

//--------------------------------------------------------------
void PhysicsWorld::mouseEntered(ofMouseEventArgs &a){
    
}

//--------------------------------------------------------------
void PhysicsWorld::mouseExited(ofMouseEventArgs &a){
    
}


void PhysicsWorld::toggleMouseActive(){
    bIsMouseActive=!bIsMouseActive;
}


void PhysicsWorld::turnOn(){
    if(!bAddedListeners){
        addListeners();
    }
    bAddedListeners=true;
}

void PhysicsWorld::turnOff(){
    if(bAddedListeners){
        removeListeners();
    }
    bAddedListeners=false;
}
