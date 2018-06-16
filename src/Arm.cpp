//
//  Arm.cpp
//  appswitcher
//
//  Created by Flückiger Michael on 11.06.18.
//

#include "Arm.hpp"
#include "ApplicationController.h"

#include "KinectV2Manager.hpp"


Arm::Arm(){
    init();
}


Arm::~Arm(){
    
}

void Arm::init(){
    cout<<"init Arm"<<endl;
    bAddedListeners = false;
    
    box2d.init();
    box2d.setGravity(0, 0);
    box2d.createGround();

    box2d.setFPS(60.0);
    box2d.registerGrabbing();
    
    
    float xPos=(ofGetWidth()/3)*2;
    
    anchor.setPhysics(50, 0.5, 0.9);

    anchor.setup(box2d.getWorld(), xPos, 0, 10);
    anchor2.setup(box2d.getWorld(), ofGetWidth(), 0, 10);
    
    
    int num=20;
    int jointlength=45;
    
    // first we add just a few circles
    for(int i=0; i<num; i++) {
        
        auto circle = std::make_shared<ofxBox2dCircle>();
        
        // if(i%2==0){
        //   circle.get()->setPhysics(10.0, 0.53, 0.9);
        
        // }else{
        circle.get()->setPhysics(10, 0.53, 0.9);
        
        // }
        circle.get()->setup(box2d.getWorld(), xPos, (ofGetHeight()/(num+1))+(ofGetHeight()/(num+1))*i, 5);
        circles.push_back(circle);
        positions.push_back(ofVec2f(xPos, (ofGetHeight()/(num+1))+(ofGetHeight()/(num+1))*i));
    }
    
    // now connect each circle with a joint
    for(int i=0; i<circles.size(); i++) {
        auto joint = std::make_shared<ofxBox2dJoint>();
        // if this is the first point connect to the top anchor.
        if(i == 0) {
            joint.get()->setup(box2d.getWorld(), anchor.body, circles[i].get()->body,15,10);
        }
        else {
            joint.get()->setup(box2d.getWorld(), circles[i-1].get()->body, circles[i].get()->body,15,10);
        }
       joint.get()->setLength(jointlength);
        joints.push_back(joint);
    }
    
    auto joint = std::make_shared<ofxBox2dJoint>();
    joint.get()->setup(box2d.getWorld(), circles[circles.size()-1].get()->body, anchor2.body,15,10);
    joint.get()->setLength(jointlength);
    
    joints.push_back(joint);
    
    
}

void Arm::update(){
    box2d.update();
    
    ofVec2f mouse(ofGetMouseX(), ofGetMouseY());
    float minDis = ofGetMousePressed() ? 100 : 50;
    
    vector<MappedPoints> mskel=KINECTMANAGER->getMappedSkelettons();
    ofVec2f leftHand;
    if(mskel.size()>0){
         leftHand=mskel[0].leftHand;
    }
    
    for(int i=0; i<circles.size(); i++) {
       /* float dis = mouse.distance(circles[i].get()->getPosition());
        if(dis < minDis && bIsMouseActive) circles[i].get()->addRepulsionForce(mouse,10);
            if(mskel.size()>0){
                float handDist = leftHand.distance(circles[i].get()->getPosition());
                if(dis < minDis) circles[i].get()->addRepulsionForce(leftHand,10);
            }*/
       circles[i].get()->setDamping(0.9);
    }
    
    
    
    if( bUseHand){
        if(mskel.size()>0 ){
        if(repulse){
            anchor.addRepulsionForce(leftHand,10);
        }else{
            anchor.addAttractionPoint(leftHand,1000);
        }}
    }else{
        if(repulse){
            anchor.addRepulsionForce(mouse,50);
        }else{
            anchor.addAttractionPoint(mouse,50);
        }
    }
    
    anchor2.setDamping(0.9);

    //ofVec2f distance=mouse-anchor.getPosition();
  //  anchor2.setVelocity(distance.normalize()*-1);
    ofPolyline cur;
    
    cur.addVertex(anchor.getPosition());
    
    for(int i=0; i<circles.size(); i++) {
        cur.addVertex( circles[i].get()->getPosition());
    }
    
    cur.addVertex(anchor2.getPosition());
    
    line=cur;
}


void Arm::draw(){
    ofSetColor(0);
    ofDrawRectangle(0,0,ofGetWidth(),ofGetHeight());
    
    
    ofPushStyle();
    ofSetColor(255);
    ofSetLineWidth(20);
    line.draw();
    
    ofDrawCircle(anchor.getPosition().x,anchor.getPosition().y,50);
    ofPopStyle();
    
    
    ofSetHexColor(0xf2ab01);


    if(APPC->debug){
        ofPushStyle();

        anchor.draw();
        anchor2.draw();
        
       ofDrawRectangle(anchor.getPosition().x,anchor.getPosition().y,10,10);
        
            for(int i=0; i<circles.size(); i++) {
            ofFill();
            ofSetHexColor(0x01b1f2);
             circles[i].get()->draw();
        }
        
        for(int i=0; i<joints.size(); i++) {
            ofSetHexColor(0x444342);
              joints[i].get()->draw();
        }
        ofPopStyle();

    }
    
   // line=line.getResampledBySpacing(20);
  // line=line.getSmoothed(10);
    


    
    
    string info = "";
    info += "Press [n] to add a new joint\n";
    info += "click and pull the chain around\n";
    info += "FPS: "+ofToString(ofGetFrameRate(), 1)+"\n";
    ofSetHexColor(0x444342);
    ofDrawBitmapString(info, 30, 30);
    
}

void Arm::exit(){
    cout<<"exit Arm"<<endl;
}



//KEY LISTENER
//--------------------------------------------------------------
void Arm::keyPressed(ofKeyEventArgs &e){
   if(e.key=='r'){
        anchor.setPhysics(10, 0.5, 0.9);
        anchor.body->SetType(b2_dynamicBody);
        
    }
    
    
    if(e.key=='R'){
        anchor2.setPhysics(10, 1, 10);
        anchor2.body->SetType(b2_dynamicBody);
        
    }
    
    if(e.key=='l'){
        bUseHand=!bUseHand;
    }
    
    if(e.key=='h'){
        anchor.setPosition(0, ofGetHeight()/2);
        anchor.setPhysics(0, 0.5, 0.9);
        anchor.body->SetType(b2_staticBody);

        anchor2.setPosition(ofGetWidth(), ofGetHeight()/2);
        anchor2.setPhysics(0, 0.5, 0.9);
        anchor2.body->SetType(b2_staticBody);
        
    }
    
    
    if(e.key=='a'){
        anchor2.setPosition(ofGetMouseX(), ofGetMouseY());
    }
    
    
    if(e.key=='s'){
        anchor.setPosition(ofGetMouseX(), ofGetMouseY());
    }
    
    if(e.key=='w'){
        repulse=!repulse;
    }
    
    
    if(e.key=='-'){
        for(int i=0; i<joints.size(); i++) {
            joints[i].get()->setFrequency(joints[i].get()->getFrequency()-0.1);
        }
        cout<<joints[0].get()->getFrequency()<<endl;
    }
    
    if(e.key=='+'){
        for(int i=0; i<joints.size(); i++) {
            joints[i].get()->setFrequency(joints[i].get()->getFrequency()+0.1);
        }
        cout<<joints[0].get()->getFrequency()<<endl;
    }
    
   /* if(e.key=='d'){
        for(int i=0; i<joints.size(); i++) {
            joints[i].get()->setDamping( joints[i].get()->getDamping()+0.01);
        }
        cout<<joints[0].get()->getDamping()<<endl;
    }
    if(e.key=='D'){
        for(int i=0; i<joints.size(); i++) {
            joints[i].get()->setDamping( joints[i].get()->getDamping()-0.01);
        }
        cout<<joints[0].get()->getDamping()<<endl;
    }
    */
    
    if(e.key=='m'){
        toggleMouseActive();
    }
    
    
}


//--------------------------------------------------------------
void Arm::mouseMoved(ofMouseEventArgs &a){

}

//--------------------------------------------------------------
void Arm::mouseDragged(ofMouseEventArgs &a){
    
}

//--------------------------------------------------------------
void Arm::mousePressed(ofMouseEventArgs &a){
    cout<<"Mouse"<< anchor.isFixed()<<endl;
    anchor.setPhysics(0, 0.5, 0.9);
    anchor.body->SetType(b2_staticBody);
    //anchor.setup(box2d.getWorld(), a.x, 0, 10);
    

}

//--------------------------------------------------------------
void Arm::mouseReleased(ofMouseEventArgs &a){
    
}

void Arm::mouseScrolled(ofMouseEventArgs &a){
    
}

//--------------------------------------------------------------
void Arm::mouseEntered(ofMouseEventArgs &a){
    
}

//--------------------------------------------------------------
void Arm::mouseExited(ofMouseEventArgs &a){
    
}


void Arm::toggleMouseActive(){
    bIsMouseActive=!bIsMouseActive;
}


void Arm::turnOn(){
    if(!bAddedListeners){
        addListeners();
    }
    bAddedListeners=true;
}

void Arm::turnOff(){
    if(bAddedListeners){
        removeListeners();
    }
    bAddedListeners=false;
}
