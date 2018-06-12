//
//  Physics.cpp
//  appswitcher
//
//  Created by Flückiger Michael on 11.06.18.
//

#include "Physics.hpp"
#include "ApplicationController.h"

Physics::Physics(){
    init();
}


Physics::~Physics(){
    
}

void Physics::init(){
    cout<<"init Physics"<<endl;
    bAddedListeners = false;
    
    box2d.init();
    box2d.setGravity(0, 0);
    box2d.createGround();

    box2d.setFPS(60.0);
    box2d.registerGrabbing();
    
    
    float xPos=(ofGetWidth()/3)*2;
    anchor.setup(box2d.getWorld(), xPos, 0, 10);
    anchor2.setup(box2d.getWorld(), xPos, ofGetHeight(), 10);
    
    
    int num=30;
    int jointlength=(ofGetHeight()/(num+1));
    
    // first we add just a few circles
    for(int i=0; i<num; i++) {
        auto circle = std::make_shared<ofxBox2dCircle>();
        
        // if(i%2==0){
        //   circle.get()->setPhysics(10.0, 0.53, 0.9);
        
        // }else{
        circle.get()->setPhysics(3, 0.53, 5);
        
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
            joint.get()->setup(box2d.getWorld(), anchor.body, circles[i].get()->body,20,1);
        }
        else {
            joint.get()->setup(box2d.getWorld(), circles[i-1].get()->body, circles[i].get()->body,10,10);
        }
        joint.get()->setLength(jointlength);
        joints.push_back(joint);
    }
    
    auto joint = std::make_shared<ofxBox2dJoint>();
    joint.get()->setup(box2d.getWorld(), circles[circles.size()-1].get()->body, anchor2.body,20,0.1);
    joint.get()->setLength(jointlength);
    
    joints.push_back(joint);
    
    
}

void Physics::update(){
    box2d.update();
    
    ofVec2f mouse(ofGetMouseX(), ofGetMouseY());
    float minDis = ofGetMousePressed() ? 100 : 50;
    
    for(int i=0; i<circles.size(); i++) {
        float dis = mouse.distance(circles[i].get()->getPosition());
     //   if(bIsMouseActive)circles[i].get()->addRepulsionForce(mouse, 9);
       if(dis < minDis && bIsMouseActive) circles[i].get()->addRepulsionForce(mouse,10);
       //  else circles[i].get()->addAttractionPoint(mouse, 4.0);
       // circles[i].get()->setDamping(0.98);
        
    }
    
    anchor.addAttractionPoint(mouse,40);

    
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


void Physics::draw(){
    ofSetColor(0);
    ofDrawRectangle(0,0,ofGetWidth(),ofGetHeight());
    
    ofSetHexColor(0xf2ab01);


    if(APPC->debug){
        
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
    }
    
    line=line.getResampledBySpacing(2);
   // line=line.getSmoothed(5);
    
    ofSetColor(255);
    ofSetLineWidth(3);
    line.draw();
    
    string info = "";
    info += "Press [n] to add a new joint\n";
    info += "click and pull the chain around\n";
    info += "FPS: "+ofToString(ofGetFrameRate(), 1)+"\n";
    ofSetHexColor(0x444342);
    ofDrawBitmapString(info, 30, 30);
    
}

void Physics::exit(){
    cout<<"exit Physics"<<endl;
}



//KEY LISTENER
//--------------------------------------------------------------
void Physics::keyPressed(ofKeyEventArgs &e){
    if(e.key=='r'){
        anchor.setPhysics(10, 0.5, 0.9);
        anchor.body->SetType(b2_dynamicBody);
        
    }
    
    
    if(e.key=='R'){
        anchor2.setPhysics(10, 1, 10);
        anchor2.body->SetType(b2_dynamicBody);
        
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
        anchor.setPosition(0, ofGetMouseY());
        
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
void Physics::mouseMoved(ofMouseEventArgs &a){

}

//--------------------------------------------------------------
void Physics::mouseDragged(ofMouseEventArgs &a){
    
}

//--------------------------------------------------------------
void Physics::mousePressed(ofMouseEventArgs &a){
    cout<<"Mouse"<< anchor.isFixed()<<endl;
    anchor.setPhysics(0, 0.5, 0.9);
    anchor.body->SetType(b2_staticBody);
    //anchor.setup(box2d.getWorld(), a.x, 0, 10);
    

}

//--------------------------------------------------------------
void Physics::mouseReleased(ofMouseEventArgs &a){
    
}

void Physics::mouseScrolled(ofMouseEventArgs &a){
    
}

//--------------------------------------------------------------
void Physics::mouseEntered(ofMouseEventArgs &a){
    
}

//--------------------------------------------------------------
void Physics::mouseExited(ofMouseEventArgs &a){
    
}


void Physics::toggleMouseActive(){
    bIsMouseActive=!bIsMouseActive;
}


void Physics::turnOn(){
    if(!bAddedListeners){
        addListeners();
    }
    bAddedListeners=true;
}

void Physics::turnOff(){
    if(bAddedListeners){
        removeListeners();
    }
    bAddedListeners=false;
}
